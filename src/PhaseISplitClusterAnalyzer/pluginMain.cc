#include "../../interface/PhaseISplitClusterAnalyzer.h"

PhaseISplitClusterAnalyzer::PhaseISplitClusterAnalyzer(const edm::ParameterSet& t_iConfig): 
	m_iConfig(t_iConfig),
	m_outputFilePath(t_iConfig.getUntrackedParameter<std::string>("outputFileName", DEFAULT_OUTPUT_FILE_PATH)),
	m_isEventMC(false),
	m_pileup(NOVAL_I)
{
	m_clustersToken      = consumes<edmNew::DetSetVector<SiPixelCluster>>(edm::InputTag("siPixelClusters"));
	m_digiFlagsToken     = consumes<edm::DetSetVector<PixelDigi>>        (edm::InputTag("simSiPixelDigis", "dcolLostNeighbourDigiFlags"));
	m_pileupSummaryToken = consumes<std::vector<PileupSummaryInfo>>      (edm::InputTag("addPileupInfo"));
	// const std::string command = "echo $(ssh ahunyadi@lxplus.cern.ch \"/afs/cern.ch/user/a/ahunyadi/.local/bin/brilcalc lumi --byls -u /nb --minBiasXsec 78100 --begin 5659 --end 9999 | head -n-8 | tail -n+5 | sed \\\"s;|;;g;s;:; ;g\\\"\")";
	// std::cout << "Executing test bash command: \"" << command << "\"" << std::endl;
	// std::string message = executeBashScript(command);
	// std::cout << "Done executing the command. Press any key to continue..." << std::endl;
	// std::cin.get();
	// std::cout << "Output: " << message << std::endl;
	// std::cin.get();
	// std::cout << "Press any key to continue..." << std::endl;
}

PhaseISplitClusterAnalyzer::~PhaseISplitClusterAnalyzer() {}

void PhaseISplitClusterAnalyzer::beginJob()
{
	m_outputFile = new TFile(m_outputFilePath.c_str(), "RECREATE");
	generateHistogramCollections();
}

void PhaseISplitClusterAnalyzer::endJob()
{
	// Don't put anything here that you want to run even if an exception is thrown
	savePerEventDistributions();
	saveCummulativeDistributions();
	savePileupDependenceDistributions();
	m_outputFile -> Close();
}

void PhaseISplitClusterAnalyzer::analyze(const edm::Event& t_iEvent, const edm::EventSetup& t_iSetup)
{
	m_iEvent = &t_iEvent;
	// Get event type
	m_isEventMC = m_iEvent -> id().run() == 1;
	static bool lastEventType = !m_isEventMC;
	if(lastEventType != m_isEventMC)
	{
		lastEventType = m_isEventMC;
		std::cout << "Deduced data type: "<< (m_isEventMC ? "MONTE-CARLO" : "REAL RAW DATA") << "." << std::endl;
	}
	// Set the event identifiers
	updateTimestamp();
	m_siPixelCoordinates.init(t_iSetup);
	// Get the cluster collection for the event
	m_iEvent -> getByToken(m_clustersToken, m_clusterCollectionHandle);
	// Get digi flags for split clusters
	m_iEvent -> getByToken(m_digiFlagsToken, m_digiFlagsCollectionHandle);
	recalculateDetIdToMarkerPtrMap();
	// The checks Should be disabled when the pileup mixing starts to work
	checkDetIdToMarkerPtrMap();
	// Get the pileup information for the event
	m_iEvent -> getByToken(m_pileupSummaryToken, m_puInfoCollectionHandle);
	// Get the tracker geometry object
	edm::ESHandle<TrackerGeometry> trackerGeometryHandle;
	t_iSetup.get<TrackerDigiGeometryRecord>().get(trackerGeometryHandle);
	m_trackerGeometry = trackerGeometryHandle.product();
	// Get the pixel parameter estimator object
	edm::ESHandle<PixelClusterParameterEstimator> pixelClusterParameterEstimatorHandle;
	t_iSetup.get<TkPixelCPERecord>().get("PixelCPEGeneric", pixelClusterParameterEstimatorHandle);
	m_pixelClusterParameterEstimator = pixelClusterParameterEstimatorHandle.product();
	// Get the pileup from the pileup info object
	m_pileup = getPileupInfo();
	// Module cluster plots
	handleModuleClusterPlots();
	// Generate statistics
	std::cout << "m_pileup: " << m_pileup << std::endl;
	handleEventStatisticsForDistributions();
}

void PhaseISplitClusterAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{

}

void PhaseISplitClusterAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{

}

void PhaseISplitClusterAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{

}

void PhaseISplitClusterAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{

}

void PhaseISplitClusterAnalyzer::updateRunNumber()
{
	int newRunNumber = m_iEvent -> id().run();
	if(m_runNumber == newRunNumber)
	{
		m_isNewRun = false;
		return;
	}
	m_runNumber = newRunNumber;
	m_isNewRun = true;
}

void PhaseISplitClusterAnalyzer::updateLuminosityBlockNumber()
{
	int newLuminosityBlock = m_iEvent -> luminosityBlock();
	if(m_luminosityBlock == newLuminosityBlock)
	{
		m_isNewLuminosityBlock = false;
		return;
	}
	m_luminosityBlock = newLuminosityBlock;
	m_isNewLuminosityBlock = true;
}

void PhaseISplitClusterAnalyzer::updateTimestamp()
{
	updateRunNumber();
	updateLuminosityBlockNumber();
	m_eventNumber = m_iEvent -> id().event();
}

[[deprecated]] void PhaseISplitClusterAnalyzer::recalculateDetIdToMarkerPtrMapOld()
{
	for(const auto& markerSet: *m_digiFlagsCollectionHandle)
	{
		m_detIdToMarkerPtrMap.insert(std::make_pair<DetId, const edm::DetSet<PixelDigi>*>(markerSet.detId(), &markerSet));
	}
}

// If the marker positions are equivalent to those of the digis from the clusters -> add a pointer to the markers on module
// Otherwise copy all the markers that correspond to clusters to a new container and add 0-s to every other pixel position
// Currently: generating another collection with all the markers and mocked markers available
void PhaseISplitClusterAnalyzer::recalculateDetIdToMarkerPtrMap()
{
	m_detIdToMarkerPtrMap.clear();
	m_fakeFlagsManager.clear();
	std::size_t numMockedMarkers = 0;
	std::size_t numRealMarkers   = 0;
	// Making sure that each cluster-pixel has a corresponding marker 
	// for(const auto& clusterCollectionOnModule: *m_clusterCollectionHandle)
	for(auto clustersOnModuleIt = m_clusterCollectionHandle -> begin(), lastClustersModuleIt = m_clusterCollectionHandle -> end(); clustersOnModuleIt != lastClustersModuleIt; ++clustersOnModuleIt)
	{
		DetId detId(clustersOnModuleIt -> detId());
		auto mockedFlagsOnModule = std::make_unique<edm::DetSet<PixelDigi>>();
		const auto findResult = std::find_if(m_digiFlagsCollectionHandle -> begin(), m_digiFlagsCollectionHandle -> end(), [&] (const auto& e) { return e.detId() == detId; });
		// For modules that have no corresponding markers omit search
		if(findResult == m_digiFlagsCollectionHandle -> end())
		{
			// Hopefully std::make_unique move constructs
			// std::cout << "Mocking a whole module, channels: ";
			forEachPixelForClustersOnModule(clustersOnModuleIt -> begin(), clustersOnModuleIt -> end(), [&] (const auto& pixel)
			{
				// std::cout << getPixelChannel(pixel) << " ";
				++numMockedMarkers;
				mockedFlagsOnModule -> data.emplace_back(PixelDigi(pixel.x, pixel.y, 0));
			});
			std::cout << std::endl;
		}
		// For modules with corresponding markers
		else
		{
			// markerset := reference to the digi flags collection for the module of the same detId as the clusters collection
			const auto& markerSet = *findResult;
			forEachPixelForClustersOnModule(clustersOnModuleIt -> begin(), clustersOnModuleIt -> end(), [&] (const auto& pixel)
			{
				// This could be moved into the lambda capture list to decrease compile overhead
				// but gcc seems to have an issue with capturing this, even though getPixelChannel is static 
				int channel = getPixelChannel(pixel);
				auto pixelMarkerIt = std::find_if(markerSet.begin(), markerSet.end(), [&] (const auto& e) { return channel == e.channel(); } );
				// Pixel is available in the markerSet
				if(pixelMarkerIt == markerSet.end())
				{
					++numMockedMarkers;
					// std::cout << "Adding mocked pixel of channel: " << getPixelChannel(pixel) << std::endl;
					mockedFlagsOnModule -> data.emplace_back(PixelDigi(pixel.x, pixel.y, 0));
				}
				else
				{
					++numRealMarkers;
					// std::cout << "Adding real pixel of channel: " << getPixelChannel(pixel) << std::endl;
					// if(getPixelChannel(pixel) )
					mockedFlagsOnModule -> data.emplace_back(*pixelMarkerIt);
				}
			});
		}
		m_fakeFlagsManager.emplace_back(std::move(mockedFlagsOnModule));
		const edm::DetSet<PixelDigi>* fakeDigisPtr = m_fakeFlagsManager.back().get();
		m_detIdToMarkerPtrMap.insert(std::make_pair(detId, fakeDigisPtr));
	}
	std::cout << "numMockedMarkers: " << numMockedMarkers << ", numRealMarkers: " << numRealMarkers << ", real marker rate: " 
		<< ((numRealMarkers + numMockedMarkers == 0) ? 0.0 : static_cast<double>(numRealMarkers) / (numRealMarkers + numMockedMarkers)) << std::endl;
}

