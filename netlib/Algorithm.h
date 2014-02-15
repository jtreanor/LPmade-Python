#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "LinkPredictor/LinkPredictor.h"
#include "WeightedNetwork.h"

class Algorithm {
	private:
		const WeightedNetwork& directedNetwork;
		const WeightedNetwork& undirectedNetwork;
		const WeightedNetwork& reversedNetwork;
		Algorithm();
	public:
		Algorithm( const WeightedNetwork&, const WeightedNetwork&, const WeightedNetwork& );
		~Algorithm();
		enum {COMMON_NEIGHBOURS=0, ADAMIC_ADAR, PROP_FLOW, UNWEIGHTED_PROP_FLOW, GRAPH_DISTANCE, ROOTED_PAGE_RANK, JACCARD, PREFERENTIAL_ATTACHMENT, KATZ_MEASURE, IN_DEGREE, RANDOM, WTF };
		enum {DIRECTED=0, REVERSED, UNDIRECTED };
		const WeightedNetwork& networkForDirection(int) const;
		LinkPredictor* predictorForType(int, int) const;
};

#endif