#include "../../interface/PhaseISplitClusterAnalyzer.h"

PhaseISplitClusterAnalyzer::PhaseISplitClusterAnalyzer(const edm::ParameterSet& t_iConfig): 
	m_iConfig(t_iConfig),
	m_outputFilePath(t_iConfig.getUntrackedParameter<std::string>("outputFileName", DEFAULT_OUTPUT_FILE_PATH))
{
	m_clustersToken = consumes<edmNew::DetSetVector<SiPixelCluster>>(edm::InputTag("siPixelClusters"));
}

PhaseISplitClusterAnalyzer::~PhaseISplitClusterAnalyzer()
{
	savePerEventDistributions();
	m_outputFile -> Close();
}

void PhaseISplitClusterAnalyzer::beginJob()
{
	m_outputFile = new TFile(m_outputFilePath.c_str(), "RECREATE");
	generateHistogramCollections();
}

void PhaseISplitClusterAnalyzer::endJob()
{
	// Don't put anything here that you want to run even if an exception is thrown
}

void PhaseISplitClusterAnalyzer::analyze(const edm::Event& t_iEvent, const edm::EventSetup& t_iSetup)
{
	m_iEvent = &t_iEvent;
	// Set the event identifiers
	updateTimestamp();
	m_siPixelCoordinates.init(t_iSetup);
	// Get the cluster collection in the event
	m_iEvent -> getByToken(m_clustersToken, m_clusterCollectionHandle);

	edm::ESHandle<TrackerGeometry> trackerGeometryHandle;
	t_iSetup.get<TrackerDigiGeometryRecord>().get(trackerGeometryHandle);
	m_trackerGeometry = trackerGeometryHandle.product();

	edm::ESHandle<PixelClusterParameterEstimator> pixelClusterParameterEstimatorHandle;
	t_iSetup.get<TkPixelCPERecord>().get("PixelCPEGeneric", pixelClusterParameterEstimatorHandle);
	m_pixelClusterParameterEstimator = pixelClusterParameterEstimatorHandle.product();
	// Module cluster plots
	handleModuleClusterPlots();
	// Generate statistics
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
