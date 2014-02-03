#include "OutDegreeLinkPredictor.h"

OutDegreeLinkPredictor::OutDegreeLinkPredictor( const WeightedNetwork& network, const WeightedNetwork& completeNetwork ) : LinkPredictor(network,completeNetwork) {
}

OutDegreeLinkPredictor::~OutDegreeLinkPredictor() {
}

double OutDegreeLinkPredictor::generateScore( vertex_t vertex, vertex_t neighbor ) {
	return (double)(this->network.outDegree( neighbor ));
}
