#include "../../interface/PhaseISplitClusterAnalyzer.h"

PhaseISplitClusterAnalyzer::PhaseISplitClusterAnalyzer(const edm::ParameterSet& t_iConfig): 
	m_iConfig(t_iConfig),
	m_outputFilePath(t_iConfig.getUntrackedParameter<std::string>("outputFileName", DEFAULT_OUTPUT_FILE_PATH)),
	m_isEventMC(false),
	m_pileup(NOVAL_I)
{
	m_clustersToken      = consumes<edmNew::DetSetVector<SiPixelCluster>>(edm::InputTag("siPixelClusters"));
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
	m_pileup = getPileupInfo(m_puInfoCollectionHandle);
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

// This follows a weird logic:
// The number of primary vertices is stored in the first 
// pileup DOES_NOT_WORK, use the provided shell script instead

float PhaseISplitClusterAnalyzer::getPileupInfo(const edm::Handle<std::vector<PileupSummaryInfo>>& puInfoCollectionHandle)
{
	if (!m_isEventMC) return NOVAL_F;
	static int reportNum = 0;
	if(!puInfoCollectionHandle.isValid()) 
	{
		// Abandon hope all ye who enter here
		if(reportNum++ < 20) std::cout << "Warning: The provided pileup info is invalid." << std::endl;
		return NOVAL_F;
	}
	auto zerothPileup = std::find_if(puInfoCollectionHandle -> rbegin(), puInfoCollectionHandle -> rend(), [] (const auto& puInfo) { return puInfo.getBunchCrossing() == 0; });
	if(zerothPileup == puInfoCollectionHandle -> rend()) 
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