import InvisibleHiggs.Ntuple.invHiggsTree_Master_cfg

# no need for trigger filter
process.path.remove(process.hltHighLevel)

# jet corrections have already been applied
process.path.remove(process.kt6PFJets)
process.path.remove(process.ak5PFJets)
process.path.remove(process.ak5PFJetsL1FastL2L3)
process.path.remove(process.ak5CaloJets)
process.path.remove(process.ak5CaloJetsL1FastL2L3)

# read Tree info from PAT instead of AOD
process.invHiggsTree.usePAT = cms.untracked.bool(True)
process.invHiggsTree.caloJetTag = cms.untracked.InputTag("patCaloJets")
process.invHiggsTree.pfJetTag = cms.untracked.InputTag("patPFJets")
process.invHiggsTree.muonTag = cms.untracked.InputTag("patMuons")
process.invHiggsTree.electronTag = cms.untracked.InputTag("patElectrons")
process.invHiggsTree.caloMETTag = cms.untracked.InputTag("patMET")
process.invHiggsTree.pfMETTag = cms.untracked.InputTag("patPFMet")
process.invHiggsTree.mhtTag = cms.untracked.InputTag("patPFMht")
process.invHiggsTree.vertexTag = cms.untracked.InputTag("offlinePrimaryVertices")



### THINGS TO EDIT BELOW ###

# change Global Tag
process.GlobalTag.globaltag = 'START44_V6::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# input files
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
    'file:/gpfs_phys/storm/cms/mc/Fall11/VBF_HToZZTo4Nu_M-120_7TeV-pythia6/GEN-SIM-RAW-HLTDEBUG-RECO/E7TeV_Ave32_50ns-v1/0000/04074BAA-0459-E111-91A3-003048D46300.root'
    ] );

