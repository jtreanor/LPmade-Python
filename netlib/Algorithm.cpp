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

WeightedNetwork Algorithm::modifiedTrainingNetworkForDirection(int direction, const WeightedNetwork& trainingNetwork) {
	switch (direction) {
		case REVERSED:
			return trainingNetwork.reverseEdges();
		case UNDIRECTED:
			return trainingNetwork.undirected();
	}
	return trainingNetwork;
}

LinkPredictor* Algorithm::predictorForType(int recommender, int direction, const WeightedNetwork& trainingNetwork) {
	const WeightedNetwork& newTrainingNetwork = Algorithm::modifiedTrainingNetworkForDirection(direction, trainingNetwork);

	switch (recommender) {
		case COMMON_NEIGHBOURS:
			return new CommonNeighborLinkPredictor(newTrainingNetwork, trainingNetwork);
		case ADAMIC_ADAR:
			return new AdamicAdarLinkPredictor(newTrainingNetwork, trainingNetwork);
		case PROP_FLOW:
			return new PropFlowLinkPredictor(newTrainingNetwork, trainingNetwork, 5 );
		case GRAPH_DISTANCE:
			return new DistanceLinkPredictor(newTrainingNetwork, trainingNetwork, 5);
		case ROOTED_PAGE_RANK:
			return new RootedPageRankLinkPredictor(newTrainingNetwork, trainingNetwork, 0.15 );
		case JACCARD:
			return new JaccardCoefficientLinkPredictor(newTrainingNetwork, trainingNetwork);
		case PREFERENTIAL_ATTACHMENT:
			return new PreferentialAttachmentLinkPredictor(newTrainingNetwork, trainingNetwork);
		case KATZ_MEASURE:
			return new KatzLinkPredictor(newTrainingNetwork, trainingNetwork, 5, 0.005 );
		case WTF:
			return new WTFLinkPredictor(newTrainingNetwork, trainingNetwork, 0.15 );
		case IN_DEGREE:
			return new InDegreeLinkPredictor(newTrainingNetwork, trainingNetwork );
		case RANDOM:
			return new OneLinkPredictor(newTrainingNetwork, trainingNetwork);
	}
	return NULL;
}