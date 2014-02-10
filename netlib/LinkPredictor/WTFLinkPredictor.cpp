#include "WTFLinkPredictor.h"
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

WTFLinkPredictor::WTFLinkPredictor( const WeightedNetwork &network, const WeightedNetwork &completeNetwork, double alpha ) : LinkPredictor(network, completeNetwork), alpha(alpha), salsaNetwork(network), rootedPageRankLinkPredictor(RootedPageRankLinkPredictor( this->network, this->completeNetwork, this->alpha ))
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

        this->hubs = this->rootedPageRankLinkPredictor.hubs(vertex, 100);
        if (this->hubs.size() == 0) {
            // std::cout << "No hubs" << "\n"; 
            return 0;
        }
        this->authorities = this->rootedPageRankLinkPredictor.authorities(this->hubs);

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
            //Go to auth
            currentVertex = this->nextVertex( currentVertex, true );
            this->scores.at( currentVertex ) += 2;

            //Go to hub
            currentVertex = this->nextVertex( currentVertex, false );
            this->scores.at( currentVertex )++;

            if ( step == 100000 )
            {
                oldScores = this->scores;
            }
            else if ( step % 100000 == 0 )
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
