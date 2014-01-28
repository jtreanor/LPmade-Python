#include "WTFLinkPredictor.h"
#include "RootedPageRankLinkPredictor.h"
// #include <time>
#include "../Statistics.h"


double diffclock(clock_t clock1, clock_t clock2)
{
    double diffticks = clock1 - clock2;
    double diffms = (diffticks * 1000) / CLOCKS_PER_SEC;
    return diffms;
}

vertex_t WTFLinkPredictor::nextVertex( vertex_t currentVertex, bool isHub )
{
    const neighbor_set_t &neighbors = this->salsaNetwork.outNeighbors( currentVertex );

    if ( neighbors.size() < 1 || (double)rand() / RAND_MAX < this->alpha )
    {
        return isHub ? this->randomAuth() : this->randomHub();
    }

    vertex_t randomNeighbor = rand() % neighbors.size();
    return neighbors.at( randomNeighbor ).first;
}

vertex_t WTFLinkPredictor::randomHub( )
{
    int random = rand() % this->hubs.size();
    return this->hubs.at( random );
}

vertex_t WTFLinkPredictor::randomAuth( )
{
    int random = rand() % this->authorities.size();
    return this->authorities.at( random );
}

WTFLinkPredictor::WTFLinkPredictor( const WeightedNetwork &network, const WeightedNetwork &completeNetwork, double alpha ) : LinkPredictor(network, completeNetwork), alpha(alpha), salsaNetwork(network)
{
}

WTFLinkPredictor::~WTFLinkPredictor()
{
}

double WTFLinkPredictor::generateScore( unsigned int vertex, unsigned int neighbor )
{
    if ( this->vertex != vertex )
    {
        // std::cout << "Start" << "\n"; 

        this->vertex = vertex;

        this->scores = vector<double>( this->network.vertexCount(), 1.0 );
        vector<double> oldScores = vector<double>( this->network.vertexCount(), 1.0 );

        RootedPageRankLinkPredictor *predictor = new RootedPageRankLinkPredictor( this->completeNetwork, this->completeNetwork, this->alpha );

        this->hubs = predictor->hubs(vertex, 100);
        if (this->hubs.size() == 0) {
            // std::cout << "No hubs" << "\n"; 
            return 0;
        }
        this->authorities = predictor->authorities(this->hubs);
        if (this->authorities.size() == 0) {
            // std::cout << "No auths" << "\n";
            return 0;
        }
        this->salsaNetwork = this->network.salsaNetwork( this->hubs );

        // Start at random hub
        vertex_t currentVertex = this->randomHub();
        this->scores.at( currentVertex )++;

        for ( unsigned int step = 1; true; step++ )
        {
        	for(vertex_t hub: this-> hubs) {
                double nbSum = 0.0;
                // // Update the degree because not all authorities were selected
                const neighbor_set_t &neighbors = this->salsaNetwork.outNeighbors( hub );
                for(neighbor_t auth_neighbor : neighbors) {
                    nbSum += this->scores.at(auth_neighbor.first) / this->salsaNetwork.outDegree( auth_neighbor.first );
                }
                // if (step > 1) {
                //     std::cout << hub << " Score : " << nbSum << " Out Degree: " << this->salsaNetwork.outDegree( hub ) << "\n";
                // }
                
                this->scores.at(hub) = nbSum;
            }

            for(vertex_t auth: this->authorities) {
                this->scores.at(auth) = 0;
            }

            for(vertex_t hub: this-> hubs) {
                double myContribution = this->scores.at(hub) / this->salsaNetwork.outDegree(hub);
                // std::cout << myContribution << "\n";

                const neighbor_set_t &neighbors = this->salsaNetwork.outNeighbors( hub );
                for(neighbor_t auth_neighbor : neighbors) {
                    this->scores.at(auth_neighbor.first) += myContribution;
                }
            }

            if ( step == 1 )
            {
                oldScores = this->scores;
            }
            else if ( step % 2 == 0 )
            {
                double r = Statistics<double>::sampleCorrelationCoefficient( oldScores, this->scores );
                // std::cout << r << "\n";
                if ( r > 0.9999 )
                {
                    return this->scores.at( neighbor );
                }
                else
                {
                    oldScores = this->scores;
                }
            }
        }
    }
    return this->scores.at( neighbor );
}
