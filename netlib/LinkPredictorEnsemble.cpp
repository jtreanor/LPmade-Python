#include "LinkPredictorEnsemble.h"
#include <queue>
#include <tuple>
#include <math.h> 

LinkPredictorEnsemble::LinkPredictorEnsemble( const WeightedNetwork& trainingNetwork, const std::vector<double>& weights, const std::vector<int>& degrees , int ensembleType, const AlgorithmManager& alg ) : trainingNetwork(trainingNetwork), weights(weights), alg(alg), degrees(degrees), ensembleType(ensembleType) {
	this->linkPredictors = alg.linkPredictors();
	srand(time(NULL));
}

LinkPredictorEnsemble::~LinkPredictorEnsemble() {
	for (LinkPredictor *lp : this->linkPredictors) {
		delete lp;
	}
}

std::vector<vertex_t> LinkPredictorEnsemble::topNVerticesExt(vertex_t vertexExt, int n) const {
	switch (this->ensembleType) {
		case MEAN:
			return topNVerticesExtMean(vertexExt,n);
		case BORDA:
			return topNVerticesExtBorda(vertexExt,n);
		case LOGISTIC_REGRESSION:
			return topNVerticesExtLR(vertexExt,n);
	}
	return topNVerticesExtMean(vertexExt,n);
}

std::vector<vertex_t> LinkPredictorEnsemble::topNVerticesExtBorda(vertex_t vertexExt, int n) const {
	if (this->linkPredictors.size() == 1) {
		if (this->degrees.at(0) == 0) {
			return linkPredictors.at(0)->topNVerticesExt(vertexExt,n);
		} else {
			return linkPredictors.at(0)->topNVerticesExt(vertexExt,n,this->degrees.at(0));
		}
	}
	
	std::vector<double> bordaScores = std::vector<double>( this->trainingNetwork.vertexCount() );

	for ( unsigned int l = 0; l < this->linkPredictors.size(); l++ ) {
		LinkPredictor *pred = this->linkPredictors.at(l);
		std::vector<vertex_t> topNRecs = this->degrees.at(l) == 0 ? pred->topNVerticesExt(vertexExt, n) : pred->topNVerticesExt(vertexExt, n, this->degrees.at(l));

		//Weight for this predictor
		double weight = this->weights.at(l);

		for (unsigned int i = 0; i < topNRecs.size(); i++) {
			double rank = topNRecs.size() - i;
			vertex_t rec = this->trainingNetwork.translateExtToInt(topNRecs.at(i));

			bordaScores.at(rec) += rank * weight;
		}
	}

	std::priority_queue< std::tuple<double, int ,int> > q;

	for (size_t i = 0; i < bordaScores.size(); ++i) {
		q.push( std::make_tuple( bordaScores.at(i), rand(), i ) );
	}

	std::vector<vertex_t> topVertices;

	for (int i = 0; i < n; ++i) {
		topVertices.push_back( this->trainingNetwork.translateIntToExt( std::get<2>(q.top()) ) );
		q.pop();
	}

	return topVertices;
}

std::vector<vertex_t> LinkPredictorEnsemble::topNVerticesExtMean(vertex_t vertexExt, int n) const {
	if (this->linkPredictors.size() == 1) {
		if (this->degrees.at(0) == 0) {
			return linkPredictors.at(0)->topNVerticesExt(vertexExt,n);
		} else {
			return linkPredictors.at(0)->topNVerticesExt(vertexExt,n,this->degrees.at(0));
		}
	}

	std::vector<double> averageScores = std::vector<double>( this->trainingNetwork.vertexCount() );

	for ( unsigned int l = 0; l < this->linkPredictors.size(); l++ ) {
		LinkPredictor *pred = this->linkPredictors.at(l);

		//Metric for every vertex
		std::vector<double> predictorScores = pred->allNormalised(vertexExt);

		//Weight for this predictor
		double weight = this->weights.at(l);

		for (size_t i = 0; i < averageScores.size(); ++i) {
    		averageScores.at(i) += predictorScores.at(i) * weight;
		}
	}

	std::priority_queue< std::tuple<double, int ,int> > q;

	for (size_t i = 0; i < averageScores.size(); ++i) {
		q.push( std::make_tuple( averageScores.at(i), rand(), i ) );
	}

	std::vector<vertex_t> topVertices;

	for (int i = 0; i < n; ++i) {
		topVertices.push_back( this->trainingNetwork.translateIntToExt( std::get<2>(q.top()) ) );
		q.pop();
	}

	return topVertices;
}

std::vector<vertex_t> LinkPredictorEnsemble::topNVerticesExtLR(vertex_t vertexExt, int n) const {
	if (this->linkPredictors.size() == 1) {
		if (this->degrees.at(0) == 0) {
			return linkPredictors.at(0)->topNVerticesExt(vertexExt,n);
		} else {
			return linkPredictors.at(0)->topNVerticesExt(vertexExt,n,this->degrees.at(0));
		}
	}

	std::vector<double> averageScores = std::vector<double>( this->trainingNetwork.vertexCount());

	for ( unsigned int l = 0; l < this->linkPredictors.size(); l++ ) {
		LinkPredictor *pred = this->linkPredictors.at(l);

		//Metric for every vertex
		std::vector<double> predictorScores = pred->allNormalised(vertexExt);

		//Weight for this predictor
		double weight = this->weights.at(l);

		for (size_t i = 0; i < averageScores.size(); ++i) {
    		averageScores.at(i) += predictorScores.at(i) * weight;
		}
	}

	//From sklearn
	for (size_t i = 0; i < averageScores.size(); ++i) {
    	averageScores.at(i) = 1.0 / ( 1.0 + exp(averageScores.at(i) + /*Intercept*/this->weights.at(this->linkPredictors.size())) );
	}

	std::priority_queue< std::tuple<double, int ,int> > q;

	for (size_t i = 0; i < averageScores.size(); ++i) {
		q.push( std::make_tuple( averageScores.at(i), rand(), i ) );
	}

	std::vector<vertex_t> topVertices;

	for (int i = 0; i < n; ++i) {
		topVertices.push_back( this->trainingNetwork.translateIntToExt( std::get<2>(q.top()) ) );
		q.pop();
	}

	return topVertices;
}