#include "RecommenderResult.h"
#include "AdamicAdarLinkPredictor.h"
#include "CommonNeighborLinkPredictor.h"
#include "DistanceLinkPredictor.h"
#include "JaccardCoefficientLinkPredictor.h"
#include "KatzLinkPredictor.h"
#include "LinkPredictor.h"
#include "OneLinkPredictor.h"
#include "PreferentialAttachmentLinkPredictor.h"
#include "PropFlowLinkPredictor.h"
#include "RootedPageRankLinkPredictor.h"
#include "WTFLinkPredictor.h"
#include <queue>

LinkPredictor* RecommenderResult::predictorForType(Recommender recommender) {
	switch (recommender) {
		case COMMON_NEIGHBOURS:
			return new CommonNeighborLinkPredictor(this->trainingNetwork, this->originalTraining);
		case ADAMIC_ADAR:
			return new AdamicAdarLinkPredictor(this->trainingNetwork, this->originalTraining);
		case PROP_FLOW:
			return new PropFlowLinkPredictor(this->trainingNetwork, this->originalTraining, 5 );
		case GRAPH_DISTANCE:
			return new DistanceLinkPredictor(this->trainingNetwork, this->originalTraining, 5);
		case ROOTED_PAGE_RANK:
			return new RootedPageRankLinkPredictor(this->trainingNetwork, this->originalTraining, 0.15 );
		case JACCARD:
			return new JaccardCoefficientLinkPredictor(this->trainingNetwork, this->originalTraining);
		case PREFERENTIAL_ATTACHMENT:
			return new PreferentialAttachmentLinkPredictor(this->trainingNetwork, this->originalTraining);
		case KATZ_MEASURE:
			return new KatzLinkPredictor(this->trainingNetwork, this->originalTraining, 5, 0.005 );
		case WTF:
			return new WTFLinkPredictor(this->trainingNetwork, this->originalTraining, 0.15 );
		case RANDOM:
			return new OneLinkPredictor(this->trainingNetwork, this->originalTraining);
	}
	return NULL;
}

std::vector<vertex_t> RecommenderResult::acceptedNodesAt(vertex_t vertexExt, int n) {
	if (this->linkPredictors.size() == 1) {
		return linkPredictors.at(0)->topNVerticesExt(vertexExt,n);
	}
	
	std::priority_queue<std::pair<double, vertex_t>> q;
	for (LinkPredictor *predictor : this->linkPredictors) {
		std::vector< std::pair<double, vertex_t> > vertexPairs = predictor->topNNormalised(vertexExt,n);
		for ( std::pair<double, vertex_t> pair : vertexPairs ) {
			q.push(pair);
		}
	}

	std::vector<vertex_t> topVertices;

	for (int i = 0; i < n; ++i) {
		topVertices.push_back(q.top().second);
		q.pop();
	}

	return topVertices;
}

RecommenderResult::RecommenderResult( const WeightedNetwork& trainingNetwork, const WeightedNetwork& originalTraining, const WeightedNetwork& testNetwork, const std::vector<int>& algorithms ) : trainingNetwork(trainingNetwork), originalTraining(originalTraining),testNetwork(testNetwork), algorithms(algorithms) {
	this->linkPredictors = std::vector<LinkPredictor*>();
	for (int algorithm : this->algorithms) {
		Recommender rec = (Recommender) algorithm;
		this->linkPredictors.push_back( this->predictorForType(rec) );
	}
}

RecommenderResult::~RecommenderResult() {
}

std::vector<double> RecommenderResult::precisionAtN(int n, int start, int end) {
	std::vector<double> precisionAtN = std::vector<double>( n );

	double proportion = 1.0/(end - start);

	// time_t timer = time(NULL); 

	for (int currentVertex = start; currentVertex < end; currentVertex++ ) {
		vertex_t currentVertexExt = this->testNetwork.translateIntToExt(currentVertex);
		
		std::vector<vertex_t> acceptedVertices = this->acceptedNodesAt(currentVertexExt,n);
		
		int currentN = 1;
  		double correct_recommendations = 0;
  		int total_correct_answers = this->testNetwork.outDegree( currentVertex );

		for ( vertex_t acceptedVertexExt : acceptedVertices ) {
			if ( this->testNetwork.hasEdgeExt(currentVertexExt, acceptedVertexExt) ) {
  				correct_recommendations++;
  			}
  			
	        double denominator = total_correct_answers < currentN ? total_correct_answers : currentN;       
	        double precision = denominator == 0 ? 0 : correct_recommendations/denominator;

	        precisionAtN.at(currentN-1) += proportion * precision;

			currentN++;
		}

		// if (currentVertex % 100 == 0) {
		// 	std::cout << currentVertex << " : " << difftime(time(NULL),timer) << "\n";
		// 	timer = time(NULL);
		// }
	}

	return precisionAtN;
}

// def top_n_mean_ensemble(k,ext_node,training_ext_nodes,link_predictors, estimator=None):
//     if len(link_predictors) == 1:
//         for l in link_predictors:
//             return l.topNVerticesExt(ext_node,k)

//     n = len(training_ext_nodes)
//     dicts = []
//     zero = 0

//     for i,link_predictor in enumerate(link_predictors):
//         metrics = {}

//         metric_sum = 0.0
//         sq_sum = 0.0

//         for check_node in training_ext_nodes:
//             metric = link_predictor.generateScoreIfNotNeighbors(ext_node,check_node)

//             if metric > 0:
//                 metric_sum += metric
//                 sq_sum += metric * metric
//                 metrics[check_node] = metric
//             else:
//                 zero += 1

//         zero = 0
//         mean = metric_sum / n
//         standard_deviation = sqrt(sq_sum / n - mean * mean)
//         dicts.append({k:LinkRecommender.z_score(metrics[k],standard_deviation,mean) for k in metrics})

//     return LinkRecommender.merge_dicts(dicts, training_ext_nodes)