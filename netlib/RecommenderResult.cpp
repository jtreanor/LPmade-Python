#include "RecommenderResult.h"
#include <queue>
#include <tuple>

RecommenderResult::RecommenderResult( const WeightedNetwork& testNetwork, const LinkPredictorEnsemble& ensemble ) : testNetwork(testNetwork), ensemble(ensemble) {
}

RecommenderResult::~RecommenderResult() {
}

double RecommenderResult::evaluate(int n, int start, int end) const {
	double precision = 0.0;
	double proportion = 1.0/(end - start);

	for (int currentVertex = start; currentVertex < end; currentVertex++ ) {
		vertex_t currentVertexExt = this->testNetwork.translateIntToExt(currentVertex);
		std::vector<vertex_t> acceptedVertices = this->ensemble.topNVerticesExt(currentVertexExt,n);

  		double correct_recommendations = 0;
  		int total_correct_answers = this->testNetwork.outDegree( currentVertex );

		for ( vertex_t acceptedVertexExt : acceptedVertices ) {
			if ( this->testNetwork.hasEdgeExt(currentVertexExt, acceptedVertexExt) ) {
  				correct_recommendations++;
  			}
		}

		double denominator = total_correct_answers < n ? total_correct_answers : n;       
        double current_precision = denominator == 0 ? 0 : correct_recommendations/denominator;

        precision += proportion * current_precision;
	}

	return precision;
}