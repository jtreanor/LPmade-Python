/*
------------------------------------------------
Copyright (C) 2011 by Ryan N. Lichtenwalter
Email: rlichtenwalter@gmail.com

This file is part of LPmade.
LPmade is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. LPmade is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with LPmade. If not, see <http://www.gnu.org/licenses/>.
------------------------------------------------
*/

#ifndef VCP3UNDIRECTEDLINKPREDICTOR_H
#define VCP3UNDIRECTEDLINKPREDICTOR_H

#include "../WeightedNetwork.h"
#include "LinkPredictor.h"

struct DetailedNeighbors;

class VCP3UndirectedLinkPredictor : public LinkPredictor {
	private:
		static const unsigned long graphlets = 8;
		double counts[graphlets];
		struct DetailedNeighbors {
			vector<vertex_t> either;
		};
		struct DetailedNeighbors computeDetailedNeighbors( const neighbor_set_t&, const neighbor_set_t&, vertex_t avoidance ) const;
		unsigned long intersectionCount( const vector<vertex_t>&, const vector<vertex_t>& ) const;
		unsigned long differenceCount( const vector<vertex_t>&, const vector<vertex_t>& ) const;
		unsigned long unionCount( const vector<vertex_t>&, const vector<vertex_t>& ) const;
		void edgeProfile( vertex_t, vertex_t, unsigned long[] ) const;
	protected:
	public:
		VCP3UndirectedLinkPredictor( const WeightedNetwork&, const WeightedNetwork& );
		~VCP3UndirectedLinkPredictor();
		double generateScore( vertex_t, vertex_t );
};

#endif
