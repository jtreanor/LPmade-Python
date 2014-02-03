#ifndef OUTDEGREELINKPREDICTOR_H
#define OUTDEGREELINKPREDICTOR_H

#include "../WeightedNetwork.h"
#include "LinkPredictor.h"

class OutDegreeLinkPredictor : public LinkPredictor {
	private:
	protected:
	public:
		OutDegreeLinkPredictor( const WeightedNetwork&, const WeightedNetwork& );
		~OutDegreeLinkPredictor();
		double generateScore( vertex_t, vertex_t );
};

#endif
