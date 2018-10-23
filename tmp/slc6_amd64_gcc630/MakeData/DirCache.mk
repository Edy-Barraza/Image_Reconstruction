ALL_SUBSYSTEMS+=Demo
subdirs_src_Demo = src_Demo_ImageAnalyzer
ALL_PACKAGES += Demo/ImageAnalyzer
subdirs_src_Demo_ImageAnalyzer := src_Demo_ImageAnalyzer_python src_Demo_ImageAnalyzer_test src_Demo_ImageAnalyzer_interface src_Demo_ImageAnalyzer_plugins
ifeq ($(strip $(PyDemoImageAnalyzer)),)
PyDemoImageAnalyzer := self/src/Demo/ImageAnalyzer/python
src_Demo_ImageAnalyzer_python_parent := 
ALL_PYTHON_DIRS += $(patsubst src/%,%,src/Demo/ImageAnalyzer/python)
PyDemoImageAnalyzer_files := $(patsubst src/Demo/ImageAnalyzer/python/%,%,$(wildcard $(foreach dir,src/Demo/ImageAnalyzer/python ,$(foreach ext,$(SRC_FILES_SUFFIXES),$(dir)/*.$(ext)))))
PyDemoImageAnalyzer_LOC_USE := self  
PyDemoImageAnalyzer_PACKAGE := self/src/Demo/ImageAnalyzer/python
ALL_PRODS += PyDemoImageAnalyzer
PyDemoImageAnalyzer_INIT_FUNC        += $$(eval $$(call PythonProduct,PyDemoImageAnalyzer,src/Demo/ImageAnalyzer/python,src_Demo_ImageAnalyzer_python,1,1,$(SCRAMSTORENAME_PYTHON),$(SCRAMSTORENAME_LIB),,))
else
$(eval $(call MultipleWarningMsg,PyDemoImageAnalyzer,src/Demo/ImageAnalyzer/python))
endif
ALL_COMMONRULES += src_Demo_ImageAnalyzer_python
src_Demo_ImageAnalyzer_python_INIT_FUNC += $$(eval $$(call CommonProductRules,src_Demo_ImageAnalyzer_python,src/Demo/ImageAnalyzer/python,PYTHON))
ALL_COMMONRULES += src_Demo_ImageAnalyzer_test
src_Demo_ImageAnalyzer_test_parent := Demo/ImageAnalyzer
src_Demo_ImageAnalyzer_test_INIT_FUNC += $$(eval $$(call CommonProductRules,src_Demo_ImageAnalyzer_test,src/Demo/ImageAnalyzer/test,TEST))
