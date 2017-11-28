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
#ifndef itkFourierStripeArtifactImageFilter_h
#define itkFourierStripeArtifactImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkForwardFFTImageFilter.h"
#include "itkInverseFFTImageFilter.h"
#include "itkImageRegionSplitterDirection.h"

namespace itk
{

/** \class FourierStripeArtifactImageFilter
 *
 * \brief Filter out vertical stripe artifacts.
 *
 * This filter removes vertical stripe artifacts from the input image.
 *
 * Stripes are removed in the direction specified with SetDirection by
 * application of a Gaussian in the Fourier domain.To increase suppression of
 * stripes not completely aligned with Direction, Increase the value of Sigma.
 *
 * To apply the filter for ring artifact removal from x-ray computed
 * tomography images, either apply the filter to the sinogram prior to
 * reconstruction, or apply the filter to an image transformed with
 * itk::CartesianToPolarTransform after reconstruction.
 *
 * \note This class has not yet been implemented
 *
 * \ingroup RingArtifact
 */
template< typename TImage >
class FourierStripeArtifactImageFilter: public ImageToImageFilter< TImage, TImage >
{
public:
  typedef TImage                        ImageType;
  typedef typename ImageType::PixelType PixelType;

  /** Standard class typedefs. */
  typedef FourierStripeArtifactImageFilter           Self;
  typedef ImageToImageFilter< ImageType, ImageType > Superclass;
  typedef SmartPointer< Self >                       Pointer;
  typedef SmartPointer< const Self >                 ConstPointer;

  itkStaticConstMacro( ImageDimension, unsigned int, ImageType::ImageDimension );

  /** Run-time type information. */
  itkTypeMacro( FourierStripeArtifactImageFilter, ImageToImageFilter );

  /** Standard New macro. */
  itkNewMacro( Self );

  /** Set the direction of the stripes. */
  itkSetMacro( Direction, unsigned int );
  itkGetConstMacro( Direction, unsigned int );

  /** Standard deviation of the Gaussian used to remove the stripes in the
   * frequency domain along Direction. */
  itkSetMacro( Sigma, double );
  itkGetConstMacro( Sigma, double );

  /** Fractional start frequency in the range [0.0, 1.0) to start filtering.
   * Frequencies below this value are not filtered. Set to a high value to
   * filter when only thin stripes are expected. */
  itkSetMacro( StartFrequency, double );
  itkGetConstMacro( StartFrequency, double );

protected:
  FourierStripeArtifactImageFilter();
  virtual ~FourierStripeArtifactImageFilter() {}

  void PrintSelf( std::ostream& os, Indent indent ) const ITK_OVERRIDE;

  virtual void BeforeThreadedGenerateData() ITK_OVERRIDE;

  typedef typename ImageType::RegionType OutputRegionType;
  virtual void ThreadedGenerateData( const OutputRegionType & outputRegion, ThreadIdType threadId ) ITK_OVERRIDE;

  virtual void AfterThreadedGenerateData() ITK_OVERRIDE;

  virtual const ImageRegionSplitterBase* GetImageRegionSplitter() const ITK_OVERRIDE;

private:
  ITK_DISALLOW_COPY_AND_ASSIGN(FourierStripeArtifactImageFilter);

  typedef ForwardFFTImageFilter< ImageType > ForwardFFTFilterType;
  typename ForwardFFTFilterType::Pointer m_ForwardFFTFilter;

  typedef typename ForwardFFTFilterType::OutputImageType ComplexImageType;
  typename ComplexImageType::Pointer m_ComplexImage;

  typedef InverseFFTImageFilter< ComplexImageType > InverseFFTFilterType;
  typename InverseFFTFilterType::Pointer m_InverseFFTFilter;

  unsigned int m_Direction;
  double       m_Sigma;
  double       m_StartFrequency;

  ImageRegionSplitterDirection::Pointer m_ImageRegionSplitter;

#ifdef ITK_USE_CONCEPT_CHECKING
  itkConceptMacro( FloatingPointPixel, ( itk::Concept::IsFloatingPoint< typename ImageType::PixelType > ) );
#endif
};
}

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFourierStripeArtifactImageFilter.hxx"
#endif

#endif // itkFourierStripeArtifactImageFilter_h
