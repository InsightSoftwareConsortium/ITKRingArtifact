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

#include "itkImageScanlineIterator.h"
#include "itkProgressReporter.h"

namespace itk
{

template< typename TImage >
FourierStripeArtifactImageFilter< TImage >
::FourierStripeArtifactImageFilter():
  m_Direction( 0 )
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
void
FourierStripeArtifactImageFilter< TImage >
::BeforeThreadedGenerateData()
{
  typename ImageType::Pointer input = ImageType::New();
  input->Graft( const_cast< ImageType * >( this->GetInput() ) );

  this->m_ForwardFFTFilter->SetInput( input );

  this->m_ForwardFFTFilter->Update();
  this->m_ComplexImage->Graft( this->m_ForwardFFTFilter->GetOutput() );
}


template< typename TImage >
void
FourierStripeArtifactImageFilter< TImage >
::ThreadedGenerateData( const OutputRegionType & outputRegion, ThreadIdType threadId )
{
  typedef typename NumericTraits< typename ImageType::PixelType >::RealType RealType;
  typedef GaussianOperator< RealType, ImageDimension > GaussianOperatorType;

  GaussianOperatorType gaussianOperator;
  gaussianOperator.SetDirection( this->GetDirection() );
  //typename ImageType::Pointer input = ImageType::New();
  //input->Graft( const_cast< ImageType * >( this->GetInput() ));


  //ImageType * output = this->GetOutput();

  //m_RescaleFilter->GraftOutput( this->GetOutput() );
  //m_RescaleFilter->Update();
  //this->GraftOutput( m_RescaleFilter->GetOutput() );
}


template< typename TImage >
void
FourierStripeArtifactImageFilter< TImage >
::AfterThreadedGenerateData()
{
  this->m_InverseFFTFilter->GraftOutput( this->GetOutput() );

  // todo: this->m_InverseFFTFilter->SetInput(

  this->GraftOutput( this->m_InverseFFTFilter->GetOutput() );
}

} // end namespace itk

#endif // itkFourierStripeArtifactImageFilter_hxx
