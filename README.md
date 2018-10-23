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
