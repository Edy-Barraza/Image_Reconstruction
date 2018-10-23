ALL_COMMONRULES += src_Demo_ImageAnalyzer_test
src_Demo_ImageAnalyzer_test_parent := Demo/ImageAnalyzer
src_Demo_ImageAnalyzer_test_INIT_FUNC += $$(eval $$(call CommonProductRules,src_Demo_ImageAnalyzer_test,src/Demo/ImageAnalyzer/test,TEST))