void PhaseISplitClusterAnalyzer::checkDetIdToMarkerPtrMap() const
{
	if(m_detIdToMarkerPtrMap.size() != m_clusterCollectionHandle -> size())
	{
		std::cout << "m_detIdToMarkerPtrMap.size(): " << m_detIdToMarkerPtrMap.size() << std::endl;
		std::cout << "m_clusterCollectionHandle -> size(): " << m_clusterCollectionHandle -> size() << std::endl;
		e_marker_signal_mismatch();
	}
	for(auto clustersOnModuleIt = m_clusterCollectionHandle -> begin(), lastClustersModuleIt = m_clusterCollectionHandle -> end(); clustersOnModuleIt != lastClustersModuleIt; ++clustersOnModuleIt)
	{
		DetId detId(clustersOnModuleIt -> id());
		const auto findResult = m_detIdToMarkerPtrMap.find(detId);
		// Throw on missing detId
		if(findResult == m_detIdToMarkerPtrMap.end()) e_marker_detId();
		// Get pointer to the proper digi flag detset
		const edm::DetSet<PixelDigi>* digiFlagsOnModulePtr = findResult -> second;
		const std::set<int> pixelChannels  = getAllDigiChannelsFromClustersOnModule<std::set<int>>(*clustersOnModuleIt);
		const std::set<int> markerChannels = getAllDigiChannelsFromDigisOnModule<std::set<int>>(*digiFlagsOnModulePtr);
		const bool sets_equal = std::equal(pixelChannels.begin(), pixelChannels.end(), markerChannels.begin(), markerChannels.end());
		if(!sets_equal)
		{
			std::cout << std::distance(m_clusterCollectionHandle -> begin(), clustersOnModuleIt) << std::endl;
			std::cout << "Pixel channels: " << std::endl;
			std::copy(pixelChannels.begin(), pixelChannels.end(), std::ostream_iterator<int>(std::cout, " "));
			std::cout << std::endl;
			std::cout << "Marker channels: " << std::endl;
			std::copy(markerChannels.begin(), markerChannels.end(), std::ostream_iterator<int>(std::cout, " "));
			std::cout << std::endl;
			std::cout << "pixelChannels.size(): " << pixelChannels.size() << std::endl;
			std::cout << "markerChannels.size(): " << markerChannels.size() << std::endl;
			dumpClusterDigiAndFlagChannels();
			e_marker_signal_mismatch();
		}
	}
}

