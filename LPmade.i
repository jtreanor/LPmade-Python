%module LPmade
%{
	#define SWIG_FILE_WITH_INIT

	#include "netlib/WeightedNetwork.h"
	#include "netlib/Statistics.h"
	#include "netlib/AlgorithmManager.h"
	#include "netlib/LinkPredictorEnsemble.h"   
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
	#include "netlib/LinkPredictor/UnweightedPropFlowLinkPredictor.h"
	#include "netlib/LinkPredictor/TriangleLinkPredictor.h"
	#include "netlib/LinkPredictor/SimRankLinkPredictor.h"
	#include "netlib/LinkPredictor/MutualityLinkPredictor.h" 

	/*
	#include "netlib/LinkPredictor/JVolumeLinkPredictor.h"                 
	#include "netlib/LinkPredictor/ShortestPathCountLinkPredictor.h"
	#include "netlib/LinkPredictor/ClusteringCoefficientLinkPredictor.h"  
	#include "netlib/LinkPredictor/MRLPLinkPredictor.h"              
	#include "netlib/LinkPredictor/IDegreeLinkPredictor.h"                
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

// Get the NumPy typemaps
%include "numpy.i"

 // Get the STL typemaps
%include "stl.i"
%include "std_iostream.i"

// Handle standard exceptions
%include "exception.i"
%exception
{
  try
  {
    $action
  }
  catch (const std::invalid_argument& e)
  {
    SWIG_exception(SWIG_ValueError, e.what());
  }
  catch (const std::out_of_range& e)
  {
    SWIG_exception(SWIG_IndexError, e.what());
  }
}
%init %{
  import_array();
%}

// Apply the 1D NumPy typemaps
%apply (int DIM1  , long* INPLACE_ARRAY1)
      {(int length, long* data          )};
%apply (long** ARGOUTVIEW_ARRAY1, int* DIM1  )
      {(long** data             , int* length)};

// Apply the 2D NumPy typemaps
%apply (int DIM1 , int DIM2 , long* INPLACE_ARRAY2)
      {(int nrows, int ncols, long* data          )};
%apply (int* DIM1 , int* DIM2 , long** ARGOUTVIEW_ARRAY2)
      {(int* nrows, int* ncols, long** data             )};

%inline %{
void create_list(int net, double *arr){
  for (int i=0; i<net; i++)
    arr[i] = i;
}
%}

namespace std {
   %template(IntVector) vector<int>;
   %template(LongVector) vector<long>;
   %template(UnsignedIntVector) vector<unsigned int>;
   %template(PredictorVector) vector<LinkPredictor*>;
   %template(DoubleVector) vector<double>;
   %template(StringVector) vector<string>;
   %template(ConstCharVector) vector<const char*>;
}

%include "netlib/WeightedNetwork.h"
%include "netlib/Statistics.h"
%include "netlib/AlgorithmManager.h"
%include "netlib/LinkPredictorEnsemble.h"
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
%include "netlib/LinkPredictor/UnweightedPropFlowLinkPredictor.h"
%include "netlib/LinkPredictor/TriangleLinkPredictor.h"
%include "netlib/LinkPredictor/SimRankLinkPredictor.h"
%include "netlib/LinkPredictor/MutualityLinkPredictor.h"  

/*
%include "netlib/LinkPredictor/JVolumeLinkPredictor.h"                 
%include "netlib/LinkPredictor/ShortestPathCountLinkPredictor.h"
%include "netlib/LinkPredictor/ClusteringCoefficientLinkPredictor.h"  
%include "netlib/LinkPredictor/MRLPLinkPredictor.h"              
%include "netlib/LinkPredictor/IDegreeLinkPredictor.h"                             
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

