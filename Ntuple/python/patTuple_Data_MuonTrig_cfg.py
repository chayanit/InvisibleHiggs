from InvisibleHiggs.Ntuple.patTuple_Data_cfg import *

### select di-muon triggers
process.hltHighLevel.HLTPaths = cms.vstring(
    "HLT_Mu13_Mu8_v*"
)


### stuff to edit
process.maxEvents.input = -1

process.source.fileNames = [
    'file:/storage/phjjb/invisibleHiggs/test/Run2012A_MET_PromptRecov1_Run191720.root'
    ]
