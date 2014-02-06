#include "RecommenderResult.h"
#include "LinkPredictor/AdamicAdarLinkPredictor.h"
#include "LinkPredictor/CommonNeighborLinkPredictor.h"
#include "LinkPredictor/DistanceLinkPredictor.h"
#include "LinkPredictor/JaccardCoefficientLinkPredictor.h"
#include "LinkPredictor/KatzLinkPredictor.h"
#include "LinkPredictor/LinkPredictor.h"
#include "LinkPredictor/OneLinkPredictor.h"
#include "LinkPredictor/PreferentialAttachmentLinkPredictor.h"
#include "LinkPredictor/PropFlowLinkPredictor.h"
#include "LinkPredictor/RootedPageRankLinkPredictor.h"
#include "LinkPredictor/InDegreeLinkPredictor.h"
#include "LinkPredictor/OutDegreeLinkPredictor.h"
#include "LinkPredictor/WTFLinkPredictor.h"
#include <queue>
#include <tuple>

RecommenderResult::RecommenderResult( const WeightedNetwork& trainingNetwork, const WeightedNetwork& originalTraining, const WeightedNetwork& testNetwork, const std::vector<int>& algorithms ) : trainingNetwork(trainingNetwork), originalTraining(originalTraining),testNetwork(testNetwork), algorithms(algorithms) {
	this->linkPredictors = std::vector<LinkPredictor*>();
	for (int algorithm : this->algorithms) {
		Recommender rec = (Recommender) algorithm;
		this->linkPredictors.push_back( this->predictorForType(rec) );
	}
}

RecommenderResult::~RecommenderResult() {
}

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
		case WTF:
			return new WTFLinkPredictor(this->trainingNetwork, this->originalTraining, 0.15 );
		case IN_DEGREE:
			return new InDegreeLinkPredictor(this->trainingNetwork, this->originalTraining );
		case RANDOM:
			return new OneLinkPredictor(this->trainingNetwork, this->originalTraining);
	}
	return NULL;
}

std::vector<vertex_t> RecommenderResult::acceptedNodesAt(vertex_t vertexExt, int n) {
	if (this->linkPredictors.size() == 1) {
		return linkPredictors.at(0)->topNVerticesExt(vertexExt,n);
	}

	std::vector<double> averageScores = std::vector<double>( this->trainingNetwork.vertexCount() );

	double proportion = 1.0 / linkPredictors.size();

	for ( LinkPredictor *pred : linkPredictors ) {
		std::vector<double> predictorScores = pred->allNormalised(vertexExt);
		for (size_t i = 0; i < averageScores.size(); ++i) {
    		averageScores.at(i) += predictorScores.at(i) * proportion;
		}
	}

	std::priority_queue< std::tuple<double, int ,int> > q;

	for (size_t i = 0; i < averageScores.size(); ++i) {
		double average = averageScores.at(i) / 2.0;
		q.push( std::make_tuple( average, rand(), i ) );
	}

	std::vector<vertex_t> topVertices;

	for (int i = 0; i < n; ++i) {
		topVertices.push_back( this->trainingNetwork.translateIntToExt( std::get<2>(q.top()) ) );
		q.pop();
	}

	return topVertices;
}

std::vector<double> RecommenderResult::precisionAtN(int n, int start, int end) {
	std::vector<double> precisionAtN = std::vector<double>( n );

	double proportion = 1.0/(end - start);

	for (int currentVertex = start; currentVertex < end; currentVertex++ ) {
		vertex_t currentVertexExt = this->testNetwork.translateIntToExt(currentVertex);
		
		std::vector<vertex_t> acceptedVertices = this->acceptedNodesAt(currentVertexExt,n);
		
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