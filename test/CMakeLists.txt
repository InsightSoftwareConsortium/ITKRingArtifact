itk_module_test()

set(RingArtifactTests
  itkFourierStripeArtifactImageFilterTest.cxx
  )

CreateTestDriver(RingArtifact "${RingArtifact-Test_LIBRARIES}" "${RingArtifactTests}")

itk_add_test(NAME itkFourierStripeArtifactImageFilterTest
  COMMAND RingArtifactTestDriver
  --compare
    ${ITK_TEST_OUTPUT_DIR}/itkFourierStripeArtifactImageFilterTest.nrrd
    DATA{Baseline/itkFourierStripeArtifactImageFilterTest.nrrd}
  itkFourierStripeArtifactImageFilterTest
    DATA{Input/bone-ct-polar.nrrd}
    ${ITK_TEST_OUTPUT_DIR}/itkFourierStripeArtifactImageFilterTest.nrrd
  )
