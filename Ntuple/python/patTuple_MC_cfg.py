
from InvisibleHiggs.Ntuple.patTuple_Master_cfg import *

from PhysicsTools.PatAlgos.tools.coreTools import *

# No trigger filter
#process.p.remove(process.hltHighLevel)

## switchJetCollection(process, 
##                     cms.InputTag('ak5PFJets'),   
##                     doJTA            = True,            
##                     doBTagging       = False,            
##                     jetCorrLabel     = ('AK5PF', ['L1FastJet', 'L2Relative', 'L3Absolute']),  
##                     doType1MET       = False,            
##                     genJetCollection = cms.InputTag("ak5GenJets"),
##                     doJetID      = False,
##                     jetIdLabel   = "ak5"
## #                    btagInfo = ['impactParameterTagInfos','secondaryVertexTagInfos']
## #                    btagdiscriminators=['simpleSecondaryVertexHighEffBJetTags','simpleSecondaryVertexHighPurBJetTags']
##                     )


# save MC branches
#from PhysicsTools.PatAlgos.patEventContent_cff import *

process.out.outputCommands += [ 'keep GenEventInfoProduct_*_*_*'
                                , 'keep recoGenParticles_*_*_*'
                                , 'keep GenMETs_*_*_*'
                                , 'keep *_addPileupInfo_*_*'
                                , 'keep LHEEventProduct_*_*_*'
                                ]

## stuff to edit
process.GlobalTag.globaltag = 'START52_V9B::All'

process.maxEvents.input = -1

process.source.fileNames = [
    'file:/storage/phjjb/invisibleHiggs/test/ZJetsToNuNu_400_HT_inf-PU_S7_START52_V9-AODSIM.root'
    ]

