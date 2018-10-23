ifeq ($(strip $(DemoImageAnalyzerAuto)),)
DemoImageAnalyzerAuto := self/src/Demo/ImageAnalyzer/plugins
PLUGINS:=yes
DemoImageAnalyzerAuto_files := $(patsubst src/Demo/ImageAnalyzer/plugins/%,%,$(wildcard $(foreach dir,src/Demo/ImageAnalyzer/plugins ,$(foreach ext,$(SRC_FILES_SUFFIXES),$(dir)/*.$(ext)))))
DemoImageAnalyzerAuto_BuildFile    := $(WORKINGDIR)/cache/bf/src/Demo/ImageAnalyzer/plugins/BuildFile
DemoImageAnalyzerAuto_LOC_FLAGS_CPPDEFINES   := -DNO_FREETYPE
DemoImageAnalyzerAuto_LOC_USE := self  FWCore/Framework Fireworks/Geometry Fireworks/Tracks FWCore/PluginManager FWCore/ParameterSet DataFormats/TrackReco DataFormats/TrackerRecHit2D CommonTools/UtilAlgos pngwriter
DemoImageAnalyzerAuto_PRE_INIT_FUNC += $$(eval $$(call edmPlugin,DemoImageAnalyzerAuto,DemoImageAnalyzerAuto,$(SCRAMSTORENAME_LIB),src/Demo/ImageAnalyzer/plugins))
DemoImageAnalyzerAuto_PACKAGE := self/src/Demo/ImageAnalyzer/plugins
ALL_PRODS += DemoImageAnalyzerAuto
Demo/ImageAnalyzer_forbigobj+=DemoImageAnalyzerAuto
DemoImageAnalyzerAuto_INIT_FUNC        += $$(eval $$(call Library,DemoImageAnalyzerAuto,src/Demo/ImageAnalyzer/plugins,src_Demo_ImageAnalyzer_plugins,$(SCRAMSTORENAME_BIN),,$(SCRAMSTORENAME_LIB),$(SCRAMSTORENAME_LOGS)))
DemoImageAnalyzerAuto_CLASS := LIBRARY
else
$(eval $(call MultipleWarningMsg,DemoImageAnalyzerAuto,src/Demo/ImageAnalyzer/plugins))
endif
ALL_COMMONRULES += src_Demo_ImageAnalyzer_plugins
src_Demo_ImageAnalyzer_plugins_parent := Demo/ImageAnalyzer
src_Demo_ImageAnalyzer_plugins_INIT_FUNC += $$(eval $$(call CommonProductRules,src_Demo_ImageAnalyzer_plugins,src/Demo/ImageAnalyzer/plugins,PLUGINS))
