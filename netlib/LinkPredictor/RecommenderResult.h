#ifndef RECOMMENDERRESULT_H
#define RECOMMENDERRESULT_H

#include <vector>
#include "../WeightedNetwork.h"
#include "LinkPredictor.h"

using std::vector;

enum Recommender {COMMON_NEIGHBOURS, ADAMIC_ADAR, PROP_FLOW, GRAPH_DISTANCE, ROOTED_PAGE_RANK, JACCARD, PREFERENTIAL_ATTACHMENT, KATZ_MEASURE, WTF, TOP, RANDOM };

class RecommenderResult {
	private:
		const WeightedNetwork& trainingNetwork;
		const WeightedNetwork& originalTraining;
		const WeightedNetwork& testNetwork;
		const std::vector<int>& algorithms;
		std::vector<LinkPredictor*> linkPredictors;

		LinkPredictor* predictorForType(Recommender);
		std::vector<vertex_t> acceptedNodesAt(vertex_t, int);
	protected:
	public:
		RecommenderResult( const WeightedNetwork&, const WeightedNetwork&, const WeightedNetwork&, const std::vector<int>& );
		~RecommenderResult();
		std::vector<double> precisionAtN(int, int, int);
		// std::vector<vertex_t> topNVertices(int);
};

#endif
