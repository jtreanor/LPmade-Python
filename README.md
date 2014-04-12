#LPmade Python wrapper

This project provides a simple SWIG wrapper for the excellent network library provided as part of [LPmade](https://github.com/rlichtenwalter/LPmade). LPmade is an extensive software solution for link prediction and related tasks. 

Although the SWIG wrapper was made for Python it should be easy to modify for any of SWIG's supported languages.

I created the wrapper to use LPmade through Python as part of my final year project for Computer Science at UCC.

Please contact me if you have any questions at all.

##Features

All link prediction algorithms in the [netlib/LinkPredictor](netlib/LinkPredictor) directory can be used through the Python module.

In addition to adjusting the LPmade library to be used with SWIG, I made the following additions:

- LPmade is designed to be evaluated using threshold curves, which evaluate all possible links to be created. The experiments in my projects focussed on recommending the top N nodes. For this reason I changed LPmade from being a predictor to a top N recommender. Care was taken to ensure that ties were broken randomly.
- I implemented and added support for unsupervised ensembles. These are Borda count and weighted mean ensembles. See the example below.
- I implemented the SALSA (Twitter WTF), Out Degree and In Degree algorithms.
- I added a modular evaluation system that generates recommendations and calculates the precision at N for a given range of nodes (See the RecommenderResult class). This allows the evaluation to be divided into as many individual chunks as desired. This is useful for parallelisation of experiments.

##Usage

There are no dependencies apart from SWIG and a C++11 compiler.

To install the Python module simply run the following command:

``sudo python setup.py install``

Or:

``sudo python3 setup.py install``

The API is largely identical to the C++ library, with several useful additions. For example:

```
from LPmade import *

#Load network in LPmade format from disk
network = WeightedNetwork.readNetworkNamed("COMPLETE.net")

test_network = WeightedNetwork.readNetworkNamed("TEST.net")
training_network = network.removeEdges(test_network)

#Calculate Katz measure between vertices 1 and 10
katz = KatzLinkPredictor(training_network,training_network,5,0.005)
katz_measure = katz.generateScore(1,10)

#Get the top 50 recommened vertices for vertex 1 using Katz
recommendations = katz.topNVerticesExt(1,50)

alg = AlgorithmManager(training_network, 
                       IntVector(AlgorithmManager.COMMON_NEIGHBOURS,
                                 AlgorithmManager.GRAPH_DISTANCE),
                       IntVector(AlgorithmManager.DIRECTED,
                                 AlgorithmManager.UNDIRECTED))

#Create a mean ensemble predictor using common neighbours and graph distance
ensemble = LinkPredictorEnsemble(training_network,
                                 DoubleVector(1.0,2.0), #Weights
                                 IntVector(0,0), #Don't use neighbourhood limiting
                                 LinkPredictorEnsemble.MEAN,
                                 alg)

#Recommend 10 nodes for node '1' using the ensemble
topTen = ensemble.topNVerticesExt(1,10)

#Evaluate Rooted PageRank using hold out to give with precision at 10.
recommenderResult = RecommenderResult( test_network,
                                       ensemble )
precision_at_10 = recommenderResult.evaluate(10,0,test_network.vertexCount() )
```

## License

LPmade and the additions in this repositpry are released under the GNU GENERAL PUBLIC LICENSE Version 3. That license is available in the file './gpl-3.0.txt'.
