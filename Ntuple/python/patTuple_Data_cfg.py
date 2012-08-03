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
                    doJetID          = True,
                    jetIdLabel       = "ak5"
                    )



## stuff to edit
process.GlobalTag.globaltag = 'GR_R_52_V9::All'

process.maxEvents.input = -1

process.source.fileNames = [
    'file:/storage/phjjb/invisibleHiggs/test/MET-Run2012A-Run193621-AOD.root'
    ]
