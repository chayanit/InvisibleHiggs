import InvisibleHiggs.Ntuple.invHiggsTree_Master_cfg

# no need for trigger filter
process.path.remove(process.hltHighLevel)

# jet corrections have already been applied
process.path.remove(process.kt6PFJets)
process.path.remove(process.ak5PFJets)
process.path.remove(process.ak5PFJetsL1FastL2L3)
process.path.remove(process.ak5CaloJets)
process.path.remove(process.ak5CaloJetsL1FastL2L3)

# use PAT instead of AOD
invHiggsTree.usePAT = cms.untracked.bool(True)
invHiggsTree.caloJetTag = cms.untracked.InputTag("patCaloJets")
invHiggsTree.pfJetTag = cms.untracked.InputTag("patPFJets")
invHiggsTree.muonTag = cms.untracked.InputTag("patMuons")
invHiggsTree.electronTag = cms.untracked.InputTag("patElectrons")
invHiggsTree.caloMETTag = cms.untracked.InputTag("patMET")
invHiggsTree.pfMETTag = cms.untracked.InputTag("patPFMet")
invHiggsTree.mhtTag = cms.untracked.InputTag("patPFMht")
invHiggsTree.vertexTag = cms.untracked.InputTag("offlinePrimaryVertices")



### THINGS TO EDIT BELOW ###

# change Global Tag
process.GlobalTag.globaltag = 'GR_R_52_V7::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# input files
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
    'file:/gpfs_phys/storm/cms/mc/Fall11/VBF_HToZZTo4Nu_M-120_7TeV-pythia6/GEN-SIM-RAW-HLTDEBUG-RECO/E7TeV_Ave32_50ns-v1/0000/04074BAA-0459-E111-91A3-003048D46300.root'
    ] );

