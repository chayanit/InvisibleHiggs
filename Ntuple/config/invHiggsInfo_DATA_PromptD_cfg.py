import FWCore.ParameterSet.Config as cms

from InvisibleHiggs.Ntuple.invHiggsInfo_Master_cfg import *

# PU re-weighting
process.invHiggsInfo.puMCFile   = cms.untracked.string("")
process.invHiggsInfo.puDataFile = cms.untracked.string("")
process.invHiggsInfo.puMCHist   = cms.untracked.string("")
process.invHiggsInfo.puDataHist = cms.untracked.string("")
process.invHiggsInfo.mcPYTHIA   = cms.untracked.bool(False)

process.TFileService.fileName = cms.string('invHiggsInfo_Data.root')


### THINGS TO EDIT BELOW ###

# change Global Tag
process.GlobalTag.globaltag = 'GR_P_V42_AN3::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# input files
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
    'file:patTuple_MC.root'
    #'file:/gpfs_phys/storm/cms/user/chayanit/VBF_HToZZTo4Nu_M-120_8TeV-pythia6/InvHiggsPAT_Summer12_v6A_VBF_HToZZTo4Nu_M120/5f2442bea011b43a734a0e8981525e8b/patTuple_6_1_deN.root'
    #'/uscms_data/d3/chayanit/VplusJetsAnalysis2012/CMSSW_5_2_5/src/InvisibleHiggs/Ntuple/python/testroot/patTuple_DYJetsToLLTrig.root'
    ] );

