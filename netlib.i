%module netlib
%{
	#include "netlib/WeightedNetwork.h"
	#include "netlib/Statistics.h"
	#include "netlib/LinkPredictor/LinkPredictor.h"
	#include "netlib/RecommenderResult.h"   
	#include "netlib/LinkPredictor/WTFLinkPredictor.h"     
	#include "netlib/LinkPredictor/AdamicAdarLinkPredictor.h"
	#include "netlib/LinkPredictor/RootedPageRankLinkPredictor.h" 
	#include "netlib/LinkPredictor/KatzLinkPredictor.h"                    
	#include "netlib/LinkPredictor/CommonNeighborLinkPredictor.h"                              
	#include "netlib/LinkPredictor/DistanceLinkPredictor.h"               
	#include "netlib/LinkPredictor/OneLinkPredictor.h"                     
	#include "netlib/LinkPredictor/PropFlowLinkPredictor.h"                
	#include "netlib/LinkPredictor/JaccardCoefficientLinkPredictor.h"     
	#include "netlib/LinkPredictor/PreferentialAttachmentLinkPredictor.h" 
	#include "netlib/LinkPredictor/InDegreeLinkPredictor.h" 
	#include "netlib/LinkPredictor/OutDegreeLinkPredictor.h" 

	/*
	#include "netlib/LinkPredictor/JVolumeLinkPredictor.h"                 
	#include "netlib/LinkPredictor/ShortestPathCountLinkPredictor.h"
	#include "netlib/LinkPredictor/ClusteringCoefficientLinkPredictor.h"  
	#include "netlib/LinkPredictor/SimRankLinkPredictor.h"
	#include "netlib/LinkPredictor/TriangleLinkPredictor.h"
	#include "netlib/LinkPredictor/MRLPLinkPredictor.h"              
	#include "netlib/LinkPredictor/UnweightedPropFlowLinkPredictor.h"
	#include "netlib/LinkPredictor/IDegreeLinkPredictor.h"                
	#include "netlib/LinkPredictor/MutualityLinkPredictor.h"               
	#include "netlib/LinkPredictor/VCP3DirectedLinkPredictor.h"
	#include "netlib/LinkPredictor/IPageRankLinkPredictor.h"              
	#include "netlib/LinkPredictor/VCP3UndirectedLinkPredictor.h"
	#include "netlib/LinkPredictor/IVolumeLinkPredictor.h"                
	#include "netlib/LinkPredictor/PathCountLinkPredictor.h"               
	#include "netlib/LinkPredictor/VCP4DirectedLinkPredictor.h"
	#include "netlib/LinkPredictor/VCP4UndirectedLinkPredictor.h"
	#include "netlib/LinkPredictor/JDegreeLinkPredictor.h"                
	#include "netlib/LinkPredictor/WeightedRootedPageRankLinkPredictor.h"
	#include "netlib/LinkPredictor/JPageRankLinkPredictor.h"              	    
	#include "netlib/LinkPredictor/WeightedTriangleLinkPredictor.h"
	*/

%}

%include "std_vector.i"
%include "std_pair.i"
%include "std_iostream.i"

namespace std {
   %template(IntVector) vector<int>;
   %template(UnsignedIntVector) vector<unsigned int>;
   %template(PredictorVector) vector<LinkPredictor*>;
   %template(DoubleVector) vector<double>;
   %template(StringVector) vector<string>;
   %template(ConstCharVector) vector<const char*>;
}

%include "netlib/WeightedNetwork.h"
%include "netlib/Statistics.h"
%include "netlib/LinkPredictor/LinkPredictor.h"
%include "netlib/RecommenderResult.h"   
%include "netlib/LinkPredictor/WTFLinkPredictor.h"     
%include "netlib/LinkPredictor/AdamicAdarLinkPredictor.h"
%include "netlib/LinkPredictor/RootedPageRankLinkPredictor.h" 
%include "netlib/LinkPredictor/KatzLinkPredictor.h"                    
%include "netlib/LinkPredictor/CommonNeighborLinkPredictor.h"                              
%include "netlib/LinkPredictor/DistanceLinkPredictor.h"               
%include "netlib/LinkPredictor/OneLinkPredictor.h"                     
%include "netlib/LinkPredictor/PropFlowLinkPredictor.h"                
%include "netlib/LinkPredictor/JaccardCoefficientLinkPredictor.h"     
%include "netlib/LinkPredictor/PreferentialAttachmentLinkPredictor.h"
%include "netlib/LinkPredictor/InDegreeLinkPredictor.h"
%include "netlib/LinkPredictor/OutDegreeLinkPredictor.h"

/*
%include "netlib/LinkPredictor/JVolumeLinkPredictor.h"                 
%include "netlib/LinkPredictor/ShortestPathCountLinkPredictor.h"
%include "netlib/LinkPredictor/ClusteringCoefficientLinkPredictor.h"  
%include "netlib/LinkPredictor/SimRankLinkPredictor.h"
%include "netlib/LinkPredictor/TriangleLinkPredictor.h"
%include "netlib/LinkPredictor/MRLPLinkPredictor.h"              
%include "netlib/LinkPredictor/UnweightedPropFlowLinkPredictor.h"
%include "netlib/LinkPredictor/IDegreeLinkPredictor.h"                
%include "netlib/LinkPredictor/MutualityLinkPredictor.h"               
%include "netlib/LinkPredictor/VCP3DirectedLinkPredictor.h"
%include "netlib/LinkPredictor/IPageRankLinkPredictor.h"              
%include "netlib/LinkPredictor/VCP3UndirectedLinkPredictor.h"
%include "netlib/LinkPredictor/IVolumeLinkPredictor.h"                
%include "netlib/LinkPredictor/PathCountLinkPredictor.h"               
%include "netlib/LinkPredictor/VCP4DirectedLinkPredictor.h"
%include "netlib/LinkPredictor/VCP4UndirectedLinkPredictor.h"
%include "netlib/LinkPredictor/JDegreeLinkPredictor.h"                
%include "netlib/LinkPredictor/WeightedRootedPageRankLinkPredictor.h"
%include "netlib/LinkPredictor/JPageRankLinkPredictor.h"              	    
%include "netlib/LinkPredictor/WeightedTriangleLinkPredictor.h"
*/

