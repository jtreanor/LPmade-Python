/*
------------------------------------------------
Copyright (C) 2010 by Ryan N. Lichtenwalter
Email: rlichtenwalter@gmail.com

This file is part of LPmade.
LPmade is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. LPmade is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with LPmade. If not, see <http://www.gnu.org/licenses/>.
------------------------------------------------
*/

#include <iostream>

#include "UnweightedPropFlowLinkPredictor.h"

using std::cout;

UnweightedPropFlowLinkPredictor::UnweightedPropFlowLinkPredictor( const WeightedNetwork& network, const WeightedNetwork& completeNetwork,unsigned int l ) : LinkPredictor(network,completeNetwork), l(l) {
}

UnweightedPropFlowLinkPredictor::~UnweightedPropFlowLinkPredictor() {
}

double UnweightedPropFlowLinkPredictor::generateScore( unsigned int vertex, unsigned int neighbor ) {
	if( this->vertex != vertex ) {
		this->vertex = vertex;
		this->scores = vector<double>( this->network.vertexCount() );
		
		vector<bool> found = vector<bool>( this->network.vertexCount() );
		
		vector<vertex_t> oldSearch;
		
		found.at( vertex ) = true;
		oldSearch.push_back( vertex );
		this->scores.at( vertex ) = 1;
		for( unsigned int degree = 0; degree < this->l; degree++ ) {
			vector<vertex_t> newSearch;
			for( vector<vertex_t>::const_iterator vertexIterator = oldSearch.begin(); vertexIterator != oldSearch.end(); vertexIterator++ ) { 
				const vertex_t searchVertex = *vertexIterator;
				double sourceVertexInput = this->scores.at( searchVertex );
				const neighbor_set_t& neighbors = this->network.outNeighbors( searchVertex );
				for( neighbor_set_t::const_iterator neighborIterator = neighbors.begin(); neighborIterator != neighbors.end(); neighborIterator++ ) {
					const vertex_t neighbor = neighborIterator->first;
					this->scores.at( neighbor ) += sourceVertexInput * ((double)1 / neighbors.size());
					if( !found.at( neighbor ) ) {
						found.at( neighbor ) = true;
						newSearch.push_back( neighbor );
					}
				}
			}
			oldSearch.swap( newSearch );
		}
	}
	cout << network.translateIntToExt( vertex ) << " " << network.translateIntToExt( neighbor ) << " " << this->scores.at( neighbor ) << "\n";
	return this->scores.at( neighbor );
}
