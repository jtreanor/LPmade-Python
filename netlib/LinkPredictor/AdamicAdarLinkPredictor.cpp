/*
------------------------------------------------
Copyright (C) 2010 by Ryan N. Lichtenwalter
Email: rlichtenwalter@gmail.com

This file is part of LPmade.
LPmade is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. LPmade is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with LPmade. If not, see <http://www.gnu.org/licenses/>.
------------------------------------------------
*/

#include <math.h>
#include "AdamicAdarLinkPredictor.h"

AdamicAdarLinkPredictor::AdamicAdarLinkPredictor( const WeightedNetwork& network, const WeightedNetwork& completeNetwork ) : LinkPredictor(network,completeNetwork) {
}

AdamicAdarLinkPredictor::~AdamicAdarLinkPredictor() {
}

double AdamicAdarLinkPredictor::generateScore( vertex_t vertex, vertex_t neighbor ) {
	double result = 0.0;
	neighbor_set_t commonNeighbors = this->network.commonOutNeighbors( vertex, neighbor );
	for( neighbor_set_t::const_iterator cnIterator = commonNeighbors.begin(); cnIterator != commonNeighbors.end(); cnIterator++ ) {
		const vertex_t commonNeighbor = cnIterator->first;
		result += 1 / log( this->network.inDegree( commonNeighbor ) );
	}
	return result;
}
