# Image_Reconstruction

Build for Reconstructing Images on CERN's CMSSW Distributed Computing Framework.

Once in folder on node, the following will start image reconstruction:
```
#build code
cmsenv
cd src
scram  b

# run code
cd /Demo/ImageAnalyzer/python/
cmsRun ConfFile_cfg.py
```

Simple means of changing properties of image reconstruction is changing 
```
Demo/ImageAnalyzer/python/ConfFile_cfg.py. 
```
Complicated means of changing properties of image reconstruction is changing 
```
Demo/ImageAnalyzer/plugins/ImageAnalyzer.cc 
``` 
