#ifndef RECOMMENDERRESULT_H
#define RECOMMENDERRESULT_H

#include <vector>
#include "WeightedNetwork.h"
#include "LinkPredictor/LinkPredictor.h"
#include "LinkPredictorEnsemble.h"
#include "AlgorithmManager.h"

using std::vector;

class RecommenderResult {
	private:
		const WeightedNetwork& testNetwork;
		const LinkPredictorEnsemble& ensemble;
	protected:
	public:
		RecommenderResult( const WeightedNetwork&, const LinkPredictorEnsemble& );
		~RecommenderResult();
		double evaluate(int, int, int) const;
};

#endif
