
from InvisibleHiggs.Ntuple.patTuple_Master_cfg import *

from PhysicsTools.PatAlgos.tools.coreTools import *


## remove MC matching from the default sequence
removeMCMatching(process, ['All'])
runOnData(process)
addPfMET(process, 'PF')

## apply L2L3residual JEC
switchJetCollection(process, 
                    cms.InputTag('ak5PFJets'),   
                    doJTA            = True,            
                    doBTagging       = False,            
                    jetCorrLabel     = ('AK5PF', ['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']),  
                    doType1MET       = True,            
                    genJetCollection = cms.InputTag(""),
                    doJetID          = False,
                    jetIdLabel       = "ak5"
                    )



## stuff to edit
process.GlobalTag.globaltag = 'GR_R_52_V9::All'

process.maxEvents.input = -1

process.source.fileNames = [
    '/store/data/Run2012A/MET/RECO/PromptReco-v1/000/190/945/B000D689-0886-E111-BDE0-5404A63886B6.root'
#    '/store/data/Run2012A/DoubleMu/AOD/PromptReco-v1/000/190/490/96550922-2F81-E111-A37F-001D09F2462D.root'
    ]
 
process.out.fileName = 'patTuple_Data.root'



