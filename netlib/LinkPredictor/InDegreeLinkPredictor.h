#ifndef INDEGREELINKPREDICTOR_H
#define INDEGREELINKPREDICTOR_H

#include "../WeightedNetwork.h"
#include "LinkPredictor.h"

class InDegreeLinkPredictor : public LinkPredictor {
	private:
	protected:
	public:
		InDegreeLinkPredictor( const WeightedNetwork&, const WeightedNetwork& );
		~InDegreeLinkPredictor();
		double generateScore( vertex_t, vertex_t );
};

#endif
