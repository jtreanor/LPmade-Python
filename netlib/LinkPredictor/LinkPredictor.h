/*
------------------------------------------------
Copyright (C) 2010 by Ryan N. Lichtenwalter
Email: rlichtenwalter@gmail.com

This file is part of LPmade.
LPmade is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. LPmade is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with LPmade. If not, see <http://www.gnu.org/licenses/>.
------------------------------------------------
*/

#ifndef LINKPREDICTOR_H
#define LINKPREDICTOR_H

#include <vector>
#include <queue>
#include "../WeightedNetwork.h"

using std::ostream;
using std::vector;

class LinkPredictor {
	private:
		LinkPredictor& operator=( const LinkPredictor& );
	protected:
		const WeightedNetwork& network;
		const WeightedNetwork& completeNetwork;
		vertex_t vertex;
		vertex_t neighbor;
		vertex_t threshold;
		LinkPredictor();
	public:
		static vertex_t cutoff;
		static vertex_t getCutOff();
		static vertex_t setCutOff(vertex_t);
		enum Direction { IN, OUT };
		LinkPredictor( const WeightedNetwork&, const WeightedNetwork& );
		virtual ~LinkPredictor();
		virtual double generateScore( vertex_t, vertex_t ) = 0;
		std::vector<double> allNormalised(vertex_t);
		std::vector<double> allScores(vertex_t);
		std::vector<vertex_t> topNVertices(vertex_t, int);
		std::vector<vertex_t> topNVerticesExt(vertex_t, int);
		std::vector<vertex_t> topNVerticesExt(vertex_t, int, int);
		double generateScoreIfNotNeighborsInt( vertex_t , vertex_t );
		double generateScoreIfNotNeighbors( vertex_t, vertex_t );
		double generateScoreExt( vertex_t, vertex_t );
		void printOutNeighborScoress( unsigned int degree );
		void printInNeighborScores( unsigned int degree );
};

#endif
