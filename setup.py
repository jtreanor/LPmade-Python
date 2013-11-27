#!/usr/bin/env python3

"""
setup.py file for SWIG example
"""

from distutils.core import setup, Extension


net_lib_module = Extension('_netlib',
                           sources=['netlib.i','netlib/WeightedNetwork.cpp','netlib/Statistics.cpp',
			                       	"netlib/LinkPredictor/AdamicAdarLinkPredictor.cpp",             
									"netlib/LinkPredictor/JVolumeLinkPredictor.cpp",                 
									"netlib/LinkPredictor/ShortestPathCountLinkPredictor.cpp",
									"netlib/LinkPredictor/ClusteringCoefficientLinkPredictor.cpp",  
									"netlib/LinkPredictor/KatzLinkPredictor.cpp",                    
									"netlib/LinkPredictor/SimRankLinkPredictor.cpp",
									"netlib/LinkPredictor/CommonNeighborLinkPredictor.cpp",         
									"netlib/LinkPredictor/LinkPredictor.cpp",                        
									"netlib/LinkPredictor/TriangleLinkPredictor.cpp",
									"netlib/LinkPredictor/DistanceLinkPredictor.cpp",               
									"netlib/LinkPredictor/MRLPLinkPredictor.cpp",                    
									"netlib/LinkPredictor/UnweightedPropFlowLinkPredictor.cpp",
									"netlib/LinkPredictor/IDegreeLinkPredictor.cpp",                
									"netlib/LinkPredictor/MutualityLinkPredictor.cpp",               
									"netlib/LinkPredictor/VCP3DirectedLinkPredictor.cpp",
									"netlib/LinkPredictor/IPageRankLinkPredictor.cpp",              
									"netlib/LinkPredictor/OneLinkPredictor.cpp",                     
									"netlib/LinkPredictor/VCP3UndirectedLinkPredictor.cpp",
									"netlib/LinkPredictor/IVolumeLinkPredictor.cpp",                
									"netlib/LinkPredictor/PathCountLinkPredictor.cpp",               
									"netlib/LinkPredictor/VCP4DirectedLinkPredictor.cpp",
									"netlib/LinkPredictor/JaccardCoefficientLinkPredictor.cpp",     
									"netlib/LinkPredictor/PreferentialAttachmentLinkPredictor.cpp",  
									"netlib/LinkPredictor/VCP4UndirectedLinkPredictor.cpp",
									"netlib/LinkPredictor/JDegreeLinkPredictor.cpp",                
									"netlib/LinkPredictor/PropFlowLinkPredictor.cpp",                
									"netlib/LinkPredictor/WeightedRootedPageRankLinkPredictor.cpp",
									"netlib/LinkPredictor/JPageRankLinkPredictor.cpp",              
									"netlib/LinkPredictor/RootedPageRankLinkPredictor.cpp",          
									"netlib/LinkPredictor/WeightedTriangleLinkPredictor.cpp"],
			                       swig_opts=['-c++'],
                           )

setup (name = 'netlib',
       version = '0.1',
       author      = "James Treanor",
       description = """netlib wrapper""",
       ext_modules = [net_lib_module],
       py_modules = ["netlib"],
       )