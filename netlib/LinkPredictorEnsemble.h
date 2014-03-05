#ifndef LINKPREDICTORENSEMBLE_H
#define LINKPREDICTORENSEMBLE_H

#include <vector>
#include "WeightedNetwork.h"
#include "AlgorithmManager.h"
#include "LinkPredictor/LinkPredictor.h"

using std::vector;

class LinkPredictorEnsemble {
	private:
		const WeightedNetwork& trainingNetwork;
		const std::vector<double>& weights;
		const AlgorithmManager& alg;
		std::vector<LinkPredictor*> linkPredictors;
	protected:
	public:
		LinkPredictorEnsemble( const WeightedNetwork&, const std::vector<int>&, const std::vector<int>&, const std::vector<double>&, const AlgorithmManager& );
		~LinkPredictorEnsemble();
		std::vector<vertex_t> topNVertices(vertex_t, int);
		std::vector<vertex_t> topNVerticesExt(vertex_t, int);
		std::vector<vertex_t> topNVerticesExtBorda(vertex_t, int);
};

#endif
