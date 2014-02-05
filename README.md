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
from netlib import *

#Load network from disk
network = WeightedNetwork.readNetworkNamed("COMPLETE.net")

#Calculate Katz measure between vertices 1 and 10
katz = KatzLinkPredictor(network,network,5,0.005)
katz_measure = katz.generateScore(1,10)

#Get the top 50 recommened vertices for vertex 0 using Katz
recommendations = katz.topNVertices(0,50)
```

## License

LPmade is released under the GNU GENERAL PUBLIC LICENSE Version 3. That license is available in the file './gpl-3.0.txt'.
