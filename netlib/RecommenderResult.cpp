#include "RecommenderResult.h"
#include <queue>
#include <tuple>

RecommenderResult::RecommenderResult( const WeightedNetwork& trainingNetwork, const WeightedNetwork& testNetwork, const std::vector<int>& algorithms, const std::vector<int>& directions, const std::vector<double>& weights, const AlgorithmManager& alg ) : trainingNetwork(trainingNetwork), testNetwork(testNetwork) {
    srand(time(NULL));
	this->ensemble = new LinkPredictorEnsemble(this->trainingNetwork, algorithms, directions, weights, alg);
}

RecommenderResult::~RecommenderResult() {
	delete ensemble;
}

void RecommenderResult::evaluate(int n, int start, int end) {
	this->precision = 0;

	double proportion = 1.0/(end - start);

	for (int currentVertex = start; currentVertex < end; currentVertex++ ) {
		vertex_t currentVertexExt = this->testNetwork.translateIntToExt(currentVertex);
		std::vector<vertex_t> acceptedVertices = this->ensemble->topNVerticesExt(currentVertexExt,n);

  		double correct_recommendations = 0;
  		int total_correct_answers = this->testNetwork.outDegree( currentVertex );

		for ( vertex_t acceptedVertexExt : acceptedVertices ) {
			if ( this->testNetwork.hasEdgeExt(currentVertexExt, acceptedVertexExt) ) {
  				correct_recommendations++;
  			}
		}

		double denominator = total_correct_answers < n ? total_correct_answers : n;       
        double precision = denominator == 0 ? 0 : correct_recommendations/denominator;

        precision += proportion * precision;
	}
}