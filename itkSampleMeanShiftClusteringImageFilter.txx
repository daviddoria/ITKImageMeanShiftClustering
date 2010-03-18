  /*=========================================================================

    Program:   Insight Segmentation & Registration Toolkit
    Module:    $RCSfile: itkScalarImageKmeansImageFilter.txx,v $
    Language:  C++
    Date:      $Date: 2009-05-02 05:43:55 $
    Version:   $Revision: 1.12 $

    Copyright (c) Insight Software Consortium. All rights reserved.
    See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

       This software is distributed WITHOUT ANY WARRANTY; without even 
       the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
       PURPOSE.  See the above copyright notices for more information.

  =========================================================================*/
#ifndef __itkSampleMeanShiftClusteringImageFilter_txx
#define __itkSampleMeanShiftClusteringImageFilter_txx

#include "itkSampleMeanShiftClusteringImageFilter.h"

#include "itkScalarImageToListAdaptor.h"

#include "itkImageFileReader.h"
#include "itkImageRegionIterator.h"
#include "itkScalarImageToListAdaptor.h"
#include "itkKdTree.h"
#include "itkKdTreeGenerator.h"
#include "itkMeanShiftModeCacheMethod.h"
#include "itkHypersphereKernelMeanShiftModeSeeker.h"
#include "itkSampleMeanShiftBlurringFilter.h"
#include "itkSampleMeanShiftClusteringFilter.h"

namespace itk
{
  
template< class TInputImage >
void
SampleMeanShiftClusteringImageFilter< TInputImage >
::GenerateData()
{   
  typedef itk::Statistics::ScalarImageToListAdaptor< TInputImage >
    ListSampleType ;
  
  ListSampleType::Pointer listSample = 
    ListSampleType::New() ;
  listSample->SetImage( this->GetInput() ) ;

  typedef itk::Statistics::KdTreeGenerator< ListSampleType > 
    TreeGeneratorType ;
  TreeGeneratorType::Pointer treeGenerator = TreeGeneratorType::New() ;
  treeGenerator->SetSample( listSample ) ;
  treeGenerator->SetBucketSize( 200 ) ;
  treeGenerator->Update() ;

  typedef TreeGeneratorType::KdTreeType TreeType ;
  TreeType::Pointer tree = treeGenerator->GetOutput() ;

  typedef itk::Statistics::HypersphereKernelMeanShiftModeSeeker< 
    TreeType > ModeSeekerType ;
  ModeSeekerType::Pointer modeSeeker = ModeSeekerType::New() ;
  modeSeeker->SetInputSample( tree ) ;
  modeSeeker->SetSearchRadius( 4.0 ) ;

  typedef itk::Statistics::MeanShiftModeCacheMethod< TreeType::MeasurementVectorType > CacheMethodType ;
  CacheMethodType::Pointer cacheMethod = CacheMethodType::New() ;
  cacheMethod->SetMaximumEntries(255) ;
  cacheMethod->SetMaximumConsecutiveFailures(100) ;
  cacheMethod->SetHitRatioThreshold( 0.5 ) ;
  modeSeeker->SetCacheMethod( cacheMethod.GetPointer() ) ;

  typedef itk::Statistics::SampleMeanShiftBlurringFilter< TreeType >
    FilterType ;
  FilterType::Pointer filter = FilterType::New() ;
  filter->SetInputSample( tree ) ;
  filter->SetMeanShiftModeSeeker( modeSeeker ) ;
  filter->Update() ;

  typedef ImageType OutputImageType ;
  OutputImageType::Pointer outputImage = OutputImageType::New() ;
  outputImage->SetRegions( image->GetLargestPossibleRegion() ) ;
  outputImage->Allocate() ;

  typedef itk::ImageRegionIterator< OutputImageType > ImageIteratorType ;
  ImageIteratorType io_iter( outputImage,
                             outputImage->GetLargestPossibleRegion() ) ;
  io_iter.GoToBegin() ;

  FilterType::OutputType::Pointer output = filter->GetOutput() ;
  FilterType::OutputType::Iterator fo_iter = output->Begin() ;
  FilterType::OutputType::Iterator fo_end = output->End() ;
  
//this is definitely necessary
  while ( fo_iter != fo_end )
    {
    io_iter.Set( (PixelType) fo_iter.GetMeasurementVector()[0]) ;
    ++fo_iter ;
    ++io_iter ;
    }

  ListSampleType::Pointer listSample2 = ListSampleType::New() ;
  listSample2->SetImage( outputImage ) ;

  TreeGeneratorType::Pointer treeGenerator2 = TreeGeneratorType::New() ;
  treeGenerator2->SetSample( listSample2 ) ;
  treeGenerator2->SetBucketSize( 200 ) ;
  treeGenerator2->Update() ;

  typedef itk::Statistics::SampleMeanShiftClusteringFilter< TreeType >
    ClusteringMethodType ;

  ClusteringMethodType::Pointer clusteringMethod =
    ClusteringMethodType::New() ;
  clusteringMethod->SetInputSample( treeGenerator2->GetOutput() ) ;
  clusteringMethod->SetThreshold( 0.5 ) ;
  clusteringMethod->SetMinimumClusterSize( 16 ) ;
  clusteringMethod->Update() ;

  // save clustered image
  OutputImageType::Pointer clusterMap = this->GetOutput();
  clusterMap->SetRegions( image->GetLargestPossibleRegion() ) ;
  clusterMap->Allocate() ;
  
  ImageIteratorType m_iter( clusterMap, 
                            clusterMap->GetLargestPossibleRegion() ) ;
  m_iter.GoToBegin() ;
  
  ClusteringMethodType::ClusterLabelsType clusterLabels = 
    clusteringMethod->GetOutput() ;
  
  ClusteringMethodType::ClusterLabelsType::iterator co_iter = 
    clusterLabels.begin() ;
  
  while ( co_iter != clusterLabels.end() )
    {
    m_iter.Set( (PixelType) *co_iter ) ;
    ++co_iter ;
    ++m_iter ;
    }
  

}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage >
void
SampleMeanShiftClusteringImageFilter<TInputImage >
::PrintSelf(
  std::ostream& os, 
  Indent indent) const
{
  Superclass::PrintSelf( os, indent );
}

} // end namespace itk

#endif
