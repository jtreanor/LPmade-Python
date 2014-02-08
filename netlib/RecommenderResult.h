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
		const WeightedNetwork& testNetwork;
		LinkPredictorEnsemble *ensemble;
	protected:
	public:
		RecommenderResult( const WeightedNetwork&, const WeightedNetwork&, const std::vector<int>&, const std::vector<int>&, const std::vector<double>& );
		~RecommenderResult();
		std::vector<double> precisionAtN(int, int, int);
};

#endif
