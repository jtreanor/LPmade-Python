/*
------------------------------------------------
Copyright (C) 2010 by Ryan N. Lichtenwalter
Email: rlichtenwalter@gmail.com

This file is part of LPmade.
LPmade is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. LPmade is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with LPmade. If not, see <http://www.gnu.org/licenses/>.
------------------------------------------------
*/

#include "RootedPageRankLinkPredictor.h"
#include "../Statistics.h"

RootedPageRankLinkPredictor::RootedPageRankLinkPredictor( const WeightedNetwork& network, const WeightedNetwork& completeNetwork,double alpha ) : LinkPredictor(network,completeNetwork), alpha(alpha) {
}

RootedPageRankLinkPredictor::~RootedPageRankLinkPredictor() {
}


std::set<vertex_t> RootedPageRankLinkPredictor::circleOfTrust(unsigned int vertex, int n) {
	this->generateScore(vertex,0); //Build pageranks if nessesary

	std::set<vertex_t> circleOfTrust;

	std::priority_queue<std::pair<double, int>> q;
	for (unsigned int i = 0; i < this->scores.size(); ++i) {
		q.push(std::pair<double, int>(this->scores[i], i));
	}
	  
	for (int i = 0; i < n; ++i) {
		int index = q.top().second;
		circleOfTrust.insert(index);
	    q.pop();
	}

	return circleOfTrust;
}

double RootedPageRankLinkPredictor::generateScore( unsigned int vertex, unsigned int neighbor ) {
	if( this->vertex != vertex ) {
		this->vertex = vertex;
		this->scores = vector<double>( this->network.vertexCount() );
		
		vector<double> oldPageRanks = vector<double>( this->network.vertexCount() );
		
		vertex_t currentVertex = vertex;
		this->scores.at( currentVertex )++;
		for( unsigned int step = 1; true; step++ ) {
			const neighbor_set_t& neighbors = this->network.outNeighbors( currentVertex );
			if( neighbors.size() < 1 || (double)rand()/RAND_MAX < this->alpha ) {
				currentVertex = vertex;
			} else {					
				vertex_t randomNeighbor = rand() % neighbors.size();
				currentVertex = neighbors.at( randomNeighbor ).first;
			}
			this->scores.at( currentVertex )++;
			
			if( step == 100000 ) {
				oldPageRanks = this->scores;
			} else if( step % 100000 == 0 ) {
				double r = Statistics<double>::sampleCorrelationCoefficient( oldPageRanks, this->scores );
//				cerr << r << "\n";
				if( r > 0.9999 ) {
					return this->scores.at( neighbor );
				} else {
					oldPageRanks = this->scores;
				}
			}
		}
	}
	return this->scores.at( neighbor );
}
