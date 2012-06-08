import FWCore.ParameterSet.Config as cms

from InvisibleHiggs.Ntuple.invHiggsAOD_cfg import *


process.path.remove(process.hltHighLevel)



### THINGS TO EDIT BELOW ###

# change Global Tag
process.GlobalTag.globaltag = 'START44_V6::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# input files
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
    'file:/gpfs_phys/storm/cms/mc/Fall11/VBF_HToZZTo4Nu_M-120_7TeV-pythia6/GEN-SIM-RAW-HLTDEBUG-RECO/E7TeV_Ave32_50ns-v1/0000/04074BAA-0459-E111-91A3-003048D46300.root'
    ] );

