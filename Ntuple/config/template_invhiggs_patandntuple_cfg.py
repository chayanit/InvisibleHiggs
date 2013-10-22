###--------------------------------------------------------------
### Load PAT, because it insists on defining the process for you
from PhysicsTools.PatAlgos.patTemplate_cfg import *
###--------------------------------------------------------------

from InvisibleHiggs.Ntuple.patNTuple_Master_cfg import addInvHiggsProcess

###--------------------------------------------------------------
### Set these parameters
iRunOnData  = False

#iData Jul13, Aug06, Aug24, PromptC2, PromptD, Dec11
#No meaning if iRunData = False
iData       = "PromptC2" 

#MET, SingleMuon, DoubleMuon, SingleElectron, DoubleElectron, NoTrig
#(If iMCSignal = True, trigger will be set to NoTrig automatically)
iHLTFilter  = "MET"

#MCSignal or Background if True, PDF will be collected (Not applied yet)
#No meaning if iRunData = True
iMCSignal   = False

#iFile	    = '/store/mc/Summer12_DR53X/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/AODSIM/PU_S10_START53_V7A-v1/0000/7CDFA60D-29CF-E111-9D37-002481E150EA.root'
iFile       = '/store/mc/Summer12_DR53X/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/AODSIM/PU_S10_START53_V7A-v1/0000/BE502377-92D1-E111-B1B6-0025B3E0656C.root'
#iFile       = '/store/data/Run2012C/MET/AOD/PromptReco-v2/000/203/002/04BCEC26-AA02-E211-A81D-003048CF99BA.root'

iMaxEvent   = 500 

#Dump configuration (Used to check configuration)
iDump       = True
###--------------------------------------------------------------


###--------------------------------------------------------------
### Create the configuration and dump
if iMCSignal == True:
    iHLTFilter  = "NoTrig"
addInvHiggsProcess(process, iRunOnData, iData, iHLTFilter, iMCSignal, iFile, iMaxEvent)

# Dump
if iDump == True:
    iFileName = "InvHiggs"
    if iRunOnData == True:
        iFileName += "_DATA_"
        iFileName += iData
        iFileName += "_"
    else:
        if iMCSignal == True:
            iFileName += "_DY_"
        else:
            iFileName += "_MC_"
    iFileName += iHLTFilter
    iFileName += "_Central_cfg.py"
    file = open(iFileName,'w')
    file.write(str(process.dumpPython()))
    file.close()
###--------------------------------------------------------------
