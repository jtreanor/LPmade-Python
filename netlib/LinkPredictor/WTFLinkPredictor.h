#ifndef WTFLINKPREDICTOR_H
#define WTFLINKPREDICTOR_H

#include <vector>
#include "../WeightedNetwork.h"
#include "LinkPredictor.h"

using std::vector;

class WTFLinkPredictor : public LinkPredictor {
	private:
		double alpha;
		vector<double> scores;
		WTFLinkPredictor( const WeightedNetwork&, const WeightedNetwork& );
	protected:
	public:
		WTFLinkPredictor( const WeightedNetwork&, const WeightedNetwork&,double );
		~WTFLinkPredictor();
		double generateScore( vertex_t, vertex_t );
};

#endif
