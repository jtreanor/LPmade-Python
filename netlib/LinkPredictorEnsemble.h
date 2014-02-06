#ifndef LINKPREDICTORENSEMBLE_H
#define LINKPREDICTORENSEMBLE_H

#include <vector>
#include "WeightedNetwork.h"
#include "LinkPredictor/LinkPredictor.h"

using std::vector;

enum Algorithm {COMMON_NEIGHBOURS, ADAMIC_ADAR, PROP_FLOW, GRAPH_DISTANCE, ROOTED_PAGE_RANK, JACCARD, PREFERENTIAL_ATTACHMENT, KATZ_MEASURE, IN_DEGREE, RANDOM, WTF };

class LinkPredictorEnsemble {
	private:
		const WeightedNetwork& trainingNetwork;
		const WeightedNetwork& originalTraining;
		const std::vector<int>& algorithms;
		std::vector<LinkPredictor*> linkPredictors;
		LinkPredictor* predictorForType(Algorithm);
	protected:
	public:
		LinkPredictorEnsemble( const WeightedNetwork&, const WeightedNetwork&, const std::vector<int>& );
		// LinkPredictorEnsemble& operator=( const LinkPredictorEnsemble& );
		~LinkPredictorEnsemble();
		std::vector<vertex_t> topNVertices(vertex_t, int);
		std::vector<vertex_t> topNVerticesExt(vertex_t, int);
};

#endif
