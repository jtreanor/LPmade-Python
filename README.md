#LPmade Python wrapper

This project provides a simple SWIG wrapper for the excellent network library provided as part of [LPmade](https://github.com/rlichtenwalter/LPmade). LPmade is an extensive software solution for link prediction and related tasks. 

Although the SWIG swrapper was made for Python is should be easy to modify for any of SWIG's supported languages.

I created the wrapper to use LPmade through Python as part of my final year project for Computer Science at UCC.

##Usage

To install the Python module simply run the following command:

``sudo python setup.py install``

The API is largely identical to the C++ library. For example:

```from netlib import *

#Load network file from disk.
network = WeightedNetwork.readNetworkNamed("COMPLETE.net")

#Calculate the Katz measure
katz = KatzLinkPredictor(network)
katz_measure = katz.generateScore(1,10)```

## License

LPmade as well as the Python wrapper are released under the GNU GENERAL PUBLIC LICENSE Version 3. That license is available in the file './gpl-3.0.txt'.