"""
setup.py file for LPmade netlib
"""

from distutils.core import setup, Extension
from distutils.sysconfig import get_config_vars
import os

(opt,) = get_config_vars('OPT')
os.environ['OPT'] = " ".join(
    flag for flag in opt.split() if flag != '-Wstrict-prototypes'
)

net_lib_module = Extension('_LPmade',
                           sources=['LPmade.i',
                           			"netlib/WeightedNetwork.cpp",
									"netlib/Statistics.cpp",
									"netlib/Algorithm.cpp",
									"netlib/LinkPredictorEnsemble.cpp",
									"netlib/LinkPredictor/LinkPredictor.cpp",
									"netlib/RecommenderResult.cpp",   
									"netlib/LinkPredictor/WTFLinkPredictor.cpp",     
									"netlib/LinkPredictor/AdamicAdarLinkPredictor.cpp",
									"netlib/LinkPredictor/RootedPageRankLinkPredictor.cpp", 
									"netlib/LinkPredictor/KatzLinkPredictor.cpp",                    
									"netlib/LinkPredictor/CommonNeighborLinkPredictor.cpp",                              
									"netlib/LinkPredictor/DistanceLinkPredictor.cpp",               
									"netlib/LinkPredictor/OneLinkPredictor.cpp",                     
									"netlib/LinkPredictor/PropFlowLinkPredictor.cpp",                
									"netlib/LinkPredictor/JaccardCoefficientLinkPredictor.cpp",     
									"netlib/LinkPredictor/PreferentialAttachmentLinkPredictor.cpp",
									"netlib/LinkPredictor/InDegreeLinkPredictor.cpp",
									"netlib/LinkPredictor/OutDegreeLinkPredictor.cpp", 
									# "netlib/LinkPredictor/JVolumeLinkPredictor.cpp",                 
									# "netlib/LinkPredictor/ShortestPathCountLinkPredictor.cpp",
									# "netlib/LinkPredictor/ClusteringCoefficientLinkPredictor.cpp",  
									# "netlib/LinkPredictor/SimRankLinkPredictor.cpp",
									# "netlib/LinkPredictor/TriangleLinkPredictor.cpp",
									# "netlib/LinkPredictor/MRLPLinkPredictor.cpp",              
									"netlib/LinkPredictor/UnweightedPropFlowLinkPredictor.cpp",
									# "netlib/LinkPredictor/IDegreeLinkPredictor.cpp",                
									# "netlib/LinkPredictor/MutualityLinkPredictor.cpp",               
									# "netlib/LinkPredictor/VCP3DirectedLinkPredictor.cpp",
									# "netlib/LinkPredictor/IPageRankLinkPredictor.cpp",              
									# "netlib/LinkPredictor/VCP3UndirectedLinkPredictor.cpp",
									# "netlib/LinkPredictor/IVolumeLinkPredictor.cpp",                
									# "netlib/LinkPredictor/PathCountLinkPredictor.cpp",               
									# "netlib/LinkPredictor/VCP4DirectedLinkPredictor.cpp",
									# "netlib/LinkPredictor/VCP4UndirectedLinkPredictor.cpp",
									# "netlib/LinkPredictor/JDegreeLinkPredictor.cpp",                
									# "netlib/LinkPredictor/WeightedRootedPageRankLinkPredictor.cpp",
									# "netlib/LinkPredictor/JPageRankLinkPredictor.cpp",              	    
									# "netlib/LinkPredictor/WeightedTriangleLinkPredictor.cpp",
									],
			                       swig_opts=['-c++'],
			                       extra_compile_args=["-std=c++0x"],
                           )

setup (name = '	LPmade',
       version = '0.1',
       author      = "James Treanor",
       description = """LPmade wrapper""",
       ext_modules = [net_lib_module],
       py_modules = ["LPmade"],
       )
