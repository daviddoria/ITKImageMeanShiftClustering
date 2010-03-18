#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkCastImageFilter.h>

#include "itkSampleMeanShiftClusteringImageFilter.h"

int main(int argc, char* argv[] ) 
{
  //verify command line arguments
  if (argc != 3)
    {
      std::cout << "Required arguments: inputFileName OutputFileName" 
                << std::endl ;
      return EXIT_FAILURE;
    }
  
  //parse command line arguments
  char* inputFileName = argv[1];
  char* outputFileName = argv[2];
    
  //define some types
  const     unsigned int    Dimension = 2;
  typedef itk::Image< unsigned char, Dimension > UCImageType;
  typedef itk::Image< float, Dimension > FImageType;
  
  typedef  itk::ImageFileReader< UCImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);
  
  typedef itk::SampleMeanShiftClusteringImageFilter< UCImageType>ClusteringImageFilterType;
  ClusteringImageFilterType::Pointer clustering = ClusteringImageFilterType::New();
  clustering->SetInput( reader->GetOutput() );
    
  typedef  itk::ImageFileWriter< UCImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFileName);
  writer->SetInput( clustering->GetOutput() );
  writer->Update();
    
  return EXIT_SUCCESS;
}