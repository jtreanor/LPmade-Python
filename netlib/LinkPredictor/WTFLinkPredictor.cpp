#include "WTFLinkPredictor.h"
#include "RootedPageRankLinkPredictor.h"
#include "CommonNeighborLinkPredictor.h"
#include "../Statistics.h"
#include <algorithm>
#include <tuple>

WTFLinkPredictor::WTFLinkPredictor( const WeightedNetwork &network, const WeightedNetwork &completeNetwork, int hubSize ) : LinkPredictor(network, completeNetwork), hubSize(hubSize), salsaNetwork(network)
{
    hubPredictor = new CommonNeighborLinkPredictor( this->network, this->completeNetwork );
}

WTFLinkPredictor::~WTFLinkPredictor()
{
    delete this->hubPredictor;
}

std::vector<vertex_t> WTFLinkPredictor::generateHubs(unsigned int vertex, int n) {
    return this->hubPredictor->topNVertices(vertex,n);
}

std::vector<vertex_t> WTFLinkPredictor::generateAuthorities() {
    std::vector<vertex_t> authorities;

    for (vertex_t hub : hubs) {
        const neighbor_set_t& neighbors = this->network.outNeighbors( hub );
        for (neighbor_t neighbor : neighbors) {
            if(! (std::find(hubs.begin(), hubs.end(), neighbor.first) != hubs.end()) ) {
                authorities.push_back(neighbor.first);
            }
        }
    }

    return authorities;
}

double WTFLinkPredictor::generateScore( unsigned int vertex, unsigned int neighbor )
{
    if ( this->vertex != vertex )
    {
        this->vertex = vertex;

        this->scores = vector<double>( this->network.vertexCount(), 1.0 );
        vector<double> oldScores = vector<double>( this->network.vertexCount(), 1.0 );

        this->hubs = this->generateHubs(vertex, this->hubSize );
        if (this->hubs.size() == 0) {
            return 0;
        }
        this->authorities = this->generateAuthorities();

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
