from InvisibleHiggs.Ntuple.patTuple_Master_cfg import *

from PhysicsTools.PatAlgos.tools.coreTools import *

## remove MC matching from the default sequence
#removeMCMatching(process, ['Electrons', 'Muons', 'Taus', 'Jets'])
runOnData(process)

## stuff to edit
process.GlobalTag.globaltag = 'GR_R_52_V9::All'

process.maxEvents.input = -1

process.source.fileNames = [
    'file:/storage/phjjb/invisibleHiggs/test/Run2012A_MET_PromptRecov1_Run191720.root'
    ]
