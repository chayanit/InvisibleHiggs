import FWCore.ParameterSet.Config as cms

from CondCore.DBCommon.CondDBCommon_cfi import *
from CondCore.DBCommon.CondDBSetup_cfi import *

jec = cms.ESSource("PoolDBESSource",
      DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(0)
        ),
      timetype = cms.string('runnumber'),
      toGet = cms.VPSet(
      cms.PSet(
            record = cms.string('JetCorrectionsRecord'),
            tag    = cms.string('JetCorrectorParametersCollection_Summer12_V6_DATA_AK5PF'),
            # tag    = cms.string('JetCorrectorParametersCollection_Summer12_V6_MC_AK5PF'),
            label  = cms.untracked.string('AK5PF')
            ),
      cms.PSet(
            record = cms.string('JetCorrectionsRecord'),
            tag    = cms.string('JetCorrectorParametersCollection_Summer12_V6_DATA_AK5Calo'),
            # tag    = cms.string('JetCorrectorParametersCollection_Summer12_V6_MC_AK5Calo'),
            label  = cms.untracked.string('AK5Calo')
            )
      ## here you add as many jet types as you need
      ## note that the tag name is specific for the particular sqlite file 
      ), 
      connect = cms.string('sqlite:Summer12_V6_DATA.db')
      #connect = cms.string('sqlite:Summer12_V6_MC.db')
)
## add an es_prefer statement to resolve a possible conflict from simultaneous connection to a global tag
es_prefer_jec = cms.ESPrefer('PoolDBESSource','jec')
