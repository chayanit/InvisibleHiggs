###--------------------------------------------------------------
### Load PAT, because it insists on defining the process for you
from PhysicsTools.PatAlgos.patTemplate_cfg import *
###--------------------------------------------------------------

from InvisibleHiggs.Ntuple.patTuple_Master_cfg import addInvHiggsProcess

###--------------------------------------------------------------
### Set these parameters
iRunOnData  = False 

#iData Jul13, Aug06, Aug24, PromptC2, PromptD, Dec11
#No meaning if iRunData = False
iData       = "Dec11" 

#MET, SingleMuon, DoubleMuon, SingleElectron, DoubleElectron, NoTrig
#(If iMCSignal = True, trigger will be set to NoTrig automatically)
iHLTFilter  = "MET"

#MCSignal or Background if True, PDF will be collected (Not applied yet)
#No meaning if iRunData = True
iMCSignal   = False  

iFile       = '/store/data/Run2012C/MET/AOD/PromptReco-v2/000/203/002/04BCEC26-AA02-E211-A81D-003048CF99BA.root'

iMaxEvent   = 10000

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
            iFileName += "_MCSignal_"
        else:
            iFileName += "_MC_"
    iFileName += iHLTFilter
    iFileName += "_cfg.py"
    file = open(iFileName,'w')
    file.write(str(process.dumpPython()))
    file.close()
###--------------------------------------------------------------