void PhaseISplitClusterAnalyzer::dumpClusterDigiAndFlagChannels() const
{
	std::cout << "--- Begin cluster digi and flag dump ---" << std::endl;
	for(auto clustersOnModuleIt = m_clusterCollectionHandle -> begin(), lastClustersModuleIt = m_clusterCollectionHandle -> end(); clustersOnModuleIt != lastClustersModuleIt; ++clustersOnModuleIt)
	{
		DetId detId(clustersOnModuleIt -> detId());
		std::cout << "Details of digis and flags on det. id. " << detId.rawId() << ":\n";
		std::cout << "   Cluster digis: \n";
		std::cout << "   {";
		forEachPixelForClustersOnModule(clustersOnModuleIt -> begin(), clustersOnModuleIt -> end(), [&] (const auto& pixel)
		{
			std::cout << getPixelChannel(pixel) << " ";
		});
		std::cout << "}\n";
		std::cout << "   Pixel digis in the original collection: \n";
		auto digisOnModuleAt = std::find_if(m_digiFlagsCollectionHandle -> begin(), m_digiFlagsCollectionHandle -> end(), [&] (const auto& e) { return e.detId() == detId; });
		if(digisOnModuleAt == m_digiFlagsCollectionHandle -> end()) std::cout << "   [NOT_FOUND]\n";
		else
		{
			std::cout << "   {";
			std::for_each(digisOnModuleAt -> begin(), digisOnModuleAt -> end(), printPixelChannelSpaceDelimiter);
			std::cout << "}\n";
		}
		std::cout << "   Pixel digis in the helper collection: \n";
		const auto digisInHelperCollectionIt = m_detIdToMarkerPtrMap.find(detId);
		if(digisInHelperCollectionIt == m_detIdToMarkerPtrMap.end()) std::cout << "   [NOT_FOUND]\n";
		else
		{
			std::cout << "   {";
			std::for_each(digisInHelperCollectionIt -> second -> begin(), digisInHelperCollectionIt -> second -> end(), printPixelChannelSpaceDelimiter);
			std::cout << "}\n";
		}
	}
	std::cout << "--- End cluster digi and flag dump ---" << std::endl;
}

std::vector<std::vector<int>> PhaseISplitClusterAnalyzer::getPixelMarkerValuesOnDetUnit(const edmNew::DetSet<SiPixelCluster>& t_clustersOnModule, const DetId& t_detId) const
{
	// Digi markers
	auto findResult = m_detIdToMarkerPtrMap.find(t_detId);
	// Throw on missing detId
	if(findResult == m_detIdToMarkerPtrMap.end()) e_marker_detId();
	const edm::DetSet<PixelDigi>* digiFlagsOnModulePtr = findResult -> second;
	std::vector<std::vector<int>> pixelMarkerValuesOnDetUnit;
	for(auto clusterIt = t_clustersOnModule.begin(), lastClusterIt = t_clustersOnModule.end(); clusterIt != lastClusterIt; ++clusterIt)
	{
		std::vector<int> pixelMarkers {};
		// std::cout << "clusterIt -> size(): " << clusterIt -> size() << std::endl;
		// std::cout << "clusterIt -> pixels().size(): " << clusterIt -> pixels().size() << std::endl;
		// std::cout << "digiFlagsOnModulePtr -> size(): " << digiFlagsOnModulePtr -> size() << std::endl;
		for(std::size_t pixelIndex = 0, maxIndex = clusterIt -> size(); pixelIndex < maxIndex; ++pixelIndex)
		{
			pixelMarkers.push_back(getDigiMarkerValue(clusterIt -> pixel(pixelIndex), *digiFlagsOnModulePtr));
		}
		pixelMarkerValuesOnDetUnit.emplace_back(std::move(pixelMarkers));
	}
	return pixelMarkerValuesOnDetUnit;
}

template <typename T>
T PhaseISplitClusterAnalyzer::getAllDigiChannelsFromClustersOnModule(const edmNew::DetSet<SiPixelCluster>& t_clustersOnModule)
{
	T res;
	std::for_each(t_clustersOnModule.begin(), t_clustersOnModule.end(), [&] (const auto& cluster)
	{
		std::vector<int> clusterPixelChannels = getClusterPixelChannels(cluster);
		std::move(clusterPixelChannels.begin(), clusterPixelChannels.end(), std::inserter(res, res.begin()));
	});
	return res;
}

template <typename T>
T PhaseISplitClusterAnalyzer::getAllDigiChannelsFromDigisOnModule(const edm::DetSet<PixelDigi>& t_digisOnModule)
{
	T res;
	std::transform(t_digisOnModule.begin(), t_digisOnModule.end(), std::inserter(res, res.begin()), [] (const auto& e) { return e.channel(); });
	return res;
}

