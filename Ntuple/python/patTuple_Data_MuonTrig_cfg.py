from InvisibleHiggs.Ntuple.patTuple_Data_cfg import *

### select di-muon triggers
process.hltHighLevel.HLTPaths = cms.vstring(
    "HLT_Mu17_Mu8_v*"
#    "HLT_Mu13_Mu8_v*"
)


### stuff to edit
process.maxEvents.input = -1

process.source.fileNames = [
    '/store/data/Run2012A/DoubleMu/AOD/PromptReco-v1/000/190/490/96550922-2F81-E111-A37F-001D09F2462D.root'
    ]

process.out.fileName = 'patTuple_DoubleMuTrig.root'


