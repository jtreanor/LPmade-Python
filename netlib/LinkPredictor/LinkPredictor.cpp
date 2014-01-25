/*
------------------------------------------------
Copyright (C) 2010 by Ryan N. Lichtenwalter
Email: rlichtenwalter@gmail.com

This file is part of LPmade.
LPmade is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. LPmade is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with LPmade. If not, see <http://www.gnu.org/licenses/>.
------------------------------------------------
*/

#include <iostream>
#include <limits>
#include "LinkPredictor.h"
#include <math.h>

using std::numeric_limits;
using std::cout;
using std::cerr;
using std::scientific;
using std::ios_base;

double zScore(double value,double standard_deviation,double mean) {
	return standard_deviation > 0 ? (value - mean)/standard_deviation : 0;
}

LinkPredictor::LinkPredictor( const WeightedNetwork& network, const WeightedNetwork& completeNetwork ) : network(network), completeNetwork(completeNetwork), vertex(INVALID_VERTEX), neighbor(INVALID_VERTEX) {
}

LinkPredictor::~LinkPredictor() {
}

std::vector< std::pair<double, vertex_t> > LinkPredictor::topNNormalised(unsigned int vertex, int n) {
	std::priority_queue< std::pair<double, int> , vector< std::pair<double, int> >, PairCompare > q;

	vertex_t intVertex = this->network.translateExtToInt(vertex);

    double score_sum = 0.0;
    double sq_sum = 0.0;
    double count = this->network.vertexCount();

	for (unsigned int i = 0; i < this->network.vertexCount(); ++i) {
		double score = generateScoreIfNotNeighborsInt(intVertex,i);
		score_sum += score;
		sq_sum += score * score;
		q.push(std::pair<double, int>( score, i ));
	}

    double mean = score_sum / count;
    double standard_deviation = sqrt(sq_sum / count - mean * mean);

	std::vector< std::pair<double, vertex_t> > topVertices;

	for (int i = 0; i < n; ++i) {
		vertex_t extVertex = this->network.translateIntToExt(q.top().second);
		double normalisedScore = zScore( q.top().first, standard_deviation, mean );

		topVertices.push_back( std::pair<double, vertex_t>( normalisedScore, extVertex )   );
		q.pop();
	}

	return topVertices;
}

std::vector<vertex_t> LinkPredictor::topNVertices(unsigned int vertex, int n) {
	std::priority_queue< std::pair<double, int> , vector< std::pair<double, int> >, PairCompare > q;
	for (unsigned int i = 0; i < this->network.vertexCount(); ++i) {
		q.push(std::pair<double, int>( generateScoreIfNotNeighborsInt(vertex,i) , i));
	}

	std::vector<vertex_t> topVertices;

	for (int i = 0; i < n; ++i) {
		topVertices.push_back(q.top().second);
		q.pop();
	}

	return topVertices;
}

std::vector<vertex_t> LinkPredictor::topNVerticesExt(unsigned int vertex, int n) {
	std::priority_queue< std::pair<double, int> , vector< std::pair<double, int> >, PairCompare > q;

	std::vector<vertex_t> topVertices;

	vertex_t intVertex = this->network.translateExtToInt(vertex);
	if (intVertex == INVALID_VERTEX) {
		return topVertices;
	}

	for (unsigned int i = 0; i < this->network.vertexCount(); ++i) {
		q.push(std::pair<double, int>( generateScoreIfNotNeighborsInt(intVertex,i), i ));
	}

	for (int i = 0; i < n; ++i) {
		topVertices.push_back( this->network.translateIntToExt(q.top().second) );
		q.pop();
	}

	return topVertices;
}


double LinkPredictor::generateScoreIfNotNeighborsInt( vertex_t a, vertex_t b) {
	if ( completeNetwork.hasEdge(a,b) || a == b ) {
		return 0;
	}
	return generateScore(a,b);
}

double LinkPredictor::generateScoreIfNotNeighbors( vertex_t a, vertex_t b) {
	if ( completeNetwork.hasEdgeExt(a,b) || a == b ) {
		return 0;
	}
	return generateScoreExt(a,b);
}

double LinkPredictor::generateScoreExt( vertex_t a, vertex_t b) {
	return generateScore(this->network.translateExtToInt( a ),this->network.translateExtToInt( b ));
}

void LinkPredictor::printOutNeighborScoress( unsigned int degree ) {
	// if the degree is 0, do all-pairs predictions
	if( degree == 0 ) {
		for( vertex_t vertex = 0; vertex < network.vertexCount(); ++vertex ) {
			for( vertex_t neighbor = 0; neighbor < network.vertexCount(); ++neighbor ) {
				if( vertex != neighbor && !network.hasEdge( vertex, neighbor ) ) {
					// DO WE PREDICT LINKS THAT ALREADY EXIST? - // DO PROVIDED NETWORKS INCLUDE LINKS IN BOTH DIRECTIONS
					double predictedValue = this->generateScore( vertex, neighbor );
					unsigned int vertexName = this->network.translateIntToExt( vertex );
					unsigned int neighborName = this->network.translateIntToExt( neighbor );
					cout << vertexName << " " << neighborName << " " << predictedValue << "\n";;
				}
			}
		}
		return;
	}
	for( vertex_t vertex = 0; vertex < network.vertexCount(); vertex++ ) {
		vector<vertex_t> verticesToPredict;
		verticesToPredict = this->network.findOutNeighbors( vertex, degree );
		for( vector<unsigned int>::const_iterator neighborIterator = verticesToPredict.begin(); neighborIterator != verticesToPredict.end(); neighborIterator++ ) {
			vertex_t neighbor = *neighborIterator;
			double predictedValue = this->generateScore( vertex, neighbor );
			unsigned int vertexName = this->network.translateIntToExt( vertex );
			unsigned int neighborName = this->network.translateIntToExt( neighbor );
			cout << vertexName << " " << neighborName << " " << predictedValue << "\n";;
		}
	}
}

void LinkPredictor::printInNeighborScores( unsigned int degree ) {
	// if the degree is 0, do all-pairs predictions
	if( degree == 0 ) {
		for( vertex_t vertex = 0; vertex < network.vertexCount(); ++vertex ) {
			for( vertex_t neighbor = 0; neighbor < network.vertexCount(); ++neighbor ) {
				if( vertex != neighbor && !network.hasEdge( neighbor, vertex ) ) {
					// DO WE PREDICT LINKS THAT ALREADY EXIST? - // DO PROVIDED NETWORKS INCLUDE LINKS IN BOTH DIRECTIONS
					double predictedValue = this->generateScore( vertex, neighbor );
					unsigned int vertexName = this->network.translateIntToExt( vertex );
					unsigned int neighborName = this->network.translateIntToExt( neighbor );
					cout << vertexName << " " << neighborName << " " << predictedValue << "\n";;
				}
			}
		}
		return;
	}
	for( vertex_t vertex = 0; vertex < network.vertexCount(); vertex++ ) {
		vector<vertex_t> verticesToPredict;
		verticesToPredict = this->network.findInNeighbors( vertex, degree );
		for( vector<unsigned int>::const_iterator neighborIterator = verticesToPredict.begin(); neighborIterator != verticesToPredict.end(); neighborIterator++ ) {
			vertex_t neighbor = *neighborIterator;
			double predictedValue = this->generateScore( vertex, neighbor );
			unsigned int vertexName = this->network.translateIntToExt( vertex );
			unsigned int neighborName = this->network.translateIntToExt( neighbor );
			cout << vertexName << " " << neighborName << " " << predictedValue << "\n";;
		}
	}
}
