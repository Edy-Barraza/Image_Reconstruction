import FWCore.ParameterSet.Config as cms

process = cms.Process("Image")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('Image')
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    limit = cms.untracked.int32(-1)
)
#process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# process all events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
# process 10 events
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

#process.add_(cms.ESProducer("FWRecoGeometryESProducer"))






process.source = cms.Source ("PoolSource",
    fileNames=cms.untracked.vstring(

	#QCD that works
	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValQCD_Pt_600_800_13_CMSSW_10_1_0/5C8EFF99-7D1E-E811-8B94-0CC47A4D7602.root",
	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValQCD_Pt_600_800_13_CMSSW_10_1_0/721BFD3D-7F1E-E811-B5FE-0025905B85DE.root",
	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValQCD_Pt_600_800_13_CMSSW_10_1_0/7E7620FC-7011-E811-A040-0CC47A4D76B6.root",
	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValQCD_Pt_600_800_13_CMSSW_10_1_0/7E8ACE49-7011-E811-940D-0CC47A4D7602.root",
	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValQCD_Pt_600_800_13_CMSSW_10_1_0/9CB2C130-7F1E-E811-98F6-0CC47A4D76D0.root",
	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValQCD_Pt_600_800_13_CMSSW_10_1_0/A2C7E0F1-6D11-E811-B361-0025905B85FC.root",
	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValQCD_Pt_600_800_13_CMSSW_10_1_0/A42E55F6-6D11-E811-9D32-003048FFCC16.root",
	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValQCD_Pt_600_800_13_CMSSW_10_1_0/BA5F8AC4-7F1E-E811-91F4-0CC47A7C34C4.root",

	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValQCD_Pt_600_800_13_CMSSW_10_1_1/402A8B28-573C-E811-8341-0CC47A7C35F8.root",
	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValQCD_Pt_600_800_13_CMSSW_10_1_1/4CCC8FBF-583C-E811-AC00-0CC47A4C8E66.root",
	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValQCD_Pt_600_800_13_CMSSW_10_1_1/726366AB-563C-E811-ABCC-0CC47A7C3412.root"


#	Extra HIGGS Files
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValHiggs200ChargedTaus_13_CMSSW_10_1_1/5A56209B-633C-E811-A30A-00248C55CC9D.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValHiggs200ChargedTaus_13_CMSSW_10_1_1/76E9723C-F73C-E811-B61B-0CC47A4C8ECE.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValHiggs200ChargedTaus_13_CMSSW_10_1_1/9881A8D5-F53C-E811-9A2C-0025905B8594.root"


#	UPSILON FILES
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValUpsilon1SToMuMu_13_CMSSW_10_0_0/1479F483-6F00-E811-98FE-0CC47A4D7636.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValUpsilon1SToMuMu_13_CMSSW_10_0_0/3290A084-6F00-E811-BB63-0CC47A7C35A4.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValUpsilon1SToMuMu_13_CMSSW_10_0_0/1EEB9471-6A00-E811-B50A-0025905B8572.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValUpsilon1SToMuMu_13_CMSSW_10_0_0/B0E4EC6F-6A00-E811-88A2-0025905A6070.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValUpsilon1SToMuMu_13_CMSSW_10_0_3/12FB3CF9-4B1D-E811-B7D9-0025905B85DA.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValUpsilon1SToMuMu_13_CMSSW_10_0_3/349740B3-2A1D-E811-A4EC-0025905B8612.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValUpsilon1SToMuMu_13_CMSSW_10_0_3/2C9686B6-4E1D-E811-AF95-0CC47A4D76C0.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValUpsilon1SToMuMu_13_CMSSW_10_0_3/E65B0B20-2C1D-E811-9ABD-0025905B85C6.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValUpsilon1SToMuMu_13_CMSSW_10_1_0/347B8FE0-7611-E811-B6ED-0025905B8560.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValUpsilon1SToMuMu_13_CMSSW_10_1_0/FAB07195-901E-E811-9A6E-0CC47A78A4B0.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValUpsilon1SToMuMu_13_CMSSW_10_1_0/42A516DB-7611-E811-86E3-0CC47A745282.root"





#QCD files being big bihs; 30A60A93 +; commenting to see what happens	
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValQCD_Pt_80_120_13_HI_CMSSW_10_0_0/30A60A93-ABDD-E711-A769-3417EBE338FA.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValQCD_Pt_80_120_13_HI_CMSSW_10_0_0/6C7741DC-AADD-E711-853C-008CFAFBE7DE.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValQCD_Pt_80_120_13_HI_CMSSW_10_0_0/3E7C0BDC-AADD-E711-8249-3417EBE669D4.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValQCD_Pt_80_120_13_HI_CMSSW_10_0_2/1AAA459A-F90E-E811-9AD5-001E677923A6.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValQCD_Pt_80_120_13_HI_CMSSW_10_0_2/B83D9704-FD0E-E811-8DD2-A4BF0112DF50.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValQCD_Pt_80_120_13_HI_CMSSW_10_1_0/1CE76126-3F1E-E811-A7B8-A4BF0112DF14.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValQCD_Pt_80_120_13_HI_CMSSW_10_1_0/A465D2DA-E037-E811-8C03-509A4C74D064.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValQCD_Pt_80_120_13_HI_CMSSW_10_1_0/46F7CAFB-3E1E-E811-835A-A4BF011254F0.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValQCD_Pt_80_120_13_HI_CMSSW_10_1_0/BE0DD715-DF37-E811-BB7C-7CD30AD089E0.root"

#JPSI FILES

#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValJpsiMuMu_Pt-8_CMSSW_10_0_0/3E06DEF7-37DD-E711-9061-0CC47A4C8ECE.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValJpsiMuMu_Pt-8_CMSSW_10_0_0/A292784F-37DD-E711-ABB0-0CC47A7452D0.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValJpsiMuMu_Pt-8_CMSSW_10_0_0/94B14EF5-6D00-E811-BFC0-0025905B861C.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValJpsiMuMu_Pt-8_CMSSW_10_0_0/B680BE48-8200-E811-AFA8-0025905A608A.root",

#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValJpsiMuMu_Pt-8_CMSSW_10_0_3/7E1C5A21-881D-E811-9CCD-0CC47A7C34D0.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValJpsiMuMu_Pt-8_CMSSW_10_0_3/B46875CB-451D-E811-918D-0025905A6088.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValJpsiMuMu_Pt-8_CMSSW_10_0_3/A4FDDD57-441D-E811-AA31-0CC47A4C8E1E.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValJpsiMuMu_Pt-8_CMSSW_10_0_3/C21C4D9F-871D-E811-BDE5-0CC47A7C340E.root",

#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValJpsiMuMu_Pt-8_CMSSW_10_1_0/52F9AA10-AD2F-E811-81C4-0242AC130002.root",
#	"file:/afs/cern.ch/user/e/ebarraza/workspace/data/RelValJpsiMuMu_Pt-8_CMSSW_10_1_0/9034719B-B02F-E811-8150-0242AC130002.root"
    
	

    )
)

