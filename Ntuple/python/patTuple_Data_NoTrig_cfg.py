
from InvisibleHiggs.Ntuple.patTuple_Data_cfg import *

## No trigger selection
process.p.remove(process.hltHighLevel)


### stuff to edit
process.maxEvents.input = -1

process.source.fileNames = [
    '/store/data/Run2012A/DoubleMu/AOD/PromptReco-v1/000/190/490/96550922-2F81-E111-A37F-001D09F2462D.root'
    ]

process.out.fileName = 'patTuple_DoubleMuNoTrig.root'

