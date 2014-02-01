#ifndef POPULARLINKPREDICTOR_H
#define POPULARLINKPREDICTOR_H

#include "../WeightedNetwork.h"
#include "LinkPredictor.h"

class PopularLinkPredictor : public LinkPredictor {
	private:
	protected:
	public:
		PopularLinkPredictor( const WeightedNetwork&, const WeightedNetwork& );
		~PopularLinkPredictor();
		double generateScore( vertex_t, vertex_t );
};

#endif
