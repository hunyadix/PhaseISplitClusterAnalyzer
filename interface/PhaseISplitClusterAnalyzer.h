#pragma once

// CMSSW code
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "DataFormats/Common/interface/ConditionsInEdm.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/SiPixelRawData/interface/SiPixelRawDataError.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
#include "TrackingTools/GeomPropagators/interface/Propagator.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHit.h"
#include "TrackingTools/KalmanUpdators/interface/Chi2MeasurementEstimator.h"
#include "RecoTracker/MeasurementDet/interface/MeasurementTracker.h"
#include "RecoTracker/MeasurementDet/interface/MeasurementTrackerEvent.h"
#include "TrackingTools/MeasurementDet/interface/LayerMeasurements.h"
#include "RecoLocalTracker/ClusterParameterEstimator/interface/PixelClusterParameterEstimator.h"
#include "RecoTracker/Record/interface/CkfComponentsRecord.h"
#include "RecoLocalTracker/Records/interface/TkPixelCPERecord.h"
#include "DQM/SiPixelPhase1Common/interface/SiPixelCoordinates.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "../interface/DataStructures_v6.h"

// Helpers to save canvases
#include "../interface/HelperFunctionsCommon.h"

// ROOT Libraries
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
// #include <TH1D.h>
#include <TH2D.h>
#include <TRandom3.h>
#include <TEfficiency.h>

// C++
#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <exception>
#include <cstdio>
#include <cstdlib>
#include <regex>
#include <memory>

// Compiler directives
#define EDM_ML_LOGDEBUG
#define ML_DEBUG

struct ModuleClusterPlotSetup
{
	std::string name;
	int layer;
	int module;
	int ladder;
	// ModuleClusterPlotSetup(const std::string& t_name, int t_layer, int t_ladder, int t_module): name(t_name), layer(t_layer), ladder(t_ladder), module(t_module) {}
};

class PhaseISplitClusterAnalyzer: public edm::EDAnalyzer
{
	public:
		static constexpr int MODULE_CLUSTER_PLOT_GENERATION_FREQUENCY = 1000;
		static constexpr int MODULE_CLUSTER_PLOT_EVENTS_TO_MERGE      = 50;

