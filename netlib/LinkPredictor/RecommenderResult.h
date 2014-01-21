#ifndef RECOMMENDERRESULT_H
#define RECOMMENDERRESULT_H

#include <vector>
#include "../WeightedNetwork.h"
#include "LinkPredictor.h"

using std::vector;

class RecommenderResult {
	private:
		const WeightedNetwork& trainingNetwork;
		const WeightedNetwork& testNetwork;
		const std::vector<int>& linkPredictors;
		// const std::vector<int>& verticesToCheck;
		// int start;
		// int end;
		// vector<double> precisionAtN;
		// double precision;
		// double recall;
	protected:
	public:
		RecommenderResult( const WeightedNetwork&, const WeightedNetwork&, const std::vector<int>& );
		~RecommenderResult();
		std::vector<double> precisionAtN(int, int, int);
		// std::vector<vertex_t> topNVertices(int);
};

#endif