std::vector<int> PhaseISplitClusterAnalyzer::getClusterPixelChannels(const SiPixelCluster& t_cluster)
{
	std::vector<int> res;
	for(std::size_t clusterIndex = 0, maxIndex = t_cluster.size(); clusterIndex < maxIndex; ++clusterIndex)
	{
		res.push_back(getPixelChannel(t_cluster.pixel(clusterIndex)));
	}
	return res;
}

int PhaseISplitClusterAnalyzer::getPixelChannel(const SiPixelCluster::Pixel& t_pixel)
{
	return PixelDigi::pixelToChannel(t_pixel.x, t_pixel.y); 
}

void PhaseISplitClusterAnalyzer::printPixelChannel(const PixelDigi& t_pixel, const std::string& t_delimiter)
{
	std::cout << t_pixel.channel() << t_delimiter;
}

void PhaseISplitClusterAnalyzer::printPixelChannelSpaceDelimiter(const PixelDigi& t_pixel)
{
	printPixelChannel(t_pixel, " ");
}

template <typename UnaryOperation>
void PhaseISplitClusterAnalyzer::forEachPixelForClustersOnModule(const edmNew::DetSet<SiPixelCluster>::const_iterator& t_begin, const edmNew::DetSet<SiPixelCluster>::const_iterator& t_end, UnaryOperation t_op)
{
	for(auto clusterIt = t_begin; clusterIt != t_end; ++clusterIt)
	{
		for(std::size_t pixelIndex = 0, maxIndex = clusterIt -> size(); pixelIndex < maxIndex; ++pixelIndex)
		{
			auto pixel = clusterIt -> pixel(pixelIndex);
			t_op(pixel);
		}
	}
}

// Remove optimization flag attribute when the reason of crashing disappeared
// int __attribute__((optimize("O0"))) PhaseISplitClusterAnalyzer::getDigiMarkerValue(const SiPixelCluster::Pixel& pixelToCheck, const edm::DetSet<PixelDigi>& t_digiFlags)
int PhaseISplitClusterAnalyzer::getDigiMarkerValue(const SiPixelCluster::Pixel& t_pixel, const edm::DetSet<PixelDigi>& t_digiFlags)
{
	auto findResult = std::find_if(t_digiFlags.begin(), t_digiFlags.end(), [channelToFind = PixelDigi::pixelToChannel(t_pixel.x, t_pixel.y)] (const auto& marker)
	{
		if(channelToFind == marker.channel()) return true;
		return false;
	});
	// Throws on no result
	if(findResult == t_digiFlags.end()) e_marker_not_found();
	return findResult -> adc();
}

int PhaseISplitClusterAnalyzer::getDigiMarkerValue(const int t_x, const int t_y, const edm::DetSet<PixelDigi>& digiFlags)
{
	int markerValue = 0;
	int channel = PixelDigi::pixelToChannel(t_x, t_y);
	for(const auto& digiInSet: digiFlags)
	{
		int channelToCompareWith = digiInSet.channel();
		if(channel == channelToCompareWith)
		{
			markerValue = digiInSet.adc();
		}
	}
	return markerValue;
}

// This follows a weird logic:
// The number of primary vertices is stored in the first 
// PILEUP FOR DATA DOES NOT WORK, use the provided shell script instead to get the pileuptable for the runs
float PhaseISplitClusterAnalyzer::getPileupInfo() const
{
	if (!m_isEventMC) return NOVAL_F;
	static int reportNum = 0;
	if(!m_puInfoCollectionHandle.isValid()) 
	{
		// Abandon hope all ye who enter here
		if(reportNum++ < 20) std::cout << "Warning: The provided pileup info is invalid." << std::endl;
		return NOVAL_F;
	}
	auto zerothPileup = std::find_if(m_puInfoCollectionHandle -> rbegin(), m_puInfoCollectionHandle -> rend(), [] (const auto& puInfo) { return puInfo.getBunchCrossing() == 0; });
	if(zerothPileup == m_puInfoCollectionHandle -> rend()) 
	{
		if(reportNum++ < 20) std::cout << "Error: Cannot find the in-time pileup info." << std::endl;
		return NOVAL_F;
	}
	return zerothPileup -> getTrueNumInteractions();
}

