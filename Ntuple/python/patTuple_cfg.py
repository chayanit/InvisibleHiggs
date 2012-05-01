from InvisibleHiggs.Ntuple.patTuple_master_cfy import *

from PhysicsTools.PatAlgos.tools.coreTools import *

## remove MC matching from the default sequence
removeMCMatching(process, ['Electrons', 'Muons', 'Taus', 'Jets'])
runOnData(process)

## stuff to edit
process.GlobalTag.globaltag = 'GR_R_52_V7::All'

process.maxEvents.input = -1