		const std::string DEFAULT_OUTPUT_FILE_PATH = "Ntuple.root";
	public:
		enum ModuleClusterPlotType
		{
			LOW_ETA = 0,
			HIGH_ETA_1,
			HIGH_ETA_2,
			NUM_MODULE_CLUSTER_PLOT_TYPES
		};
		std::array<ModuleClusterPlotSetup, NUM_MODULE_CLUSTER_PLOT_TYPES> moduleClusterPlotTypeDetails
		{{
			{ "Low eta",  1, 1, 2 },
			{ "High eta 1", 1, 4, 1 },
			{ "High eta 2", 1, 4, 6 } 
		}};
		// Helper classes
		enum PerEventCountable
		{
			NUM_CLUSTERS_IN_EVENT,                NUM_CLUSTERS_LOW_ETA,                  NUM_CLUSTERS_HIGH_ETA,
			NUM_PAIR_CANDIDATES_IN_EVENT,         NUM_PAIR_CANDIDATES_LOW_ETA,           NUM_PAIR_CANDIDATES_HIGH_ETA,
			EVEN_COL_CANDIDATES_IN_EVENT,         EVEN_COL_CANDIDATES_LOW_ETA,           EVEN_COL_CANDIDATES_HIGH_ETA,
			AVG_CLUSTER_SIZE_X_IN_EVENT,          AVG_CLUSTER_SIZE_X_LOW_ETA,            AVG_CLUSTER_SIZE_X_HIGH_ETA,
			AVG_CLUSTER_SIZE_Y_IN_EVENT,          AVG_CLUSTER_SIZE_Y_LOW_ETA,            AVG_CLUSTER_SIZE_Y_HIGH_ETA,
			AVG_CLUSTER_SIZE_PIXELS_IN_EVENT,     AVG_CLUSTER_SIZE_PIXELS_LOW_ETA,       AVG_CLUSTER_SIZE_PIXELS_HIGH_ETA,
			NUM_PAIR_CANDIDATES_IN_BPIX,          NUM_PAIR_CANDIDATES_IN_FPIX,     
			EVEN_COL_CANDIDATES_IN_BPIX,          EVEN_COL_CANDIDATES_IN_FPIX,     
			NUM_CLUSTERS_LAYER_1,                 NUM_CLUSTERS_LAYER_2,                  NUM_CLUSTERS_LAYER_3,                NUM_CLUSTERS_LAYER_4,
			NUM_PAIR_CANDIDATES_LAYER_1,          NUM_PAIR_CANDIDATES_LAYER_2,           NUM_PAIR_CANDIDATES_LAYER_3,         NUM_PAIR_CANDIDATES_LAYER_4,
			EVEN_COL_CANDIDATES_LAYER_1,          EVEN_COL_CANDIDATES_LAYER_2,           EVEN_COL_CANDIDATES_LAYER_3,         EVEN_COL_CANDIDATES_LAYER_4,
			AVG_CLUSTER_SIZE_X_LAYER_1,           AVG_CLUSTER_SIZE_X_LAYER_2,            AVG_CLUSTER_SIZE_X_LAYER_3,          AVG_CLUSTER_SIZE_X_LAYER_4,
			AVG_CLUSTER_SIZE_Y_LAYER_1,           AVG_CLUSTER_SIZE_Y_LAYER_2,            AVG_CLUSTER_SIZE_Y_LAYER_3,          AVG_CLUSTER_SIZE_Y_LAYER_4,
			AVG_CLUSTER_SIZE_PIXELS_LAYER_1,      AVG_CLUSTER_SIZE_PIXELS_LAYER_2,       AVG_CLUSTER_SIZE_PIXELS_LAYER_3,     AVG_CLUSTER_SIZE_PIXELS_LAYER_4,
			NUM_MARKED_PAIR_CANDIDATES_IN_EVENT,  NUM_MARKED_PAIR_CANDIDATES_LOW_ETA,    NUM_MARKED_PAIR_CANDIDATES_HIGH_ETA, NUM_UNMARKED_PAIR_CANDIDATES_IN_EVENT,
			NUM_UNMARKED_PAIR_CANDIDATES_LOW_ETA, NUM_UNMARKED_PAIR_CANDIDATES_HIGH_ETA, MARKED_PAIR_CANDIDATE_RATE_IN_EVENT, MARKED_PAIR_CANDIDATE_RATE_LOW_ETA, 
			MARKED_PAIR_CANDIDATE_RATE_HIGH_ETA,  EVEN_COL_CLUSTER_RATE_TOTAL,           EVEN_COL_CLUSTER_RATE_LOW_ETA,       EVEN_COL_CLUSTER_RATE_HIGH_ETA,
			NUMBER_OF_EVENT_COUNTABLES
		};
		enum Distributions
		{
			CLUSTER_SIZE_X_TOTAL,            CLUSTER_SIZE_X_LOW_ETA,             CLUSTER_SIZE_X_HIGH_ETA,
			CLUSTER_SIZE_Y_TOTAL,            CLUSTER_SIZE_Y_LOW_ETA,             CLUSTER_SIZE_Y_HIGH_ETA,
			CLUSTER_SIZE_PIX_TOTAL,          CLUSTER_SIZE_PIX_LOW_ETA,           CLUSTER_SIZE_PIX_HIGH_ETA,
			NUMBER_OF_PIXELS_PER_COL_BPIX,   NUMBER_OF_PIXELS_PER_COL_LOW_ETA,   NUMBER_OF_PIXELS_PER_COL_HIGH_ETA,
			CLUSTER_BOUNDARY_PIX_COLS_BPIX,  CLUSTER_BOUNDARY_PIX_COLS_LOW_ETA,  CLUSTER_BOUNDARY_PIX_COLS_HIGH_ETA,
			CLUSTER_LEFTMOST_PIX_COLS_BPIX,  CLUSTER_LEFTMOST_PIX_COLS_LOW_ETA,  CLUSTER_LEFTMOST_PIX_COLS_HIGH_ETA,
			CLUSTER_RIGHTMOST_PIX_COLS_BPIX, CLUSTER_RIGHTMOST_PIX_COLS_LOW_ETA, CLUSTER_RIGHTMOST_PIX_COLS_HIGH_ETA,
			NUMBER_OF_DISTRIBUTION_COUNTABLES
		};
		enum m_pileupDependenceDistributions
		{
			// Barrel-forward comparison
			NUM_CLUSTERS_BARREL_VS_PILEUP,              NUM_CLUSTERS_FORWARD_VS_PILEUP,
			AVG_CLUSTER_SIZE_X_BARREL_VS_PILEUP,        AVG_CLUSTER_SIZE_X_FORWARD_VS_PILEUP,
			AVG_CLUSTER_SIZE_Y_BARREL_VS_PILEUP,        AVG_CLUSTER_SIZE_Y_FORWARD_VS_PILEUP,
			AVG_CLUSTER_SIZE_PIXELS_BARREL_VS_PILEUP,   AVG_CLUSTER_SIZE_PIXELS_FORWARD_VS_PILEUP,
			// Layer level statistics
			NUM_CLUSTERS_LAYER_1_VS_PILEUP,             NUM_CLUSTERS_LAYER_2_VS_PILEUP,             NUM_CLUSTERS_LAYER_3_VS_PILEUP,             NUM_CLUSTERS_LAYER_4_VS_PILEUP,
			AVG_CLUSTER_SIZE_X_LAYER_1_VS_PILEUP,       AVG_CLUSTER_SIZE_X_LAYER_2_VS_PILEUP,       AVG_CLUSTER_SIZE_X_LAYER_3_VS_PILEUP,       AVG_CLUSTER_SIZE_X_LAYER_4_VS_PILEUP,
			AVG_CLUSTER_SIZE_Y_LAYER_1_VS_PILEUP,       AVG_CLUSTER_SIZE_Y_LAYER_2_VS_PILEUP,       AVG_CLUSTER_SIZE_Y_LAYER_3_VS_PILEUP,       AVG_CLUSTER_SIZE_Y_LAYER_4_VS_PILEUP,
			AVG_CLUSTER_SIZE_PIXELS_LAYER_1_VS_PILEUP,  AVG_CLUSTER_SIZE_PIXELS_LAYER_2_VS_PILEUP,  AVG_CLUSTER_SIZE_PIXELS_LAYER_3_VS_PILEUP,  AVG_CLUSTER_SIZE_PIXELS_LAYER_4_VS_PILEUP,
			// Others
			EVEN_COL_CLUSTER_RATE_TOTAL_VS_PILEUP, EVEN_COL_CLUSTER_RATE_LOW_ETA_VS_PILEUP, EVEN_COL_CLUSTER_RATE_HIGH_ETA_VS_PILEUP,
			// Digi flag related distributions
			MARKED_PAIR_CANDIDATE_RATE_IN_EVENT_VS_PILEUP, MARKED_PAIR_CANDIDATE_RATE_LOW_ETA_VS_PILEUP, MARKED_PAIR_CANDIDATE_RATE_HIGH_ETA_VS_PILEUP, NUMBER_OF_PILEUP_DISTRIBUTION_COUNTABLES
		};
		struct AreClustersPairInfo
		{
			enum class Type { NON_TAGGED, TAGGED };
			AreClustersPairInfo(bool t_value, Type t_type) : m_value(t_value), m_type(t_type) {}
			operator bool() const { return m_value; }
			bool m_value;
			Type m_type;
		};
	public:
		// EDAnalyzer base
		PhaseISplitClusterAnalyzer(const edm::ParameterSet& t_iConfig);
		virtual ~PhaseISplitClusterAnalyzer();
		virtual void beginJob() override final;
		virtual void endJob() override final;
		virtual void analyze(const edm::Event& t_iEvent, const edm::EventSetup& t_iSetup) override final;
		virtual void beginRun(edm::Run const&, edm::EventSetup const&) override final;
		virtual void endRun(edm::Run const&, edm::EventSetup const&) override final;
		virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override final;
		virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override final;
		
