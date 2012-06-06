from InvisibleHiggs.Ntuple.invHiggsTree_Master_cfg import *

# no need for trigger filter
process.path.remove(process.hltHighLevel)

# jet corrections have already been applied
process.path.remove(process.kt6PFJets)
process.path.remove(process.ak5PFJets)
process.path.remove(process.ak5PFJetsL1FastL2L3)
process.path.remove(process.ak5CaloJets)
process.path.remove(process.ak5CaloJetsL1FastL2L3)

# read Tree info from PAT instead of AOD
#process.invHiggsTree.caloJetTag = cms.untracked.InputTag("patCaloJets")
process.invHiggsTree.jetTag = cms.untracked.InputTag("cleanPatJets")
process.invHiggsTree.muonTag = cms.untracked.InputTag("cleaPatMuons")
process.invHiggsTree.electronTag = cms.untracked.InputTag("cleanPatElectrons")
#process.invHiggsTree.caloMETTag = cms.untracked.InputTag("patMETs")
process.invHiggsTree.pfMETTag = cms.untracked.InputTag("patMETs")
#process.invHiggsTree.mhtTag = cms.untracked.InputTag("patPFMht")
#process.invHiggsTree.vertexTag = cms.untracked.InputTag("offlinePrimaryVertices")



### THINGS TO EDIT BELOW ###

# change Global Tag
process.GlobalTag.globaltag = 'START44_V6::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# input files
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
process.source.fileNames = [
    'file:/storage/phjjb/invisibleHiggs/test/InvHiggs_PAT_Fall11_Testv1_WJetsToLNu_test.root'
    ]

