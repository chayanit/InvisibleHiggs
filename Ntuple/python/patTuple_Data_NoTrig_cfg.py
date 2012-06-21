from InvisibleHiggs.Ntuple.patTuple_Data_cfg import *

## No trigger selection
process.p.remove(process.hltHighLevel)


### stuff to edit
process.maxEvents.input = -1

process.source.fileNames = [
    'file:/storage/phjjb/invisibleHiggs/test/Run2012A_MET_PromptRecov1_Run191720.root'
    ]
