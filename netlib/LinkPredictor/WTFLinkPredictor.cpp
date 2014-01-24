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
        this->vertex = vertex;

        RootedPageRankLinkPredictor *predictor = new RootedPageRankLinkPredictor( this->network, this->network, this->alpha );

        // clock_t begin = clock();
        // clock_t end = clock();
        this->hubs = predictor->hubs(vertex, 100);
        this->authorities = predictor->authorities(this->hubs);
        // end = clock();
        // std::cout << "Hub/Authorities: " << double(diffclock(end, begin)) << " ms" << "\n";
        // begin = clock();
        this->salsaNetwork = this->network.salsaNetwork( this->hubs );
        // end = clock();
        // std::cout << "Salsa Net: " << double(diffclock(end, begin)) << " ms" << "\n";
        // begin = clock();

        this->scores = vector<double>( this->network.vertexCount() );
        vector<double> oldScores = vector<double>( this->network.vertexCount() );

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
           //      	end = clock();
			        // std::cout << "Converge: " << double(diffclock(end, begin)) << " ms" << "\n";
			        // begin = clock();
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