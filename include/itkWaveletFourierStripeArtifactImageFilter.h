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
#ifndef itkWaveletFourierStripeArtifactImageFilter_h
#define itkWaveletFourierStripeArtifactImageFilter_h

#include "itkImageToImageFilter.h"

namespace itk
{

/** \class WaveletFourierStripeArtifactImageFilter
 *
 * \brief Filter out vertical stripe artifacts.
 *
 * This filter removes vertical stripe artifacts from the input image. It is
 * based off the algorithm:
 *
 * Münch, B; Trtik, P; Marone, F; Stampanoni, M (2009). Stripe and ring
 * artifact removal with combined
 * wavelet--Fourier filtering. Optics Express, 17(10):8567-8591.
 *
 * To apply the filter for ring artifact removal from x-ray computed
 * tomography images, either apply the filter to the sinogram prior to
 * reconstruction, or apply the filter to an image transformed with
 * itk::CartesianToPolarTransform after reconstruction.
 *
 * \ingroup RingArtifact
 */
template< typename TImage >
class WaveletFourierStripeArtifactImageFilter: public ImageToImageFilter< TImage, TImage >
{
public:
  typedef TImage                        ImageType;
  typedef typename ImageType::PixelType PixelType;

  /** Standard class typedefs. */
  typedef WaveletFourierStripeArtifactImageFilter    Self;
  typedef ImageToImageFilter< ImageType, ImageType > Superclass;
  typedef SmartPointer< Self >                       Pointer;
  typedef SmartPointer< const Self >                 ConstPointer;

  itkStaticConstMacro( ImageDimension, unsigned int, ImageType::ImageDimension );

  /** Run-time type information. */
  itkTypeMacro( WaveletFourierStripeArtifactImageFilter, ImageToImageFilter );

  /** Standard New macro. */
  itkNewMacro( Self );

protected:
  WaveletFourierStripeArtifactImageFilter();
  virtual ~WaveletFourierStripeArtifactImageFilter() {}

  void PrintSelf( std::ostream& os, Indent indent ) const ITK_OVERRIDE;

  virtual void GenerateData() ITK_OVERRIDE;

private:
  ITK_DISALLOW_COPY_AND_ASSIGN(WaveletFourierStripeArtifactImageFilter);

#ifdef ITK_USE_CONCEPT_CHECKING
  itkConceptMacro( FloatingPointPixel, ( itk::Concept::IsFloatingPoint< typename ImageType::PixelType > ) );
#endif
};
}

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkWaveletFourierStripeArtifactImageFilter.hxx"
#endif

#endif // itkWaveletFourierStripeArtifactImageFilter_h
