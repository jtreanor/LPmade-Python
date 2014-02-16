#include "WTFLinkPredictor.h"
#include "../Statistics.h"

WTFLinkPredictor::WTFLinkPredictor( const WeightedNetwork &network, const WeightedNetwork &completeNetwork, double alpha, int hubSize ) : LinkPredictor(network, completeNetwork), alpha(alpha), hubSize(hubSize), salsaNetwork(network), rootedPageRankLinkPredictor(RootedPageRankLinkPredictor( this->network, this->completeNetwork, this->alpha ))
{
}

WTFLinkPredictor::~WTFLinkPredictor()
{
}

double WTFLinkPredictor::generateScore( unsigned int vertex, unsigned int neighbor )
{
    if ( this->vertex != vertex )
    {
        this->vertex = vertex;

        this->scores = vector<double>( this->network.vertexCount(), 1.0 );
        vector<double> oldScores = vector<double>( this->network.vertexCount(), 1.0 );

        this->hubs = this->rootedPageRankLinkPredictor.hubs(vertex, this->hubSize );
        if (this->hubs.size() == 0) {
            return 0;
        }
        this->authorities = this->rootedPageRankLinkPredictor.authorities(this->hubs);

        if (this->authorities.size() == 0) {
            return 0;
        }
        this->salsaNetwork = this->network.salsaNetwork( this->hubs );

        for ( unsigned int step = 1; true; step++ )
        {
            for(vertex_t hub: this-> hubs) {
                double nbSum = 0.0;
                const neighbor_set_t &neighbors = this->salsaNetwork.outNeighbors( hub );
                for(neighbor_t auth_neighbor : neighbors) {
                    nbSum += this->scores.at(auth_neighbor.first) / this->salsaNetwork.outDegree( auth_neighbor.first );
                }
                    
                this->scores.at(hub) = nbSum;
            }

            for(vertex_t auth: this->authorities) {
                this->scores.at(auth) = 0;
            }

            for(vertex_t hub: this-> hubs) {
                double myContribution = this->scores.at(hub) / this->salsaNetwork.outDegree(hub);
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
