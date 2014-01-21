#include "WTFLinkPredictor.h"
#include "RootedPageRankLinkPredictor.h"
#include <iterator>
#include "../Statistics.h"

WTFLinkPredictor::WTFLinkPredictor( const WeightedNetwork& network, const WeightedNetwork& completeNetwork,double alpha ) : LinkPredictor(network,completeNetwork), alpha(alpha) {
}

WTFLinkPredictor::~WTFLinkPredictor() {
}

double WTFLinkPredictor::generateScore( unsigned int vertex, unsigned int neighbor ) {
	if( this->vertex != vertex ) {
		this->vertex = vertex;

		RootedPageRankLinkPredictor* predictor = new RootedPageRankLinkPredictor( this->network, this->network, this->alpha );
		std::set<vertex_t> hubs = predictor->circleOfTrust(vertex,50);
		WeightedNetwork salsaNetwork = this->network.salsaNetwork( hubs );

		this->scores = vector<double>( this->network.vertexCount() );
		vector<double> oldScores = vector<double>( this->network.vertexCount() );		
		vertex_t currentHubVertex = 0;//* std::next(hubs.begin(), rand() % hubs.size());
		this->scores.at( currentHubVertex )++;

		for( unsigned int step = 1; true; step++ ) {
			//Go to random authority
			const neighbor_set_t& authNeighbors = salsaNetwork.outNeighbors( currentHubVertex );

			if( authNeighbors.size() < 1 || (double)rand()/RAND_MAX < this->alpha ) {
				currentHubVertex = 0;//* std::next(hubs.begin(), rand() % hubs.size());
			} else {
				vertex_t randomNeighbor = rand() % authNeighbors.size();
				currentHubVertex = authNeighbors.at( randomNeighbor ).first;

				//Got to random trusted vertex
				const neighbor_set_t& trustNeighbors = salsaNetwork.outNeighbors( currentHubVertex );
				randomNeighbor = rand() % trustNeighbors.size();
				currentHubVertex = trustNeighbors.at( randomNeighbor ).first;
			}

			this->scores.at( currentHubVertex )++;
			
			if( step == 100000 ) {
				oldScores = this->scores;
			} else if( step % 100000 == 0 ) {
				std::cout << "Check correlation" << "\n";
				double r = Statistics<double>::sampleCorrelationCoefficient( oldScores, this->scores );
				if( r > 0.9999 ) {
					std::cout << "correlated" << "\n";
					return this->scores.at( neighbor );
				} else {
					oldScores = this->scores;
				}
			}
		}
	}
	return this->scores.at( neighbor );
}