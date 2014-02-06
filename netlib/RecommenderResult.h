#ifndef RECOMMENDERRESULT_H
#define RECOMMENDERRESULT_H

#include <vector>
#include "WeightedNetwork.h"
#include "LinkPredictor/LinkPredictor.h"
#include "LinkPredictorEnsemble.h"

using std::vector;

class RecommenderResult {
	private:
		const WeightedNetwork& trainingNetwork;
		const WeightedNetwork& originalTraining;
		const WeightedNetwork& testNetwork;
		const std::vector<int>& algorithms;
		LinkPredictorEnsemble* ensemble;
	protected:
	public:
		RecommenderResult( const WeightedNetwork&, const WeightedNetwork&, const WeightedNetwork&, const std::vector<int>& );
		~RecommenderResult();
		std::vector<double> precisionAtN(int, int, int);
};

#endif
