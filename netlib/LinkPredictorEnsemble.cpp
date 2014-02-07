#include "LinkPredictorEnsemble.h"
#include <queue>
#include <tuple>

LinkPredictorEnsemble::LinkPredictorEnsemble( const WeightedNetwork& trainingNetwork, const std::vector<int>& algorithms, const std::vector<int>& directions ) : trainingNetwork(trainingNetwork), algorithms(algorithms), directions(directions) {
	this->linkPredictors = std::vector<LinkPredictor*>();
	for (int i = 0; i < this->algorithms.size(); i++) {
		this->linkPredictors.push_back( Algorithm::predictorForType(this->algorithms.at(i), this->directions.at(i), trainingNetwork) );
	}
}

LinkPredictorEnsemble::~LinkPredictorEnsemble() {
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