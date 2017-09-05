#include "../../interface/PhaseISplitClusterAnalyzer.h"

void PhaseISplitClusterAnalyzer::generateNewModuleClusterPlots()
{
	std::for_each(m_moduleClusterPlots.begin(), m_moduleClusterPlots.end(), [] (auto& e) { e.Delete(""); });
	m_moduleClusterPlots.clear();
	for(int type = LOW_ETA; type != NUM_MODULE_CLUSTER_PLOT_TYPES; ++type)
	{
		std::string histogramName  = "Clusters_" + moduleClusterPlotTypeDetails[type].name + "_RUN:" + std::to_string(m_runNumber) + "_LS:" + std::to_string(m_luminosityBlock) + "_EVT:" + std::to_string(m_eventNumber);
		std::string histogramTitle = "Clusters in event merged for " + std::to_string(MODULE_CLUSTER_PLOT_EVENTS_TO_MERGE) + "events, " + moduleClusterPlotTypeDetails[type].name + " " + std::to_string(m_runNumber) + "/" + std::to_string(m_luminosityBlock) + "/" + std::to_string(m_eventNumber) + ";y (pixels);x (pixels);";
		m_moduleClusterPlots.emplace_back(histogramName.c_str(), histogramTitle.c_str(), 416, 0, 416, 160, 0, 160);
	}
	m_moduleClusterPlotsGeneratedAt = std::make_tuple(m_runNumber, m_luminosityBlock, m_eventNumber);
}

void PhaseISplitClusterAnalyzer::handleModuleClusterPlots()
{
	m_eventsSinceModuleClusterPlotGeneration++;
	if(m_isNewRun || m_isNewLuminosityBlock || m_eventsSinceModuleClusterPlotGeneration == MODULE_CLUSTER_PLOT_GENERATION_FREQUENCY)
	{
		generateNewModuleClusterPlots();
		m_eventsSinceModuleClusterPlotGeneration = 0;
	}
	// Fill the histograms with the event content with the set frequency
	if(m_eventsSinceModuleClusterPlotGeneration <= MODULE_CLUSTER_PLOT_EVENTS_TO_MERGE)
	{
		fillModuleClusterPlots();
	}
	if(m_eventsSinceModuleClusterPlotGeneration == MODULE_CLUSTER_PLOT_EVENTS_TO_MERGE)
	{
		saveFinishedModuleClusterPlots();
	}
}

void PhaseISplitClusterAnalyzer::fillModuleClusterPlots()
{
	for(const auto& currentClusterSet: *m_clusterCollectionHandle)
	{
		DetId detId(currentClusterSet.id());
		ModuleData mod_on;
		getModuleData(mod_on, detId);	
		for(int type = LOW_ETA; type != NUM_MODULE_CLUSTER_PLOT_TYPES; ++type)
		{
			if(mod_on.layer != moduleClusterPlotTypeDetails[type].layer ||
				mod_on.ladder != moduleClusterPlotTypeDetails[type].ladder ||
				mod_on.module != moduleClusterPlotTypeDetails[type].module) continue;
			std::for_each(currentClusterSet.begin(), currentClusterSet.end(), [&] (const auto& cluster)
			{
				const auto& pixels = cluster.pixels();
				std::for_each(pixels.begin(), pixels.end(), [&] (const auto& pixel)
				{
					m_moduleClusterPlots[type].Fill(pixel.y, pixel.x);
					// if(pixel.x < 0 || 160 < pixel.x || pixel.y < 0 || 416 < pixel.y)
					// {
					// 	std::cout << "Did no pass boundary tests." << std::endl;
					// }
				});
			});
		}
	}
}

void PhaseISplitClusterAnalyzer::saveFinishedModuleClusterPlots()
{
	m_outputFile -> mkdir("ModuleClusterPlots");
	const std::string timeStampString   = "RUN_" + std::to_string(std::get<0>(m_moduleClusterPlotsGeneratedAt)) + "/LumiBlock_" + std::to_string(std::get<1>(m_moduleClusterPlotsGeneratedAt)) + "/Event_" + std::to_string(std::get<2>(m_moduleClusterPlotsGeneratedAt));
	const std::string saveDirectoryName = "ModuleClusterPlots/" + timeStampString;
	m_outputFile -> mkdir(saveDirectoryName.c_str());
	m_outputFile -> cd(saveDirectoryName.c_str());
	std::for_each(m_moduleClusterPlots.begin(), m_moduleClusterPlots.end(), [&] (auto& e)
	{
		e.SetDirectory(m_outputFile -> GetDirectory(saveDirectoryName.c_str()));
		e.Write();
	});
	m_outputFile -> cd();
}
