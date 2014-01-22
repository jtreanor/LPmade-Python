#include "WTFLinkPredictor.h"
#include "RootedPageRankLinkPredictor.h"
#include <iterator>
#include "../Statistics.h"

vertex_t WTFLinkPredictor::nextVertex( vertex_t currentVertex, bool isHub ) {
	const neighbor_set_t& neighbors = this->salsaNetwork.outNeighbors( currentVertex );

	if( neighbors.size() < 1 || (double)rand()/RAND_MAX < this->alpha ) {
		return isHub ? this->randomAuth() : this->randomHub();		
	}

	vertex_t randomNeighbor = rand() % neighbors.size();
	return neighbors.at( randomNeighbor ).first;
}

vertex_t WTFLinkPredictor::randomHub( ) {
	int random = rand() % this->hubs.size();
	return this->hubs.at( random );
}

vertex_t WTFLinkPredictor::randomAuth( ) {
	int random = rand() % this->authorities.size();
	return this->authorities.at( random );
}

WTFLinkPredictor::WTFLinkPredictor( const WeightedNetwork& network, const WeightedNetwork& completeNetwork, double alpha ) : LinkPredictor(network,completeNetwork), alpha(alpha), salsaNetwork(network) {
}

WTFLinkPredictor::~WTFLinkPredictor() {
}

double WTFLinkPredictor::generateScore( unsigned int vertex, unsigned int neighbor ) {
	if( this->vertex != vertex ) {
		this->vertex = vertex;

		RootedPageRankLinkPredictor* predictor = new RootedPageRankLinkPredictor( this->network, this->network, this->alpha );

		time_t timer = time(NULL);

		this->hubs = predictor->hubs(vertex,100);
		this->authorities = predictor->authorities(this->hubs);
		this->salsaNetwork = this->network.salsaNetwork( this->hubs );

		std::cout << difftime(time(NULL),timer) << "\n";
		timer = time(NULL);

		this->scores = vector<double>( this->network.vertexCount() );
		vector<double> oldScores = vector<double>( this->network.vertexCount() );		

		int random = rand() % this->hubs.size();
		vertex_t currentHubVertex = this->hubs.at( random );
		this->scores.at( currentHubVertex )++;

		for( unsigned int step = 1; true; step++ ) {
			const neighbor_set_t& neighbors = this->salsaNetwork.outNeighbors( currentHubVertex );

			if( neighbors.size() < 1 || (double)rand()/RAND_MAX < this->alpha ) {
				int randomAuth = rand() % this->authorities.size();
				currentHubVertex = this->authorities.at( randomAuth );		
			} else {
				vertex_t randomNeighbor = rand() % neighbors.size();
				currentHubVertex = neighbors.at( randomNeighbor ).first;
			}

			const neighbor_set_t& hubNeighbors = this->salsaNetwork.outNeighbors( currentHubVertex );

			if( hubNeighbors.size() < 1 || (double)rand()/RAND_MAX < this->alpha ) {
				int randomHub = rand() % this->hubs.size();
				currentHubVertex = this->hubs.at( randomHub );	
			} else {
				vertex_t randomNeighbor = rand() % hubNeighbors.size();
				currentHubVertex = hubNeighbors.at( randomNeighbor ).first;
			}

			// //Two steps
			// currentHubVertex = nextVertex( nextVertex(currentHubVertex, true), false );
			this->scores.at( currentHubVertex )++;
			
			if( step == 100000 ) {
				std::cout << difftime(time(NULL),timer) << "\n";
				timer = time(NULL);
				oldScores = this->scores;
			} else if( step % 100000 == 0 ) {
				std::cout << difftime(time(NULL),timer) << "\n";
				timer = time(NULL);
				double r = Statistics<double>::sampleCorrelationCoefficient( oldScores, this->scores );
				if( r > 0.9999 ) {
					std::cout << "converged" <<  "\n";
					return this->scores.at( neighbor );
				} else {
					oldScores = this->scores;
				}
			}
		}
	}
	return this->scores.at( neighbor );
}