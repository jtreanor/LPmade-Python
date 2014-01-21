#include "RecommenderResult.h"
#include "AdamicAdarLinkPredictor.h"
#include "CommonNeighborLinkPredictor.h"
#include "DistanceLinkPredictor.h"
#include "JaccardCoefficientLinkPredictor.h"
#include "KatzLinkPredictor.h"
#include "LinkPredictor.h"
#include "OneLinkPredictor.h"
#include "PreferentialAttachmentLinkPredictor.h"
#include "PropFlowLinkPredictor.h"
#include "RootedPageRankLinkPredictor.h"

LinkPredictor* RecommenderResult::predictorForType(Recommender recommender) {
	switch (recommender) {
		case COMMON_NEIGHBOURS:
			return new CommonNeighborLinkPredictor(this->trainingNetwork, this->originalTraining);
		case ADAMIC_ADAR:
			return new AdamicAdarLinkPredictor(this->trainingNetwork, this->originalTraining);
		case PROP_FLOW:
			return new PropFlowLinkPredictor(this->trainingNetwork, this->originalTraining, 5 );
		case GRAPH_DISTANCE:
			return new DistanceLinkPredictor(this->trainingNetwork, this->originalTraining, 5);
		case ROOTED_PAGE_RANK:
			return new RootedPageRankLinkPredictor(this->trainingNetwork, this->originalTraining, 0.15 );
		case JACCARD:
			return new JaccardCoefficientLinkPredictor(this->trainingNetwork, this->originalTraining);
		case PREFERENTIAL_ATTACHMENT:
			return new PreferentialAttachmentLinkPredictor(this->trainingNetwork, this->originalTraining);
		case KATZ_MEASURE:
			return new KatzLinkPredictor(this->trainingNetwork, this->originalTraining, 5, 0.005 );
		case RANDOM:
			return new OneLinkPredictor(this->trainingNetwork, this->originalTraining);
	}
	return NULL;
}

RecommenderResult::RecommenderResult( const WeightedNetwork& trainingNetwork, const WeightedNetwork& originalTraining, const WeightedNetwork& testNetwork, const std::vector<int>& algorithms ) : trainingNetwork(trainingNetwork), originalTraining(originalTraining),testNetwork(testNetwork), algorithms(algorithms) {
	this->linkPredictors = std::vector<LinkPredictor*>();
	for (int algorithm : this->algorithms) {
		Recommender rec = (Recommender) algorithm;
		this->linkPredictors.push_back( this->predictorForType(rec) );
	}
}

RecommenderResult::~RecommenderResult() {
}

std::vector<double> RecommenderResult::precisionAtN(int n, int start, int end) {
	std::vector<double> precisionAtN = std::vector<double>( n );

	double proportion = 1.0/(end - start);

	for (int currentVertex = start; currentVertex < end; currentVertex++ ) {
		vertex_t currentVertexExt = this->testNetwork.translateIntToExt(currentVertex);

		std::vector<vertex_t> acceptedVertices = linkPredictors.at(0)->topNVerticesExt(currentVertexExt,n);

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