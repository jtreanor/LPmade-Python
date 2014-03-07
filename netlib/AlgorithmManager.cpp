#include "AlgorithmManager.h"
#include "LinkPredictor/AdamicAdarLinkPredictor.h"
#include "LinkPredictor/CommonNeighborLinkPredictor.h"
#include "LinkPredictor/DistanceLinkPredictor.h"
#include "LinkPredictor/JaccardCoefficientLinkPredictor.h"
#include "LinkPredictor/KatzLinkPredictor.h"
#include "LinkPredictor/LinkPredictor.h"
#include "LinkPredictor/OneLinkPredictor.h"
#include "LinkPredictor/PreferentialAttachmentLinkPredictor.h"
#include "LinkPredictor/PropFlowLinkPredictor.h"
#include "LinkPredictor/UnweightedPropFlowLinkPredictor.h"
#include "LinkPredictor/RootedPageRankLinkPredictor.h"
#include "LinkPredictor/InDegreeLinkPredictor.h"
#include "LinkPredictor/OutDegreeLinkPredictor.h"
#include "LinkPredictor/WTFLinkPredictor.h"
#include "LinkPredictor/SimRankLinkPredictor.h"
#include "LinkPredictor/MutualityLinkPredictor.h"
#include "LinkPredictor/TriangleLinkPredictor.h"

AlgorithmManager::AlgorithmManager( const WeightedNetwork& network, const std::vector<int>& algorithms, const std::vector<int>& directions ) : directedNetwork(network),  undirectedNetwork(this->directedNetwork.undirected()), reversedNetwork(this->directedNetwork.reverseEdges()), algorithms(algorithms), directions(directions) {	
}

AlgorithmManager::~AlgorithmManager() {
}

const WeightedNetwork& AlgorithmManager::networkForDirection(int direction) const {
	switch (direction) {
		case UNDIRECTED:
			return this->undirectedNetwork;
		case REVERSED:
			return this->reversedNetwork;
	}
	return this->directedNetwork;
}

LinkPredictor* AlgorithmManager::predictorForType(int recommender, int direction) const {
	switch (recommender) {
		case COMMON_NEIGHBOURS:
			return new CommonNeighborLinkPredictor(this->networkForDirection(direction), this->directedNetwork);
		case ADAMIC_ADAR:
			return new AdamicAdarLinkPredictor(this->networkForDirection(direction),this->directedNetwork);
		case PROP_FLOW:
			return new PropFlowLinkPredictor(this->networkForDirection(direction),this->directedNetwork, 5 );
		case UNWEIGHTED_PROP_FLOW:
			return new UnweightedPropFlowLinkPredictor(this->networkForDirection(direction),this->directedNetwork, 5 );
		case GRAPH_DISTANCE:
			return new DistanceLinkPredictor(this->networkForDirection(direction),this->directedNetwork, 5);
		case ROOTED_PAGE_RANK:
			return new RootedPageRankLinkPredictor(this->networkForDirection(direction),this->directedNetwork, 0.15 );
		case JACCARD:
			return new JaccardCoefficientLinkPredictor(this->networkForDirection(direction),this->directedNetwork);
		case PREFERENTIAL_ATTACHMENT:
			return new PreferentialAttachmentLinkPredictor(this->networkForDirection(direction),this->directedNetwork);
		case KATZ_MEASURE:
			return new KatzLinkPredictor(this->undirectedNetwork,this->directedNetwork, 5, 0.005 );
		case WTF:
			return new WTFLinkPredictor(this->networkForDirection(direction),this->directedNetwork, 200000, 300 );
		case IN_DEGREE:
			return new InDegreeLinkPredictor(this->networkForDirection(direction),this->directedNetwork );
		case RANDOM:
			return new OneLinkPredictor(this->networkForDirection(direction),this->directedNetwork);
		case SIM_RANK:
			return new SimRankLinkPredictor(this->networkForDirection(direction),this->directedNetwork, 0.8);
		case MUTUALITY:
			return new MutualityLinkPredictor(this->networkForDirection(direction),this->directedNetwork);
		case TRIANGLE:
			return new TriangleLinkPredictor(this->networkForDirection(direction),this->directedNetwork, -1);
	}
	return NULL;
}

std::vector<LinkPredictor*> AlgorithmManager::linkPredictors() const {
	std::vector<LinkPredictor*> linkPredictors = std::vector<LinkPredictor*>();

	for (unsigned int i = 0; i < algorithms.size(); i++) {
		linkPredictors.push_back( this->predictorForType(this->algorithms.at(i), this->directions.at(i) ) );
	}
	return linkPredictors;
}