std::string PhaseISplitClusterAnalyzer::executeBashScript(const std::string& t_command)
{
	static_assert(16777216 < std::numeric_limits<std::size_t>::max(), "The system has too small numeric limit for size_t, and therefore unable to handle all the lumisections in the pileup-table.");
	// static constexpr std::size_t BUFFER_SIZE = 16777216; // 16 Mb
	static constexpr std::size_t BUFFER_SIZE = 1024; // 1 Kb
	std::array<char, BUFFER_SIZE> buffer;
	std::string result = "";
	FILE* pipe(popen(t_command.c_str(), "r"));
	// std::unique_ptr<FILE> pipe(popen(t_command, "r"), pclose);
	if(!pipe)
	{
		throw std::runtime_error("popen() failed!");
	}
	while(!feof(pipe))
	{
		if(fgets(buffer.data(), BUFFER_SIZE, pipe) != nullptr)
		{
			result += buffer.data();
		}
	}
	pclose(pipe);
	return result;
}

// Returns an object that maps a run and lumisection pair to a pileup value
// Expected input (from the command brilcalc lumi --byls -u /nb --minBiasXsec 78100 --begin 5659 --end 9999 | head -n-8 | tail -n+5 | sed "s;|;;g;s;:; ;g"): 
//  305636 6325  2398 2398  10/26/17 04 36 12  STABLE BEAMS  6500    111.591         109.831        21.3    HFET
std::pair<std::pair<int, int>, float> PhaseISplitClusterAnalyzer::transformBrilcalcLineToPileupTable(const std::string& t_line)
{
	//                  305636 6325   2398   2398   10/26/17   04     36 12  STABLE BEAMS   6500   111.591   109.831   21.3      HFET
	std::regex regex(R"((\d+) +(\d+) +(\d+) +(\d+) +([\d\/]+) +(\d+) +(\d+) +(\d+)[ a-zA-Z]+(\d+) +([\d.]+) +([\d.]+) +([\d.]+) +.+)");
	std::smatch match;
	if(!(std::regex_match(t_line, match, regex))) e_brilcalc_regex();
	// 0:  full match
	// 1:  305636 
	// 2:  6325
	// 3:  2398
	// 4:  2398
	// 5:  10/26/17 
	// 6:  04
	// 7:  36 
	// 8:  12
	// 9:  STABLE BEAMS
	// 10: 6500
	// 11: 111.591 
	// 12: 109.831 
	// 13: 21.3
	// 14: HFET
	return std::make_pair(std::make_pair(std::stoi(match.str(1)), std::stoi(match.str(3))), std::stof(match.str(13)));
}

void PhaseISplitClusterAnalyzer::getModuleData(ModuleData& t_mod, const DetId& t_detId)
{
	t_mod.init();
	t_mod.det  = t_detId.subdetId() - 1;
	t_mod.shl  = m_siPixelCoordinates.quadrant(t_detId);
	t_mod.side = m_siPixelCoordinates.side(t_detId);
	if(t_detId.subdetId() == PixelSubdetector::PixelBarrel)
	{
		t_mod.sec     = m_siPixelCoordinates.sector(t_detId);
		t_mod.half    = m_siPixelCoordinates.half(t_detId);
		t_mod.layer   = m_siPixelCoordinates.layer(t_detId);
		t_mod.flipped = m_siPixelCoordinates.flipped(t_detId); // opposite of outer
		t_mod.ladder = m_siPixelCoordinates.signed_ladder(t_detId);
		t_mod.module = m_siPixelCoordinates.signed_module(t_detId);
	}
	else if(t_detId.subdetId() == PixelSubdetector::PixelEndcap)
	{
		t_mod.ring   = m_siPixelCoordinates.ring(t_detId);
		t_mod.panel  = m_siPixelCoordinates.panel(t_detId);
		t_mod.module = m_siPixelCoordinates.module(t_detId);
		t_mod.disk  = m_siPixelCoordinates.signed_disk(t_detId);
		t_mod.blade = m_siPixelCoordinates.signed_blade(t_detId);
	}
	t_mod.rawid = t_detId.rawId();
	t_mod.fedid = m_siPixelCoordinates.fedid(t_detId);
}