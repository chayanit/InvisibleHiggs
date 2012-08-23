
from InvisibleHiggs.Ntuple.patTuple_Master_cfg import *

from PhysicsTools.PatAlgos.tools.coreTools import *


# save MC branches
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
    'file:/gpfs_phys/storm/cms/mc/Summer12/ZJetsToNuNu_400_HT_inf_TuneZ2Star_8TeV_madgraph/AODSIM/PU_S7_START52_V9-v1/0000/30D85BD2-23AE-E111-BED5-001EC9D529AB.root'
    ]

process.out.fileName = 'patTuple_MC.root'


