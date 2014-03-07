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
		const std::vector<int>& degrees;
		std::vector<LinkPredictor*> linkPredictors;
		int ensembleType;
	protected:
		std::vector<vertex_t> topNVerticesExtMean(vertex_t, int) const;
		std::vector<vertex_t> topNVerticesExtBorda(vertex_t, int) const;
		std::vector<vertex_t> topNVerticesExtLR(vertex_t, int) const;
	public:
		enum {MEAN=0, BORDA, LOGISTIC_REGRESSION };
		LinkPredictorEnsemble( const WeightedNetwork&, const std::vector<double>&, const std::vector<int>&, int, const AlgorithmManager& );
		~LinkPredictorEnsemble();
		std::vector<vertex_t> topNVerticesExt(vertex_t, int) const;
};

#endif
