#include "../../interface/PhaseISplitClusterAnalyzer.h"

void PhaseISplitClusterAnalyzer::generateHistogramCollections()
{
	// Per event distributions
	m_perEventDistributions.clear();
	m_perEventDistributions.reserve(NUMBER_OF_EVENT_COUNTABLES);
	m_perEventDistributions.emplace_back("NUM_CLUSTERS_IN_EVENT", "NUM_CLUSTERS_IN_EVENT", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_CLUSTERS_LOW_ETA", "NUM_CLUSTERS_LOW_ETA", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_CLUSTERS_HIGH_ETA", "NUM_CLUSTERS_HIGH_ETA", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_PAIR_CANDIDATES_IN_EVENT", "NUM_PAIR_CANDIDATES_IN_EVENT", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_PAIR_CANDIDATES_LOW_ETA", "NUM_PAIR_CANDIDATES_LOW_ETA", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_PAIR_CANDIDATES_HIGH_ETA", "NUM_PAIR_CANDIDATES_HIGH_ETA", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("EVEN_COL_CANDIDATES_IN_EVENT", "EVEN_COL_CANDIDATES_IN_EVENT", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("EVEN_COL_CANDIDATES_LOW_ETA", "EVEN_COL_CANDIDATES_LOW_ETA", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("EVEN_COL_CANDIDATES_HIGH_ETA", "EVEN_COL_CANDIDATES_HIGH_ETA", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_X_IN_EVENT", "AVG_CLUSTER_SIZE_X_IN_EVENT", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_X_LOW_ETA", "AVG_CLUSTER_SIZE_X_LOW_ETA", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_X_HIGH_ETA", "AVG_CLUSTER_SIZE_X_HIGH_ETA", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_Y_IN_EVENT", "AVG_CLUSTER_SIZE_Y_IN_EVENT", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_Y_LOW_ETA", "AVG_CLUSTER_SIZE_Y_LOW_ETA", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_Y_HIGH_ETA", "AVG_CLUSTER_SIZE_Y_HIGH_ETA", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_PIXELS_IN_EVENT", "AVG_CLUSTER_SIZE_PIXELS_IN_EVENT", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_PIXELS_LOW_ETA", "AVG_CLUSTER_SIZE_PIXELS_LOW_ETA", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_PIXELS_HIGH_ETA", "AVG_CLUSTER_SIZE_PIXELS_HIGH_ETA", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_PAIR_CANDIDATES_IN_BPIX", "NUM_PAIR_CANDIDATES_IN_BPIX", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_PAIR_CANDIDATES_IN_FPIX", "NUM_PAIR_CANDIDATES_IN_FPIX", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("EVEN_COL_CANDIDATES_IN_BPIX", "EVEN_COL_CANDIDATES_IN_BPIX", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("EVEN_COL_CANDIDATES_IN_FPIX", "EVEN_COL_CANDIDATES_IN_FPIX", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_CLUSTERS_LAYER_1", "NUM_CLUSTERS_LAYER_1", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_CLUSTERS_LAYER_2", "NUM_CLUSTERS_LAYER_2", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_CLUSTERS_LAYER_3", "NUM_CLUSTERS_LAYER_3", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_CLUSTERS_LAYER_4", "NUM_CLUSTERS_LAYER_4", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_PAIR_CANDIDATES_LAYER_1", "NUM_PAIR_CANDIDATES_LAYER_1", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_PAIR_CANDIDATES_LAYER_2", "NUM_PAIR_CANDIDATES_LAYER_2", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_PAIR_CANDIDATES_LAYER_3", "NUM_PAIR_CANDIDATES_LAYER_3", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_PAIR_CANDIDATES_LAYER_4", "NUM_PAIR_CANDIDATES_LAYER_4", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("EVEN_COL_CANDIDATES_LAYER_1", "EVEN_COL_CANDIDATES_LAYER_1", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("EVEN_COL_CANDIDATES_LAYER_2", "EVEN_COL_CANDIDATES_LAYER_2", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("EVEN_COL_CANDIDATES_LAYER_3", "EVEN_COL_CANDIDATES_LAYER_3", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("EVEN_COL_CANDIDATES_LAYER_4", "EVEN_COL_CANDIDATES_LAYER_4", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_X_LAYER_1", "AVG_CLUSTER_SIZE_X_LAYER_1", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_X_LAYER_2", "AVG_CLUSTER_SIZE_X_LAYER_2", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_X_LAYER_3", "AVG_CLUSTER_SIZE_X_LAYER_3", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_X_LAYER_4", "AVG_CLUSTER_SIZE_X_LAYER_4", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_Y_LAYER_1", "AVG_CLUSTER_SIZE_Y_LAYER_1", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_Y_LAYER_2", "AVG_CLUSTER_SIZE_Y_LAYER_2", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_Y_LAYER_3", "AVG_CLUSTER_SIZE_Y_LAYER_3", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_Y_LAYER_4", "AVG_CLUSTER_SIZE_Y_LAYER_4", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_PIXELS_LAYER_1", "AVG_CLUSTER_SIZE_PIXELS_LAYER_1", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_PIXELS_LAYER_2", "AVG_CLUSTER_SIZE_PIXELS_LAYER_2", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_PIXELS_LAYER_3", "AVG_CLUSTER_SIZE_PIXELS_LAYER_3", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_PIXELS_LAYER_4", "AVG_CLUSTER_SIZE_PIXELS_LAYER_4", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.shrink_to_fit();
	
	// General distributions
	m_distributions.clear();
	m_distributions.emplace_back("CLUSTER_SIZE_X_TOTAL", "CLUSTER_SIZE_X_TOTAL", 1000, 0.0f, 100000000.0f);
	m_distributions.emplace_back("CLUSTER_SIZE_X_LOW_ETA", "CLUSTER_SIZE_X_LOW_ETA", 1000, 0.0f, 100000000.0f);
	m_distributions.emplace_back("CLUSTER_SIZE_X_HIGH_ETA", "CLUSTER_SIZE_X_HIGH_ETA", 1000, 0.0f, 100000000.0f);
	m_distributions.emplace_back("CLUSTER_SIZE_Y_TOTAL", "CLUSTER_SIZE_Y_TOTAL", 1000, 0.0f, 100000000.0f);
	m_distributions.emplace_back("CLUSTER_SIZE_Y_LOW_ETA", "CLUSTER_SIZE_Y_LOW_ETA", 1000, 0.0f, 100000000.0f);
	m_distributions.emplace_back("CLUSTER_SIZE_Y_HIGH_ETA", "CLUSTER_SIZE_Y_HIGH_ETA", 1000, 0.0f, 100000000.0f);
	m_distributions.emplace_back("CLUSTER_SIZE_PIX_TOTAL", "CLUSTER_SIZE_PIX_TOTAL", 1000, 0.0f, 100000000.0f);
	m_distributions.emplace_back("CLUSTER_SIZE_PIX_LOW_ETA", "CLUSTER_SIZE_PIX_LOW_ETA", 1000, 0.0f, 100000000.0f);
	m_distributions.emplace_back("CLUSTER_SIZE_PIX_HIGH_ETA", "CLUSTER_SIZE_PIX_HIGH_ETA", 1000, 0.0f, 100000000.0f);
	m_distributions.shrink_to_fit();
}

void PhaseISplitClusterAnalyzer::handleEventStatisticsForDistributions()
{
	fillPerEventDistributions();
}

void PhaseISplitClusterAnalyzer::fillPerEventDistributions()
{
	int numClustersInEvent          = 0;
	int numClustersLowEta           = 0;
	int numClustersHighEta          = 0;
	int numPairCandidatesInEvent    = 0;
	int numPairCandidatesLowEta     = 0;
	int numPairCandidatesHighEta    = 0;
	int evenColCandidatesInEvent    = 0;
	int evenColCandidatesLowEta     = 0;
	int evenColCandidatesHighEta    = 0;
	int avgClusterSizeXInEvent      = 0;
	int avgClusterSizeXLowEta       = 0;
	int avgClusterSizeXHighEta      = 0;
	int avgClusterSizeYInEvent      = 0;
	int avgClusterSizeYLowEta       = 0;
	int avgClusterSizeYHighEta      = 0;
	int avgClusterSizePixelsInEvent = 0;
	int avgClusterSizePixelsLowEta  = 0;
	int avgClusterSizePixelsHighEta = 0;
	int numPairCandidatesInBpix     = 0;
	int numPairCandidatesInFpix     = 0;
	int evenColCandidatesInBpix     = 0;
	int evenColCandidatesInFpix     = 0;
	int numClustersLayer1           = 0;
	int numClustersLayer2           = 0;
	int numClustersLayer3           = 0;
	int numClustersLayer4           = 0;
	int numPairCandidatesLayer1     = 0;
	int numPairCandidatesLayer2     = 0;
	int numPairCandidatesLayer3     = 0;
	int numPairCandidatesLayer4     = 0;
	int evenColCandidatesLayer1     = 0;
	int evenColCandidatesLayer2     = 0;
	int evenColCandidatesLayer3     = 0;
	int evenColCandidatesLayer4     = 0;
	int avgClusterSizeXLayer1       = 0;
	int avgClusterSizeXLayer2       = 0;
	int avgClusterSizeXLayer3       = 0;
	int avgClusterSizeXLayer4       = 0;
	int avgClusterSizeYLayer1       = 0;
	int avgClusterSizeYLayer2       = 0;
	int avgClusterSizeYLayer3       = 0;
	int avgClusterSizeYLayer4       = 0;
	int avgClusterSizePixelsLayer1  = 0;
	int avgClusterSizePixelsLayer2  = 0;
	int avgClusterSizePixelsLayer3  = 0;
	int avgClusterSizePixelsLayer4  = 0;
	std::for_each(m_clusterCollectionHandle -> begin(), m_clusterCollectionHandle -> end(), [&] (const auto& clusterCollectionOnModule)
	{
		numClustersInEvent += clusterCollectionOnModule.size();
		for(const auto& cluster: clusterCollectionOnModule)
		{
			DetId detId(clusterCollectionOnModule.id());
			ModuleData mod_on;
			this -> getModuleData(mod_on, detId);
			avgClusterSizeXInEvent += cluster.sizeX();
			avgClusterSizeYInEvent += cluster.sizeY();
			avgClusterSizePixelsInEvent += cluster.size();
			if(mod_on.det == 0)
			{
				if(std::abs(mod_on.module) == 1)
				{
					numClustersLowEta++;
					avgClusterSizeXLowEta += cluster.sizeX();
					avgClusterSizeYLowEta += cluster.sizeY();
					avgClusterSizePixelsLowEta += cluster.size();
				}
				if(std::abs(mod_on.module) == 4)
				{
					numClustersHighEta++;
					avgClusterSizeXHighEta += cluster.sizeX();
					avgClusterSizeYHighEta += cluster.sizeY();
					avgClusterSizePixelsHighEta += cluster.size();
				}
				if(mod_on.layer == 1)
				{
					numClustersLayer1++;
					avgClusterSizeXLayer1      += cluster.sizeX();
					avgClusterSizeYLayer1      += cluster.sizeY();
					avgClusterSizePixelsLayer1 += cluster.size();
				}
				if(mod_on.layer == 2)
				{
					numClustersLayer2++;
					avgClusterSizeXLayer2      += cluster.sizeX();
					avgClusterSizeYLayer2      += cluster.sizeY();
					avgClusterSizePixelsLayer2 += cluster.size();
				}
				if(mod_on.layer == 3)
				{
					numClustersLayer3++;
					avgClusterSizeXLayer3      += cluster.sizeX();
					avgClusterSizeYLayer3      += cluster.sizeY();
					avgClusterSizePixelsLayer3 += cluster.size();
				}
				if(mod_on.layer == 4)
				{
					numClustersLayer4++;
					avgClusterSizeXLayer4      += cluster.sizeX();
					avgClusterSizeYLayer4      += cluster.sizeY();
					avgClusterSizePixelsLayer4 += cluster.size();
				}
			}
			// Pair statistics
			std::vector<std::pair<Cluster, Cluster>> clusterPairCandidateCollection(this -> getClusterPairCandidateCollection(clusterCollectionOnModule));
			numPairCandidatesInEvent += clusterPairCandidateCollection.size();
			std::for_each(clusterPairCandidateCollection.begin(), clusterPairCandidateCollection.end(), [&] (const auto& pair)
			{
				int pairStartPixelCol = std::min(this -> clusterMinPixelCol(pair.first), this -> clusterMinPixelCol(pair.second));
				int pairStartsOnEvenCol = pairStartPixelCol % 2 == 0;
				evenColCandidatesInEvent += pairStartsOnEvenCol;
				if(mod_on.det == 0)
				{
					numPairCandidatesInBpix++;
					evenColCandidatesInBpix += pairStartsOnEvenCol;
					if(std::abs(mod_on.module) == 1)
					{
						numPairCandidatesLowEta++; 
						evenColCandidatesLowEta += pairStartsOnEvenCol;

					}
					if(std::abs(mod_on.module) == 4)
					{
						numPairCandidatesHighEta++;
						evenColCandidatesHighEta += pairStartsOnEvenCol;
					}
					if(mod_on.layer == 1)
					{
						numPairCandidatesLayer1++;
						evenColCandidatesLayer1 += pairStartsOnEvenCol;
					}
					if(mod_on.layer == 2)
					{
						numPairCandidatesLayer2++;
						evenColCandidatesLayer2 += pairStartsOnEvenCol;
					}
					if(mod_on.layer == 3)
					{
						numPairCandidatesLayer3++;
						evenColCandidatesLayer3 += pairStartsOnEvenCol;
					}
					if(mod_on.layer == 4)
					{
						numPairCandidatesLayer4++;
						evenColCandidatesLayer4 += pairStartsOnEvenCol;
					}
				}
				if(mod_on.det == 1)
				{
					numPairCandidatesInFpix++;
					evenColCandidatesInFpix += pairStartsOnEvenCol;
				}
			});
		}
	});
	avgClusterSizeXInEvent      = avgClusterSizeXInEvent      / numClustersInEvent;
	avgClusterSizeYInEvent      = avgClusterSizeYInEvent      / numClustersInEvent;
	avgClusterSizePixelsInEvent = avgClusterSizePixelsInEvent / numClustersInEvent;
	avgClusterSizeXLayer1       = avgClusterSizeXLayer1       / numClustersLayer1;
	avgClusterSizeYLayer1       = avgClusterSizeYLayer1       / numClustersLayer1;
	avgClusterSizePixelsLayer1  = avgClusterSizePixelsLayer1  / numClustersLayer1;
	avgClusterSizeXLayer2       = avgClusterSizeXLayer2       / numClustersLayer2;
	avgClusterSizeYLayer2       = avgClusterSizeYLayer2       / numClustersLayer2;
	avgClusterSizePixelsLayer2  = avgClusterSizePixelsLayer2  / numClustersLayer2;
	avgClusterSizeXLayer3       = avgClusterSizeXLayer3       / numClustersLayer3;
	avgClusterSizeYLayer3       = avgClusterSizeYLayer3       / numClustersLayer3;
	avgClusterSizePixelsLayer3  = avgClusterSizePixelsLayer3  / numClustersLayer3;
	avgClusterSizeXLayer4       = avgClusterSizeXLayer4       / numClustersLayer4;
	avgClusterSizeYLayer4       = avgClusterSizeYLayer4       / numClustersLayer4;
	avgClusterSizePixelsLayer4  = avgClusterSizePixelsLayer4  / numClustersLayer4;

	m_perEventDistributions[NUM_CLUSTERS_IN_EVENT].Fill(numClustersInEvent);
	m_perEventDistributions[NUM_CLUSTERS_LOW_ETA].Fill(numClustersLowEta);
	m_perEventDistributions[NUM_CLUSTERS_HIGH_ETA].Fill(numClustersHighEta);
	m_perEventDistributions[NUM_PAIR_CANDIDATES_IN_EVENT].Fill(numPairCandidatesInEvent);
	m_perEventDistributions[NUM_PAIR_CANDIDATES_LOW_ETA].Fill(numPairCandidatesLowEta);
	m_perEventDistributions[NUM_PAIR_CANDIDATES_HIGH_ETA].Fill(numPairCandidatesHighEta);
	m_perEventDistributions[EVEN_COL_CANDIDATES_IN_EVENT].Fill(evenColCandidatesInEvent);
	m_perEventDistributions[EVEN_COL_CANDIDATES_LOW_ETA].Fill(evenColCandidatesLowEta);
	m_perEventDistributions[EVEN_COL_CANDIDATES_HIGH_ETA].Fill(evenColCandidatesHighEta);
	m_perEventDistributions[AVG_CLUSTER_SIZE_X_IN_EVENT].Fill(avgClusterSizeXInEvent);
	m_perEventDistributions[AVG_CLUSTER_SIZE_X_LOW_ETA].Fill(avgClusterSizeXLowEta);
	m_perEventDistributions[AVG_CLUSTER_SIZE_X_HIGH_ETA].Fill(avgClusterSizeXHighEta);
	m_perEventDistributions[AVG_CLUSTER_SIZE_Y_IN_EVENT].Fill(avgClusterSizeYInEvent);
	m_perEventDistributions[AVG_CLUSTER_SIZE_Y_LOW_ETA].Fill(avgClusterSizeYLowEta);
	m_perEventDistributions[AVG_CLUSTER_SIZE_Y_HIGH_ETA].Fill(avgClusterSizeYHighEta);
	m_perEventDistributions[AVG_CLUSTER_SIZE_PIXELS_IN_EVENT].Fill(avgClusterSizePixelsInEvent);
	m_perEventDistributions[AVG_CLUSTER_SIZE_PIXELS_LOW_ETA].Fill(avgClusterSizePixelsLowEta);
	m_perEventDistributions[AVG_CLUSTER_SIZE_PIXELS_HIGH_ETA].Fill(avgClusterSizePixelsHighEta);
	m_perEventDistributions[NUM_PAIR_CANDIDATES_IN_BPIX].Fill(numPairCandidatesInBpix);
	m_perEventDistributions[NUM_PAIR_CANDIDATES_IN_FPIX].Fill(numPairCandidatesInFpix);
	m_perEventDistributions[EVEN_COL_CANDIDATES_IN_BPIX].Fill(evenColCandidatesInBpix);
	m_perEventDistributions[EVEN_COL_CANDIDATES_IN_FPIX].Fill(evenColCandidatesInFpix);
	m_perEventDistributions[NUM_CLUSTERS_LAYER_1].Fill(numClustersLayer1);
	m_perEventDistributions[NUM_CLUSTERS_LAYER_2].Fill(numClustersLayer2);
	m_perEventDistributions[NUM_CLUSTERS_LAYER_3].Fill(numClustersLayer3);
	m_perEventDistributions[NUM_CLUSTERS_LAYER_4].Fill(numClustersLayer4);
	m_perEventDistributions[NUM_PAIR_CANDIDATES_LAYER_1].Fill(numPairCandidatesLayer1);
	m_perEventDistributions[NUM_PAIR_CANDIDATES_LAYER_2].Fill(numPairCandidatesLayer2);
	m_perEventDistributions[NUM_PAIR_CANDIDATES_LAYER_3].Fill(numPairCandidatesLayer3);
	m_perEventDistributions[NUM_PAIR_CANDIDATES_LAYER_4].Fill(numPairCandidatesLayer4);
	m_perEventDistributions[EVEN_COL_CANDIDATES_LAYER_1].Fill(evenColCandidatesLayer1);
	m_perEventDistributions[EVEN_COL_CANDIDATES_LAYER_2].Fill(evenColCandidatesLayer2);
	m_perEventDistributions[EVEN_COL_CANDIDATES_LAYER_3].Fill(evenColCandidatesLayer3);
	m_perEventDistributions[EVEN_COL_CANDIDATES_LAYER_4].Fill(evenColCandidatesLayer4);
	m_perEventDistributions[AVG_CLUSTER_SIZE_X_LAYER_1].Fill(avgClusterSizeXLayer1);
	m_perEventDistributions[AVG_CLUSTER_SIZE_X_LAYER_2].Fill(avgClusterSizeXLayer2);
	m_perEventDistributions[AVG_CLUSTER_SIZE_X_LAYER_3].Fill(avgClusterSizeXLayer3);
	m_perEventDistributions[AVG_CLUSTER_SIZE_X_LAYER_4].Fill(avgClusterSizeXLayer4);
	m_perEventDistributions[AVG_CLUSTER_SIZE_Y_LAYER_1].Fill(avgClusterSizeYLayer1);
	m_perEventDistributions[AVG_CLUSTER_SIZE_Y_LAYER_2].Fill(avgClusterSizeYLayer2);
	m_perEventDistributions[AVG_CLUSTER_SIZE_Y_LAYER_3].Fill(avgClusterSizeYLayer3);
	m_perEventDistributions[AVG_CLUSTER_SIZE_Y_LAYER_4].Fill(avgClusterSizeYLayer4);
	m_perEventDistributions[AVG_CLUSTER_SIZE_PIXELS_LAYER_1].Fill(avgClusterSizePixelsLayer1);
	m_perEventDistributions[AVG_CLUSTER_SIZE_PIXELS_LAYER_2].Fill(avgClusterSizePixelsLayer2);
	m_perEventDistributions[AVG_CLUSTER_SIZE_PIXELS_LAYER_3].Fill(avgClusterSizePixelsLayer3);
	m_perEventDistributions[AVG_CLUSTER_SIZE_PIXELS_LAYER_4].Fill(avgClusterSizePixelsLayer4);
}

Cluster PhaseISplitClusterAnalyzer::getClusterDataObject(const SiPixelCluster& t_siPixelCluster, const DetId& t_detId)
{
	LocalPoint clustLocalCoordinates;
	const GeomDetUnit* geomDetUnit = m_trackerGeometry -> idToDetUnit(t_detId);
	std::tie(clustLocalCoordinates, std::ignore, std::ignore) = m_pixelClusterParameterEstimator -> getParameters(t_siPixelCluster, *geomDetUnit);
	GlobalPoint clustGlobalCoordinates = geomDetUnit -> toGlobal(clustLocalCoordinates);
	Cluster clusterData;
	getModuleData(clusterData.mod_on, t_detId);
	clusterData.x     = t_siPixelCluster.x();
	clusterData.y     = t_siPixelCluster.y();
	clusterData.lx    = clustLocalCoordinates.x();
	clusterData.ly    = clustLocalCoordinates.y();
	clusterData.glx   = clustGlobalCoordinates.x();
	clusterData.gly   = clustGlobalCoordinates.y();
	clusterData.glz   = clustGlobalCoordinates.z();
	clusterData.sizeX = t_siPixelCluster.sizeX();
	clusterData.sizeY = t_siPixelCluster.sizeY();
	clusterData.size  = t_siPixelCluster.size();
	// Charge
	clusterData.charge = t_siPixelCluster.charge();
	// Misc.
	const auto& currentPixels = t_siPixelCluster.pixels();
	for(int i = 0; i < clusterData.size && i < 1000; ++i)
	{
		clusterData.adc[i]    = t_siPixelCluster.pixelADC()[i] / 1000.0;
		clusterData.pix[i][0] = currentPixels[i].x;
		clusterData.pix[i][1] = currentPixels[i].y;
	}
	return clusterData;
}

std::vector<std::pair<Cluster, Cluster>> PhaseISplitClusterAnalyzer::getClusterPairCandidateCollection(const edmNew::DetSet<SiPixelCluster>& t_siPixelClusterDetSet)
{
	DetId detId = t_siPixelClusterDetSet.detId();
	std::vector<Cluster> clusterDataCollection;
	std::for_each(t_siPixelClusterDetSet.begin(), t_siPixelClusterDetSet.end(), [&] (const auto& e)
	{
		return clusterDataCollection.emplace_back(this -> getClusterDataObject(e, detId));
	});
	std::vector<std::pair<Cluster, Cluster>> clusterPairCollection;
	for(auto firstClusterIt = clusterDataCollection.begin(); firstClusterIt != clusterDataCollection.end(); ++firstClusterIt)
	{
		for(auto secondClusterIt = firstClusterIt + 1; secondClusterIt != clusterDataCollection.end(); ++secondClusterIt)
		{
			if(!areClustersPair(*firstClusterIt, *secondClusterIt)) continue;
			clusterPairCollection.emplace_back(*firstClusterIt, *secondClusterIt);
		}
	}
	return clusterPairCollection;
}

int PhaseISplitClusterAnalyzer::areClustersPair(const Cluster& t_first, const Cluster& t_second)
{
	if(std::abs(t_first.y - t_second.y > 5)) return false;
	int firstMinX;
	int firstMaxX;
	int secondMinX;	
	int secondMaxX;
	std::tie(firstMinX,  firstMaxX ) = clusterMinMaxPixelCol(t_first);
	std::tie(secondMinX, secondMaxX) = clusterMinMaxPixelCol(t_second);
	if(firstMaxX  + 3 == secondMaxX) return true;
	if(secondMaxX + 3 == firstMaxX ) return true;
	return false;
}

int PhaseISplitClusterAnalyzer::clusterMinPixelCol(const Cluster& t_cluster)
{
	assert(0 < t_cluster.size);
	int minX = t_cluster.pix[0][0];
	for(std::size_t pixelIndex = 1, maxIndex = t_cluster.size; pixelIndex < maxIndex; ++pixelIndex)
	{
		const int x = t_cluster.pix[pixelIndex][0] + 0.5;
		minX = std::min(minX, x);
	}
	return minX;
}

std::pair<int, int> PhaseISplitClusterAnalyzer::clusterMinMaxPixelCol(const Cluster& t_cluster)
{
	assert(0 < t_cluster.size);
	int minX = t_cluster.pix[0][0];
	int maxX = t_cluster.pix[0][0];
	for(std::size_t pixelIndex = 1, maxIndex = t_cluster.size; pixelIndex < maxIndex; ++pixelIndex)
	{
		const int x = t_cluster.pix[pixelIndex][0] + 0.5;
		minX = std::min(minX, x);
		maxX = std::max(maxX, x);
	}
	return std::make_pair(minX, maxX);
}

void PhaseISplitClusterAnalyzer::savePerEventDistributions()
{
	m_outputFile -> mkdir("PerEventDistributions");
	const std::string timeStampString   = "RUN_" + std::to_string(std::get<0>(m_moduleClusterPlotsGeneratedAt)) + "/LumiBlock_" + std::to_string(std::get<1>(m_moduleClusterPlotsGeneratedAt)) + "/Event_" + std::to_string(std::get<2>(m_moduleClusterPlotsGeneratedAt));
	const std::string saveDirectoryName = "PerEventDistributions/" + timeStampString;
	m_outputFile -> mkdir(saveDirectoryName.c_str());
	m_outputFile -> cd(saveDirectoryName.c_str());
	std::for_each(m_perEventDistributions.begin(), m_perEventDistributions.end(), [&] (auto& e)
	{
		e.SetDirectory(m_outputFile -> GetDirectory(saveDirectoryName.c_str()));
		e.Write();
	});
	m_outputFile -> cd();
}

void PhaseISplitClusterAnalyzer::saveDataStatisticsDistributions()
{
	m_outputFile -> mkdir("PerEventDistributions");
	const std::string timeStampString   = "RUN_" + std::to_string(std::get<0>(m_moduleClusterPlotsGeneratedAt)) + "/LumiBlock_" + std::to_string(std::get<1>(m_moduleClusterPlotsGeneratedAt)) + "/Event_" + std::to_string(std::get<2>(m_moduleClusterPlotsGeneratedAt));
	const std::string saveDirectoryName = "PerEventDistributions/" + timeStampString;
	m_outputFile -> mkdir(saveDirectoryName.c_str());
	m_outputFile -> cd(saveDirectoryName.c_str());
	std::for_each(m_distributions.begin(), m_distributions.end(), [&] (auto& e)
	{
		e.SetDirectory(m_outputFile -> GetDirectory(saveDirectoryName.c_str()));
		e.Write();
	});
	m_outputFile -> cd();
}
