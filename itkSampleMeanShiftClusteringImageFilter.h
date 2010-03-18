
/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSampleMeanShiftClusteringImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-05-02 05:43:55 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

//taken from itkScalarImageKmeansImageFilter

#ifndef __itkSampleMeanShiftClusteringImageFilter_h
#define __itkSampleMeanShiftClusteringImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"

#include "itkKdTree.h"
#include "itkKdTreeBasedKmeansEstimator.h"
#include "itkWeightedCentroidKdTreeGenerator.h"

#include <itkEuclideanDistanceMetric.h>
#include <itkSampleClassifierFilter.h>
#include <itkImageToListSampleAdaptor.h>
#include <itkMinimumDecisionRule2.h>

#include <itkImageRegion.h>
#include <itkRegionOfInterestImageFilter.h>

#include <vector>

namespace itk
{
/** \class SampleMeanShiftClusteringImageFilter
 * \brief Classifies the intensity values of a scalar image using the K-Means algorithm.
 *
 * Given an input image with scalar values, creates clusters.
 *
 * \sa Image
 * 
 * \ingroup ClassificationFilters 
 */
template <class TInputImage >
class ITK_EXPORT SampleMeanShiftClusteringImageFilter:
    public ImageToImageFilter< TInputImage, Image<unsigned char,
             ::itk::GetImageDimension<TInputImage>::ImageDimension> >
{
public:
  /** Extract dimension from input and output image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Convenient typedefs for simplifying declarations. */
  typedef TInputImage InputImageType;
  typedef Image<unsigned char,
             ::itk::GetImageDimension<TInputImage>::ImageDimension> OutputImageType;

  /** Standard class typedefs. */
  typedef SampleMeanShiftClusteringImageFilter                 Self;
  typedef ImageToImageFilter< InputImageType, OutputImageType> Superclass;
  typedef SmartPointer<Self>                                   Pointer;
  typedef SmartPointer<const Self>                             ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SampleMeanShiftClusteringImageFilter, ImageToImageFilter);

  /** Image typedef support. */
  typedef typename InputImageType::PixelType  InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;
#if 0
  
  /** Type used for representing the Mean values */
  typedef typename NumericTraits< InputPixelType >::RealType RealPixelType;
  
  
  /** Create a List from the scalar image */
  typedef itk::Statistics::ImageToListSampleAdaptor< InputImageType > AdaptorType;

  /** Define the Measurement vector type from the AdaptorType */
  typedef typename AdaptorType::MeasurementVectorType  MeasurementVectorType;

  typedef itk::Statistics::DistanceToCentroidMembershipFunction< MeasurementVectorType > MembershipFunctionType;
  typedef itk::Statistics::SampleClassifierFilter< AdaptorType > ClassifierType;
  typedef itk::Statistics::MinimumDecisionRule2                  DecisionRuleType;

  typedef typename ClassifierType::ClassLabelVectorType ClassLabelVectorType;
  
  typedef typename ClassifierType::MembershipFunctionVectorType MembershipFunctionVectorType;
  typedef typename MembershipFunctionType::CentroidType  MembershipFunctionOriginType;
  
  typedef typename MembershipFunctionType::Pointer     MembershipFunctionPointer;
  
  /** Create the K-d tree structure */
  typedef itk::Statistics::WeightedCentroidKdTreeGenerator< 
                                                      AdaptorType > 
                                                                TreeGeneratorType;
  typedef typename TreeGeneratorType::KdTreeType                TreeType;
  typedef itk::Statistics::KdTreeBasedKmeansEstimator<TreeType> EstimatorType;

  typedef typename EstimatorType::ParametersType ParametersType;

  typedef typename InputImageType::RegionType        ImageRegionType;
  
  typedef RegionOfInterestImageFilter< 
                                 InputImageType,
                                 InputImageType  > RegionOfInterestFilterType;

  /** Get the region over which the statistics will be computed */
  itkGetConstReferenceMacro( ImageRegion, ImageRegionType );
  
#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasNumericTraitsCheck,
                  (Concept::HasNumericTraits<InputPixelType>));
  /** End concept checking */
#endif

#endif
protected:
  SampleMeanShiftClusteringImageFilter() {};
  virtual ~SampleMeanShiftClusteringImageFilter() {}
  void PrintSelf(std::ostream& os, Indent indent) const;

  void GenerateData();

private:
  SampleMeanShiftClusteringImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};
  
} // end namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSampleMeanShiftClusteringImageFilter.txx"
#endif


#endif
