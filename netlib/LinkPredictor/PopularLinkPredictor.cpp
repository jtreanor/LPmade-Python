#include "PopularLinkPredictor.h"

PopularLinkPredictor::PopularLinkPredictor( const WeightedNetwork& network, const WeightedNetwork& completeNetwork ) : LinkPredictor(network,completeNetwork) {
}

PopularLinkPredictor::~PopularLinkPredictor() {
}

double PopularLinkPredictor::generateScore( vertex_t vertex, vertex_t neighbor ) {
	return (double)(this->network.inDegree( neighbor ));
}
