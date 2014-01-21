#include "RecommenderResult.h"
#include "CommonNeighborLinkPredictor.h"

RecommenderResult::RecommenderResult( const WeightedNetwork& trainingNetwork, const WeightedNetwork& testNetwork, const std::vector<int>& linkPredictors ) : trainingNetwork(trainingNetwork), testNetwork(testNetwork), linkPredictors(linkPredictors) {
}

RecommenderResult::~RecommenderResult() {
}

std::vector<double> RecommenderResult::precisionAtN(int n, int start, int end) {
	std::vector<double> precisionAtN = std::vector<double>( n );

	// int start = 0;
	// int end = this->testNetwork.vertexCount();

	double proportion = 1.0/(end - start);

	CommonNeighborLinkPredictor* predictor = new CommonNeighborLinkPredictor( this->trainingNetwork, this->trainingNetwork );

	std::cout << start << " -> " << end << "\n";

	for (int currentVertex = start; currentVertex < end; currentVertex++ ) {
		vertex_t currentVertexExt = this->testNetwork.translateIntToExt(currentVertex);

		std::vector<vertex_t> acceptedVertices = predictor->topNVerticesExt(currentVertexExt,n);

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