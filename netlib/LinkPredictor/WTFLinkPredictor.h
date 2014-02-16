#ifndef WTFLINKPREDICTOR_H
#define WTFLINKPREDICTOR_H

#include <vector>
#include "../WeightedNetwork.h"
#include "LinkPredictor.h"
#include "RootedPageRankLinkPredictor.h"

using std::vector;

class WTFLinkPredictor : public LinkPredictor {
	private:
		double alpha;
		int hubSize;
		vector<double> scores;
		WeightedNetwork salsaNetwork;
		std::vector<vertex_t> hubs;
		std::vector<vertex_t> authorities;
		RootedPageRankLinkPredictor rootedPageRankLinkPredictor;
		vertex_t nextVertex( vertex_t, bool );
		vertex_t randomHub( );
		vertex_t randomAuth( );
		WTFLinkPredictor( const WeightedNetwork&, const WeightedNetwork& );
	protected:
	public:
		WTFLinkPredictor( const WeightedNetwork&, const WeightedNetwork&, double, int );
		~WTFLinkPredictor();
		double generateScore( vertex_t, vertex_t );
};

#endif
