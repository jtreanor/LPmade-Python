#include "InDegreeLinkPredictor.h"

InDegreeLinkPredictor::InDegreeLinkPredictor( const WeightedNetwork& network, const WeightedNetwork& completeNetwork ) : LinkPredictor(network,completeNetwork) {
}

InDegreeLinkPredictor::~InDegreeLinkPredictor() {
}

double InDegreeLinkPredictor::generateScore( vertex_t vertex, vertex_t neighbor ) {
	return (double)(this->network.inDegree( neighbor ));
}
