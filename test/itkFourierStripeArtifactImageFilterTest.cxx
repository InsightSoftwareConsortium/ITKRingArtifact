/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include "itkFourierStripeArtifactImageFilter.h"

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkTestingMacros.h"
#include "itkFFTPadImageFilter.h"

int itkFourierStripeArtifactImageFilterTest( int argc, char * argv[] )
{
  if( argc < 3 )
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage outputImage";
    std::cerr << std::endl;
    return EXIT_FAILURE;
    }
  const char * inputImageFileName = argv[1];
  const char * outputImageFileName = argv[2];

  const unsigned int                         Dimension = 2;
  using PixelType = float;
  using ImageType = itk::Image< PixelType, Dimension >;

  using ReaderType = itk::ImageFileReader< ImageType >;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( inputImageFileName );

  using PadFilterType = itk::FFTPadImageFilter< ImageType >;
  PadFilterType::Pointer padFilter = PadFilterType::New();
  padFilter->SetInput( reader->GetOutput() );

  using FilterType = itk::FourierStripeArtifactImageFilter< ImageType >;
  FilterType::Pointer filter = FilterType::New();

  filter->SetInput( padFilter->GetOutput() );

  EXERCISE_BASIC_OBJECT_METHODS( filter, FourierStripeArtifactImageFilter , ImageToImageFilter );

  const unsigned int direction = 0;
  filter->SetDirection( direction );
  TEST_SET_GET_VALUE( direction, filter->GetDirection() );
  const double sigma = 3.0;
  filter->SetSigma( sigma );
  TEST_SET_GET_VALUE( sigma, filter->GetSigma() );
  const double startFrequency = 0.05;
  filter->SetStartFrequency( startFrequency );
  TEST_SET_GET_VALUE( startFrequency, filter->GetStartFrequency() );

  using WriterType = itk::ImageFileWriter< ImageType >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( outputImageFileName );
  writer->SetInput( filter->GetOutput() );
  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & error )
    {
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
