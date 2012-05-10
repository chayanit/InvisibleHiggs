
from InvisibleHiggs.Ntuple.patTuple_Master_cfg import *

from PhysicsTools.PatAlgos.tools.coreTools import *

# No trigger filter
process.p.remove(process.hltHighLevel)

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

process.source.fileNames = [
    'file:/gpfs_phys/storm/cms/mc/Fall11/VBF_HToZZTo4Nu_M-120_7TeV-pythia6/GEN-SIM-RAW-HLTDEBUG-RECO/E7TeV_Ave32_50ns-v1/0000/04074BAA-0459-E111-91A3-003048D46300.root'
    ]

