
from InvisibleHiggs.Ntuple.patTuple_Master_cfg import *

from PhysicsTools.PatAlgos.tools.coreTools import *

# No trigger filter
process.p.remove(process.hltHighLevel)

# JEC for MC
process.jec.toGet = cms.VPSet(
      cms.PSet(
            record = cms.string('JetCorrectionsRecord'),
            #tag    = cms.string('JetCorrectorParametersCollection_Summer12_V6_DATA_AK5PF'),
            tag    = cms.string('JetCorrectorParametersCollection_Summer12_V6_MC_AK5PF'),
            label  = cms.untracked.string('AK5PF')
            ),
      cms.PSet(
            record = cms.string('JetCorrectionsRecord'),
            #tag    = cms.string('JetCorrectorParametersCollection_Summer12_V6_DATA_AK5Calo'),
            tag    = cms.string('JetCorrectorParametersCollection_Summer12_V6_DATA_AK5Calo'),
            label  = cms.untracked.string('AK5Calo')
            )
      ## here you add as many jet types as you need
      ## note that the tag name is specific for the particular sqlite file 
      )
process.jec.connect = cms.string('sqlite:InvisibleHiggs/Ntuple/data/Summer12_V6_MC.db')

switchJetCollection(process, 
                    cms.InputTag('ak5PFJets'),   
                    doJTA            = True,            
                    doBTagging       = False,            
                    jetCorrLabel     = ('AK5PF', ['L1FastJet', 'L2Relative', 'L3Absolute']),  
                    doType1MET       = False,            
                    genJetCollection = cms.InputTag("ak5GenJets"),
                    doJetID      = False,
                    jetIdLabel   = "ak5"
#                    btagInfo = ['impactParameterTagInfos','secondaryVertexTagInfos']
#                    btagdiscriminators=['simpleSecondaryVertexHighEffBJetTags','simpleSecondaryVertexHighPurBJetTags']
                    )


# save MC branches
#from PhysicsTools.PatAlgos.patEventContent_cff import *

#process.out.outputCommands.extend([
#    'keep recoGenParticles_genParticles_*_*'
#    , 'keep GenEventInfoProduct_*_*_*'
#    , 'keep GenMETs_*_*_*'
#    , 'keep GenRunInfoProduct_*_*_*'
#])



## stuff to edit
process.GlobalTag.globaltag = 'START52_V9::All'

process.maxEvents.input = -1

process.source.fileNames = [
    'file:/gpfs_phys/storm/cms/mc/Fall11/VBF_HToZZTo4Nu_M-120_7TeV-pythia6/GEN-SIM-RAW-HLTDEBUG-RECO/E7TeV_Ave32_50ns-v1/0000/04074BAA-0459-E111-91A3-003048D46300.root'
    ]

