#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "LinkPredictor/LinkPredictor.h"
#include "WeightedNetwork.h"

class Algorithm {
	public:
		enum {COMMON_NEIGHBOURS, ADAMIC_ADAR, PROP_FLOW, GRAPH_DISTANCE, ROOTED_PAGE_RANK, JACCARD, PREFERENTIAL_ATTACHMENT, KATZ_MEASURE, IN_DEGREE, RANDOM, WTF };
		enum {DIRECTED, REVERSED, UNDIRECTED };
		static LinkPredictor* predictorForType(int, int, const WeightedNetwork&);
		static const WeightedNetwork& modifiedTrainingNetworkForDirection(int, const WeightedNetwork&);
};

#endif