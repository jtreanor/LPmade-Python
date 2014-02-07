#LPmade Python wrapper

This project provides a simple SWIG wrapper for the excellent network library provided as part of [LPmade](https://github.com/rlichtenwalter/LPmade). LPmade is an extensive software solution for link prediction and related tasks. 

Although the SWIG swrapper was made for Python is should be easy to modify for any of SWIG's supported languages.

I created the wrapper to use LPmade through Python as part of my final year project for Computer Science at UCC.

##Usage

To install the Python module simply run the following command:

``sudo python setup.py install``

Or:

``sudo python3 setup.py install``

The API is largely identical to the C++ library, with a few useful additions. For example:

```
from LPmade import *

#Load network from disk
network = WeightedNetwork.readNetworkNamed("COMPLETE.net")

#Calculate Katz measure between vertices 1 and 10
katz = KatzLinkPredictor(network,network,5,0.005)
katz_measure = katz.generateScore(1,10)

#Get the top 50 recommened vertices for vertex 1 using Katz
recommendations = katz.topNVerticesExt(1,50)

#Create a mean ensemble predictor using common neighbours and graph distance
ensemble = EnsembleLinkPredictor(network, network, IntVector(EnsembleLinkPredictor.COMMON_NEIGHBOURS, EnsembleLinkPredictor.GRAPH_DISTANCE) )
topTen = ensemble.topNVerticesExt(1,10)

#Evaluate Rooted PageRank using hold out using precision at N up to 50.
test_network = WeightedNetwork.readNetworkNamed("TEST.net")
training_network = network.removeEdges(test_network)

recommenderResult = RecommenderResult( training_network, training_network, test_network, IntVector(EnsembleLinkPredictor.ROOTED_PAGE_RANK) )
precision_at_n = recommenderResult.precisionAtN(50,0,test_network.vertexCount() )
```

## License

LPmade is released under the GNU GENERAL PUBLIC LICENSE Version 3. That license is available in the file './gpl-3.0.txt'.
