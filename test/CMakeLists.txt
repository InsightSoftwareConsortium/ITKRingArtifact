itk_module_test()

set(RingArtifactTests
  itkFourierStripeArtifactImageFilterTest.cxx
  itkWaveletFourierStripeArtifactImageFilterTest.cxx
  )

CreateTestDriver(RingArtifact "${RingArtifact-Test_LIBRARIES}" "${RingArtifactTests}")

itk_add_test(NAME itkWaveletFourierStripeArtifactImageFilterTest
  COMMAND RingArtifactTestDriver
  #--compare
    #${ITK_TEST_OUTPUT_DIR}/itkNormalDistributionImageSourceTestOutput.mha
    #DATA{Baseline/itkNormalDistributionImageSourceTestOutput.mha}
  itkWaveletFourierStripeArtifactImageFilterTest
    DATA{Input/bone-ct-polar.nrrd}
    ${ITK_TEST_OUTPUT_DIR}/itkNormalDistributionImageSourceTestOutput.mha
  )

itk_add_test(NAME itkFourierStripeArtifactImageFilterTest
  COMMAND RingArtifactTestDriver
  #--compare
    #${ITK_TEST_OUTPUT_DIR}/itkNormalDistributionImageSourceTestOutput.mha
    #DATA{Baseline/itkNormalDistributionImageSourceTestOutput.mha}
  itkFourierStripeArtifactImageFilterTest
    DATA{Input/bone-ct-polar.nrrd}
    ${ITK_TEST_OUTPUT_DIR}/itkFourierStripeArtifactImageFilterTest.nrrd
  )
