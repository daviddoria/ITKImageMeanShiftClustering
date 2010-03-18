cmake_minimum_required(VERSION 2.6)

#why is this necessary?
INCLUDE_DIRECTORIES(~/src/Insight/Code/Numerics/Statistics)

PROJECT(itkSampleMeanShiftClusteringImageFilter)

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

ADD_EXECUTABLE(Example Example.cxx itkSampleMeanShiftClusteringImageFilter.txx) 
TARGET_LINK_LIBRARIES(Example ITKCommon ITKIO ITKNumerics ITKStatistics)

