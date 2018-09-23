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
#ifndef itkFourierStripeArtifactImageFilter_hxx
#define itkFourierStripeArtifactImageFilter_hxx

#include "itkFourierStripeArtifactImageFilter.h"
#include "itkNormalVariateGenerator.h"
#include "itkGaussianOperator.h"
#include "itkImageLinearIteratorWithIndex.h"

#include "itkImageScanlineIterator.h"
#include "itkProgressReporter.h"

namespace itk
{

template< typename TImage >
FourierStripeArtifactImageFilter< TImage >
::FourierStripeArtifactImageFilter():
  m_Direction( 0 ),
  m_Sigma( 1.0 ),
  m_StartFrequency( 0.1 ),
  m_ImageRegionSplitter( ImageRegionSplitterDirection::New() )
{
  this->m_ForwardFFTFilter = ForwardFFTFilterType::New();
  this->m_InverseFFTFilter = InverseFFTFilterType::New();
  this->m_ComplexImage = ComplexImageType::New();
}


template< typename TImage >
void
FourierStripeArtifactImageFilter< TImage >
::PrintSelf( std::ostream& os, Indent indent ) const
{
  Superclass::PrintSelf( os, indent );
}


template< typename TImage >
const ImageRegionSplitterBase*
FourierStripeArtifactImageFilter< TImage >
::GetImageRegionSplitter() const
{
  return this->m_ImageRegionSplitter;
}


template< typename TImage >
void
FourierStripeArtifactImageFilter< TImage >
::BeforeThreadedGenerateData()
{
  this->m_ImageRegionSplitter->SetDirection( this->GetDirection() );

  typename ImageType::Pointer input = ImageType::New();
  input->Graft( const_cast< ImageType * >( this->GetInput() ) );

  this->m_ForwardFFTFilter->SetInput( input );

  this->m_ForwardFFTFilter->Update();
  this->m_ComplexImage->Graft( this->m_ForwardFFTFilter->GetOutput() );
}


template< typename TImage >
void
FourierStripeArtifactImageFilter< TImage >
::DynamicThreadedGenerateData( const OutputRegionType & outputRegion )
{
  typedef typename NumericTraits< typename ImageType::PixelType >::FloatType FloatType;
  typedef GaussianOperator< FloatType, ImageDimension > GaussianOperatorType;

  const unsigned int direction = this->GetDirection();

  GaussianOperatorType gaussianOperator;
  gaussianOperator.SetDirection( direction );
  gaussianOperator.SetVariance( this->GetSigma() * this->GetSigma() );
  gaussianOperator.SetMaximumError( 0.01 );
  gaussianOperator.SetMaximumKernelWidth( outputRegion.GetSize()[direction] );
  gaussianOperator.CreateDirectional();

  const ImageType * input = this->GetInput();
  OutputRegionType cropRegion;
  typename ImageType::IndexType startIndex( input->GetLargestPossibleRegion().GetIndex() );
  typename ImageType::SizeType filterSize( input->GetLargestPossibleRegion().GetSize() );
  for( unsigned int dimension = 0; dimension < ImageDimension; ++dimension )
    {
    if( dimension == direction )
      {
      continue;
      }
    const SizeValueType dimensionSize = input->GetLargestPossibleRegion().GetSize()[dimension];
    const SizeValueType frequenciesCount = static_cast< SizeValueType >( dimensionSize * ( 1.0 - this->GetStartFrequency() ));
    startIndex[dimension] = startIndex[dimension] + static_cast< IndexValueType >( dimensionSize / 2 * this->GetStartFrequency() );
    filterSize[dimension] = frequenciesCount;
    }
  cropRegion.SetIndex( startIndex );
  cropRegion.SetSize( filterSize );

  OutputRegionType filterRegion( outputRegion );
  if( !filterRegion.Crop( cropRegion ) )
    {
    return;
    }

  ImageLinearIteratorWithIndex< ComplexImageType > complexIt( this->m_ComplexImage, filterRegion );
  complexIt.SetDirection( direction );

  typedef std::vector< FloatType > CoefficientVectorType;
  const size_t gaussianRadius = gaussianOperator.GetRadius( direction );
  const size_t gaussianSize = gaussianOperator.GetSize( direction );
  const size_t gaussianRightSize = gaussianRadius + 1;
  CoefficientVectorType gaussianRight( gaussianRightSize );
  const FloatType deNormalizer = 1.0 / gaussianOperator[gaussianRadius];

  for( size_t ii = gaussianRadius; ii < gaussianSize; ++ii )
    {
    gaussianRight[ii - gaussianRadius] = 1.0 - gaussianOperator[ii] * deNormalizer;
    }
  complexIt.GoToBegin();
  while( !complexIt.IsAtEnd() )
    {
    for( size_t ii = 0; ii < gaussianRightSize; ++ii )
      {
      complexIt.Set( complexIt.Get() * gaussianRight[ii] );
      ++complexIt;
      }
    complexIt.NextLine();
    }

  const size_t gaussianLeftSize = gaussianRadius;
  CoefficientVectorType gaussianLeft( gaussianLeftSize );
  for( size_t ii = 0; ii < gaussianLeftSize; ++ii )
    {
    gaussianLeft[gaussianLeftSize - 1 - ii] = 1.0 - gaussianOperator[ii] * deNormalizer;
    }

  complexIt.GoToBegin();
  while( !complexIt.IsAtEnd() )
    {
    complexIt.GoToReverseBeginOfLine();
    for( size_t ii = 0; ii < gaussianLeftSize; ++ii )
      {
      complexIt.Set( complexIt.Get() * gaussianLeft[ii] );
      --complexIt;
      }
    complexIt.NextLine();
    }
}


template< typename TImage >
void
FourierStripeArtifactImageFilter< TImage >
::AfterThreadedGenerateData()
{
  this->m_InverseFFTFilter->GraftOutput( this->GetOutput() );

  this->m_InverseFFTFilter->SetInput( this->m_ComplexImage );
  this->m_InverseFFTFilter->Update();

  this->GraftOutput( this->m_InverseFFTFilter->GetOutput() );
}

} // end namespace itk

#endif // itkFourierStripeArtifactImageFilter_hxx
