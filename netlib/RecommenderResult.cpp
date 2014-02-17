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

std::vector<double> RecommenderResult::precisionAtN(int n, int start, int end) {
	std::vector<double> precisionAtN = std::vector<double>( n );

	double proportion = 1.0/(end - start);

	for (int currentVertex = start; currentVertex < end; currentVertex++ ) {
		vertex_t currentVertexExt = this->testNetwork.translateIntToExt(currentVertex);
		std::vector<vertex_t> acceptedVertices = this->ensemble->topNVerticesExt(currentVertexExt,n);

		int currentN = 1;
  		double correct_recommendations = 0;
  		int total_correct_answers = this->testNetwork.outDegree( currentVertex );

		for ( vertex_t acceptedVertexExt : acceptedVertices ) {
			if ( this->testNetwork.hasEdgeExt(currentVertexExt, acceptedVertexExt) ) {
  				correct_recommendations++;
  			}
  			
	        double denominator = total_correct_answers < currentN ? total_correct_answers : currentN;       
	        double precision = denominator == 0 ? 0 : correct_recommendations/denominator;

	        precisionAtN.at(currentN-1) += proportion * precision;

			currentN++;
		}
	}

	return precisionAtN;
}