		// Logic
		void updateRunNumber();
		void updateLuminosityBlockNumber();
		void updateTimestamp();
		[[deprecated]] void recalculateDetIdToMarkerPtrMapOld();
		void recalculateDetIdToMarkerPtrMap();
		void checkDetIdToMarkerPtrMap() const;
		void dumpClusterDigiAndFlagChannels() const;
		std::vector<std::vector<int>> getPixelMarkerValuesOnDetUnit(const edmNew::DetSet<SiPixelCluster>& t_clustersOnModule, const DetId& t_detId) const;
		template <typename T>
		static T getAllDigiChannelsFromClustersOnModule(const edmNew::DetSet<SiPixelCluster>& t_clustersOnModule);
		template <typename T>
		static T getAllDigiChannelsFromDigisOnModule(const edm::DetSet<PixelDigi>& t_digisOnModule);
		static std::vector<int> getClusterPixelChannels(const SiPixelCluster& t_cluster);
		static int getPixelChannel(const SiPixelCluster::Pixel& t_pixel);
		static void printPixelChannel(const PixelDigi& t_pixel, const std::string& t_delimiter);
		static void printPixelChannelSpaceDelimiter(const PixelDigi& t_pixel);
		template <typename UnaryOperation>
		static void forEachPixelForClustersOnModule(const edmNew::DetSet<SiPixelCluster>::const_iterator& t_begin, const edmNew::DetSet<SiPixelCluster>::const_iterator& t_end, UnaryOperation t_op);
		static int getDigiMarkerValue(const SiPixelCluster::Pixel& pixelToCheck, const edm::DetSet<PixelDigi>& digiFlags);
		static int getDigiMarkerValue(const int t_x, const int t_y, const edm::DetSet<PixelDigi>& t_digiFlags);
		float getPileupInfo() const;
		// ModuleClusterPlots		
		void handleModuleClusterPlots();
		void generateNewModuleClusterPlots();
		void fillModuleClusterPlots();
		void saveFinishedModuleClusterPlots();
		// Distributions
		void handleEventStatisticsForDistributions();
		void fillCummulativeDistributions();
		void fillPerEventAndPileupDependenceDistributions();
		Cluster getClusterDataObject(const SiPixelCluster& t_siPixelCluster, const DetId& t_detId);
		std::vector<std::tuple<Cluster, Cluster, bool>> getClusterPairCandidateCollection(const edmNew::DetSet<SiPixelCluster>& t_siPixelClusterDetSet, const std::vector<std::vector<int>>& t_pixelMarkerValuesOnModule);
		AreClustersPairInfo areClustersPair(const Cluster& t_first, const Cluster& t_second, const DetId& t_detId);
		int clusterMinPixelCol(const Cluster& t_cluster);
		std::vector<std::size_t> clusterMinColPixels(const Cluster& t_cluster);
		std::vector<std::size_t> clusterMaxColPixels(const Cluster& t_cluster);
		std::pair<std::vector<std::size_t>, std::vector<std::size_t>> clusterMinMaxColPixels(const Cluster& t_cluster);

