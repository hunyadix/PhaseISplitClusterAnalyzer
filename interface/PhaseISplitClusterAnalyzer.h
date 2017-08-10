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

// SiPixelCoordinates: Class for plotting Phase 0/1 Geometry
#include "DQM/SiPixelPhase1Common/interface/SiPixelCoordinates.h"

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
#include <vector>
#include <map>

// Compiler directives
#define EDM_ML_LOGDEBUG
#define ML_DEBUG

class PhaseISplitClusterAnalyzer: public edm::EDAnalyzer
{
	public:
		// EDAnalyzer base
		PhaseISplitClusterAnalyzer(const edm::ParameterSet& t_iConfig);
		virtual ~PhaseISplitClusterAnalyzer();
		virtual void beginJob() override final;
		virtual void endJob() override final;
		virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) override final;
		virtual void beginRun(edm::Run const&, edm::EventSetup const&) override final;
		virtual void endRun(edm::Run const&, edm::EventSetup const&) override final;
		virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override final;
		virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override final;
		
		// Logic

	private:
		edm::ParameterSet m_iConfig;

		// Tokens
		edm::EDGetTokenT<edm::DetSetVector<SiPixelRawDataError>> rawDataErrorToken_;
		edm::EDGetTokenT<reco::VertexCollection>                 primaryVerticesToken_;
		edm::EDGetTokenT<edm::TriggerResults>                    triggerResultsToken_;
		edm::EDGetTokenT<edmNew::DetSetVector<SiPixelCluster>>   clustersToken_;
		edm::EDGetTokenT<TrajTrackAssociationCollection>         trajTrackCollectionToken_;
		edm::EDGetTokenT<MeasurementTrackerEvent>                measurementTrackerEventToken_;
		edm::EDGetTokenT<std::vector<PileupSummaryInfo>>         pileupSummaryToken_;
		edm::EDGetTokenT<edm::DetSetVector<PixelDigi>>           pixelDigiCollectionToken_;
};
