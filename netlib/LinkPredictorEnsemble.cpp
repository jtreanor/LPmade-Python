#include "LinkPredictorEnsemble.h"
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

LinkPredictorEnsemble::LinkPredictorEnsemble( const WeightedNetwork& trainingNetwork, const WeightedNetwork& originalTraining, const std::vector<int>& algorithms ) : trainingNetwork(trainingNetwork), originalTraining(originalTraining), algorithms(algorithms) {
	this->linkPredictors = std::vector<LinkPredictor*>();
	for (int algorithm : this->algorithms) {
		Algorithm rec = (Algorithm) algorithm;
		this->linkPredictors.push_back( this->predictorForType(rec) );
	}
}

LinkPredictorEnsemble::~LinkPredictorEnsemble() {
}

LinkPredictor* LinkPredictorEnsemble::predictorForType(Algorithm recommender) {
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

std::vector<vertex_t> LinkPredictorEnsemble::topNVerticesExt(vertex_t vertexExt, int n) {
	if (this->linkPredictors.size() == 1) {
		return linkPredictors.at(0)->topNVerticesExt(vertexExt,n);
	}

	std::vector<double> averageScores = std::vector<double>( this->trainingNetwork.vertexCount() );

	double proportion = 1.0 / this->linkPredictors.size();

	for ( LinkPredictor *pred : this->linkPredictors ) {
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

std::vector<vertex_t> LinkPredictorEnsemble::topNVertices(vertex_t vertex, int n) {
	if (this->linkPredictors.size() == 1) {
		return this->linkPredictors.at(0)->topNVerticesExt(vertex,n);
	}

	std::vector<double> averageScores = std::vector<double>( this->trainingNetwork.vertexCount() );

	double proportion = 1.0 / this->linkPredictors.size();

	for ( LinkPredictor *pred : this->linkPredictors ) {
		std::vector<double> predictorScores = pred->allNormalised(vertex);
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
		topVertices.push_back( std::get<2>(q.top()) );
		q.pop();
	}

	return topVertices;
}