/*
------------------------------------------------
Copyright (C) 2010 by Ryan N. Lichtenwalter
Email: rlichtenwalter@gmail.com

This file is part of LPmade.
LPmade is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. LPmade is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with LPmade. If not, see <http://www.gnu.org/licenses/>.
------------------------------------------------
*/

#ifndef ROOTEDPAGERANKLINKPREDICTOR_H
#define ROOTEDPAGERANKLINKPREDICTOR_H

#include <vector>
#include "../WeightedNetwork.h"
#include "LinkPredictor.h"

using std::vector;

class RootedPageRankLinkPredictor : public LinkPredictor {
	private:
		double alpha;
		vector<double> scores;
		RootedPageRankLinkPredictor( const WeightedNetwork&, const WeightedNetwork& );
	protected:
	public:
		RootedPageRankLinkPredictor( const WeightedNetwork&, const WeightedNetwork&, double );
		~RootedPageRankLinkPredictor();
		double generateScore( vertex_t, vertex_t );
		std::set<vertex_t> circleOfTrust(vertex_t, int);
};

#endif