#readFiles = cms.untracked.vstring()
#secFiles = cms.untracked.vstring() 
#source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
#readFiles.extend( [
#    '/store/relval/CMSSW_10_0_0/RelValPhiToMuMu_13/GEN-SIM-DIGI-RAW/100X_upgrade2018_realistic_v6_HECOLL-v1/10000/82EC4600-6500-E811-95C7-0CC47A4C8E56.root'
#                  ] )
#secFiles.extend( [
#    '/store/relval/CMSSW_10_0_0/RelValPhiToMuMu_13/GEN-SIM-DIGI-RAW/100X_upgrade2018_realistic_v6_HECOLL-v1/10000/36615A05-6500-E811-ADBF-0025905A60B0.root',
#        ] )
#process.source = source

process.dump=cms.EDAnalyzer('EventContentAnalyzer')
process.demo = cms.EDAnalyzer('ImageAnalyzer',
    ecalMaxEnergy = cms.untracked.int32(50),
    hcalMaxEnergy = cms.untracked.int32(150),
    psMaxEnergy = cms.untracked.int32(1),
    pngWidth = cms.untracked.int32(600),
    pngHeight = cms.untracked.int32(200),
    detX = cms.untracked.int32(300),
    detY = cms.untracked.int32(300),
    detZ = cms.untracked.int32(600),
    useLog = cms.untracked.int32(0),
    verbose = cms.untracked.int32(0),
    outputDir = cms.untracked.string('/afs/cern.ch/user/e/ebarraza/workspace/data/RelValQCD_Pt_600_800_13/'),
    geomFile = cms.untracked.string('/afs/cern.ch/user/v/valya/public/geom2017.root'))

#process.p = cms.Path(process.demo)
process.p = cms.Path(process.demo*process.dump)
