#include "Algorithm.h"
#include "LinkPredictor/AdamicAdarLinkPredictor.h"
#include "LinkPredictor/CommonNeighborLinkPredictor.h"
#include "LinkPredictor/DistanceLinkPredictor.h"
#include "LinkPredictor/JaccardCoefficientLinkPredictor.h"
#include "LinkPredictor/KatzLinkPredictor.h"
#include "LinkPredictor/LinkPredictor.h"
#include "LinkPredictor/OneLinkPredictor.h"
#include "LinkPredictor/PreferentialAttachmentLinkPredictor.h"
#include "LinkPredictor/PropFlowLinkPredictor.h"
#include "LinkPredictor/RootedPageRankLinkPredictor.h"
#include "LinkPredictor/InDegreeLinkPredictor.h"
#include "LinkPredictor/OutDegreeLinkPredictor.h"
#include "LinkPredictor/WTFLinkPredictor.h"

Algorithm::Algorithm( const WeightedNetwork& network, const WeightedNetwork& undirectedNetwork, const WeightedNetwork& reversedNetwork ) : directedNetwork(network),  undirectedNetwork(undirectedNetwork), reversedNetwork(reversedNetwork) {	
}

Algorithm::~Algorithm() {
}

const WeightedNetwork& Algorithm::networkForDirection(int direction) const {
	// switch (direction) {
	// 	case UNDIRECTED:
	return this->undirectedNetwork;
	// 	case REVERSED:
	// 		return this->reversedNetwork;
	// }
	// return this->directedNetwork;
}

LinkPredictor* Algorithm::predictorForType(int recommender, int direction) const {
	switch (recommender) {
		case COMMON_NEIGHBOURS:
			return new CommonNeighborLinkPredictor(this->networkForDirection(direction), this->directedNetwork);
		case ADAMIC_ADAR:
			return new AdamicAdarLinkPredictor(this->networkForDirection(direction),this->directedNetwork);
		case PROP_FLOW:
			return new PropFlowLinkPredictor(this->networkForDirection(direction),this->directedNetwork, 5 );
		case GRAPH_DISTANCE:
			return new DistanceLinkPredictor(this->networkForDirection(direction),this->directedNetwork, 5);
		case ROOTED_PAGE_RANK:
			return new RootedPageRankLinkPredictor(this->networkForDirection(direction),this->directedNetwork, 0.15 );
		case JACCARD:
			return new JaccardCoefficientLinkPredictor(this->networkForDirection(direction),this->directedNetwork);
		case PREFERENTIAL_ATTACHMENT:
			return new PreferentialAttachmentLinkPredictor(this->networkForDirection(direction),this->directedNetwork);
		case KATZ_MEASURE:
			return new KatzLinkPredictor(this->networkForDirection(direction),this->directedNetwork, 5, 0.005 );
		case WTF:
			return new WTFLinkPredictor(this->networkForDirection(direction),this->directedNetwork, 0.15 );
		case IN_DEGREE:
			return new InDegreeLinkPredictor(this->networkForDirection(direction),this->directedNetwork );
		case RANDOM:
			return new OneLinkPredictor(this->networkForDirection(direction),this->directedNetwork);
	}
	return NULL;
}