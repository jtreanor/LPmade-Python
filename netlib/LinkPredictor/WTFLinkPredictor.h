#ifndef WTFLINKPREDICTOR_H
#define WTFLINKPREDICTOR_H

#include <vector>
#include "../WeightedNetwork.h"
#include "LinkPredictor.h"

using std::vector;

class WTFLinkPredictor : public LinkPredictor {
	private:
		double alpha;
		int hubSize;
		vector<double> scores;
		WeightedNetwork salsaNetwork;
		std::vector<vertex_t> hubs;
		std::vector<vertex_t> authorities;
		LinkPredictor *hubPredictor;
		std::vector<vertex_t> generateHubs( vertex_t, int );
		std::vector<vertex_t> generateAuthorities();
		WTFLinkPredictor( const WeightedNetwork&, const WeightedNetwork& );
	protected:
	public:
		WTFLinkPredictor( const WeightedNetwork&, const WeightedNetwork&, double, int );
		~WTFLinkPredictor();
		double generateScore( vertex_t, vertex_t );
};

#endif
