#ifndef ALGORITHMMANAGER_H
#define ALGORITHMMANAGER_H

#include "LinkPredictor/LinkPredictor.h"
#include "WeightedNetwork.h"

class AlgorithmManager {
	private:
		const WeightedNetwork& directedNetwork;
		WeightedNetwork undirectedNetwork;
		WeightedNetwork reversedNetwork;
		const std::vector<int>& algorithms;
		const std::vector<int>& directions;
		AlgorithmManager();
	public:
		AlgorithmManager( const WeightedNetwork&, const std::vector<int>&, const std::vector<int>& );
		~AlgorithmManager();
		enum {COMMON_NEIGHBOURS=0, ADAMIC_ADAR, PROP_FLOW, UNWEIGHTED_PROP_FLOW, GRAPH_DISTANCE, ROOTED_PAGE_RANK, JACCARD, PREFERENTIAL_ATTACHMENT, KATZ_MEASURE, IN_DEGREE, RANDOM, WTF, SIM_RANK, MUTUALITY, TRIANGLE };
		enum {DIRECTED=0, REVERSED, UNDIRECTED };
		const WeightedNetwork& networkForDirection(int) const;
		LinkPredictor* predictorForType(int, int) const;
		std::vector<LinkPredictor*> linkPredictors() const;
};

#endif