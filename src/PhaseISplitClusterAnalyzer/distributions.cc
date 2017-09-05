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
