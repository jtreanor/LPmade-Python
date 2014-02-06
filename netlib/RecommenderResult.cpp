#include "RecommenderResult.h"
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
#include <queue>
#include <tuple>

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
		case IN_DEGREE:
			return new InDegreeLinkPredictor(this->trainingNetwork, this->originalTraining );
		case RANDOM:
			return new OneLinkPredictor(this->trainingNetwork, this->originalTraining);
	}
	return NULL;
}

std::vector<vertex_t> RecommenderResult::acceptedNodesAt(vertex_t vertexExt, int n) {
	if (this->linkPredictors.size() == 1) {
		return linkPredictors.at(0)->topNVerticesExt(vertexExt,n);
	}
	
	std::vector<double> scoresA = linkPredictors.at(0)->allNormalised(vertexExt);
	std::vector<double> scoresB = linkPredictors.at(1)->allNormalised(vertexExt);

	std::priority_queue<std::tuple<double, int ,int>> q;

	for (int i = 0; i < this->trainingNetwork.vertexCount(); i++) {
		double average = ( scoresA.at(i) + scoresB.at(i) ) / 2.0;
		q.push( std::make_tuple( average, rand(), i ) );
	}

	std::vector<vertex_t> topVertices;

	for (int i = 0; i < n; ++i) {
		topVertices.push_back( this->trainingNetwork.translateIntToExt( std::get<2>(q.top()) ) );
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
	srand(time(0));
}

RecommenderResult::~RecommenderResult() {
}

std::vector<double> RecommenderResult::precisionAtN(int n, int start, int end) {
	std::vector<double> precisionAtN = std::vector<double>( n );

	double proportion = 1.0/(end - start);

	// int totalInDegree = 0;
	// float degreeCount = 0;

	for (int currentVertex = start; currentVertex < end; currentVertex++ ) {
		vertex_t currentVertexExt = this->testNetwork.translateIntToExt(currentVertex);
		
		std::vector<vertex_t> acceptedVertices = this->acceptedNodesAt(currentVertexExt,n);

		// for (vertex_t degreeVertex : acceptedVertices) {
		// 	totalInDegree += this->trainingNetwork.inDegree(this->trainingNetwork.translateExtToInt(degreeVertex));
		// 	degreeCount++;
		// }
		
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
	}

	// std::cout << "Average In Degree: " << totalInDegree / degreeCount << "\n";

	// precisionAtN.at(0) = totalInDegree / degreeCount;

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