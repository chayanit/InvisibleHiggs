from InvisibleHiggs.Ntuple.patTuple_master_cfy import *

from PhysicsTools.PatAlgos.tools.coreTools import *


# save MC branches
from PhysicsTools.PatAlgos.patEventContent_cff import *

process.out.outputCommands.extend([
    'keep recoGenParticles_genParticles_*_*'
    , 'keep GenEventInfoProduct_*_*_*'
    , 'keep GenMETs_*_*_*'
    , 'keep GenRunInfoProduct_*_*_*'
])





## stuff to edit
process.GlobalTag.globaltag = 'START44_V6::All'

process.maxEvents.input = -1