		void generateHistogramCollections();
		void saveCummulativeDistributions();
		void savePerEventDistributions();
		void savePileupDependenceDistributions();

		// Private logic
		void getModuleData(ModuleData& t_mod, const DetId& t_detId);
		std::string executeBashScript(const std::string& t_command);
		std::pair<std::pair<int, int>, float> transformBrilcalcLineToPileupTable(const std::string& t_line);

	private:
		// Config
		edm::ParameterSet m_iConfig;

		// Input-output
		const std::string m_outputFilePath;
		TFile* m_outputFile { nullptr };

		// Data collections
		edm::Handle<edmNew::DetSetVector<SiPixelCluster>> m_clusterCollectionHandle;
		edm::Handle<edm::DetSetVector<PixelDigi>>         m_digiFlagsCollectionHandle;
		edm::Handle<std::vector<PileupSummaryInfo>>       m_puInfoCollectionHandle;
		// Utility data collections
		// Used to hold values for cluster digis that have no corresponding markers
		// Using unique_ptr-s to avoid issues with reallocated data members on new element inertion
		std::vector<std::unique_ptr<edm::DetSet<PixelDigi>>> m_fakeFlagsManager;
		std::map<DetId, const edm::DetSet<PixelDigi>*>       m_detIdToMarkerPtrMap;

		// Tools
		SiPixelCoordinates                    m_siPixelCoordinates;
		const TrackerGeometry*                m_trackerGeometry { nullptr };
		const PixelClusterParameterEstimator* m_pixelClusterParameterEstimator { nullptr };

		// States
		const edm::Event* m_iEvent { nullptr };
		bool m_isEventMC { false };
		int m_runNumber { NOVAL_I };
		int m_luminosityBlock { NOVAL_I };
		int m_eventNumber { NOVAL_I };
		int m_pileup { NOVAL_I };
		// Timestamp-helpers
		bool m_isNewRun { false };
		bool m_isNewLuminosityBlock { false };
		std::tuple<int, int, int> m_moduleClusterPlotsGeneratedAt;
		int m_eventsSinceModuleClusterPlotGeneration { 0 };

		// Module cluster plots
		std::vector<TH2F*> m_moduleClusterPlots;

		// Distributions
		std::vector<TH1F*> m_perEventDistributions;
		std::vector<TH1F*> m_distributions;
		std::vector<TH2F*> m_pileupDependenceDistributions;

		// Tokens
		// edm::EDGetTokenT<edm::DetSetVector<SiPixelRawDataError>> rawDataErrorToken_;
		// edm::EDGetTokenT<reco::VertexCollection>                 primaryVerticesToken_;
		// edm::EDGetTokenT<edm::TriggerResults>                    triggerResultsToken_;
		edm::EDGetTokenT<edmNew::DetSetVector<SiPixelCluster>>   m_clustersToken;
		edm::EDGetTokenT<edm::DetSetVector<PixelDigi>>           m_digiFlagsToken;
		// edm::EDGetTokenT<TrajTrackAssociationCollection>         trajTrackCollectionToken_;
		// edm::EDGetTokenT<MeasurementTrackerEvent>                measurementTrackerEventToken_;
		edm::EDGetTokenT<std::vector<PileupSummaryInfo>>         m_pileupSummaryToken;
		// edm::EDGetTokenT<edm::DetSetVector<PixelDigi>>           pixelDigiCollectionToken_;

		[[noreturn, gnu::cold]] static void e_brilcalc_regex() { throw std::runtime_error("Brilcalc regex matching failed."); }
		[[noreturn, gnu::cold]] static void e_marker_signal_mismatch() { throw std::runtime_error("Marker signal mismatch!"); }
		[[noreturn, gnu::cold]] static void e_marker_detId() { throw std::runtime_error("Check marker detId iteration!"); }
		[[noreturn, gnu::cold]] static void e_marker_not_found() { throw std::runtime_error("Pixel marker not found!"); }

		// The satic assertion somehow messes up the syntax highlighting
		// The braces should be correct here though...
		static_assert(MODULE_CLUSTER_PLOT_EVENTS_TO_MERGE < MODULE_CLUSTER_PLOT_GENERATION_FREQUENCY, "");
		static_assert(MODULE_CLUSTER_PLOT_EVENTS_TO_MERGE != 0, "");
		static_assert(MODULE_CLUSTER_PLOT_GENERATION_FREQUENCY != 0, "");
};

