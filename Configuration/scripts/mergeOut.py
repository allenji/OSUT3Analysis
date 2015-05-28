#!/usr/bin/env python
import py_compile
import imp 
import sys
import os
import re
from optparse import OptionParser
from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *
from OSUT3Analysis.Configuration.formattingUtilities import *
from OSUT3Analysis.DBTools.condorSubArgumentsSet import *
parser = OptionParser()
parser = set_commandline_arguments(parser)

parser.remove_option("-o")
parser.remove_option("-c")
parser.remove_option("-n")
parser.remove_option("-u")
parser.remove_option("-e")
parser.remove_option("-r")
parser.remove_option("-R")
parser.remove_option("-d")
parser.remove_option("-b")
parser.remove_option("--2D")
parser.remove_option("-y")
parser.remove_option("-p")

parser.add_option("-d", "--dataset", dest="Dataset", default = "", help="Specify which dataset to run.")
parser.add_option("-w", "--workDirectory", dest="Directory", default = "", help="Specify the working directroy.")
parser.add_option("-L", "--targetLumi", dest="IntLumi", default = "", help="Specify the targeting luminosity.")
parser.add_option("-c", "--condor", dest="UseCondor", default = False,action = "store_true", help="Run merging jobs on condor.")

(arguments, args) = parser.parse_args()
from ROOT import TFile
###############################################################################
#                     Make the submission script for condor.                  #
###############################################################################
def MakeSubmissionScriptForMerging(Directory):
    if not arguments.localConfig:
        print "No local configuration file is provided! Submission quited!"
        sys.exit()
    else:
	os.system('touch ' + Directory + '/condorMerging.sub')
        SubmitFile = open(Directory + '/condorMerging.sub','r+w')
    for argument in sorted(CondorSubArgumentsSet):
        if CondorSubArgumentsSet[argument].has_key('Executable') and CondorSubArgumentsSet[argument]['Executable'] == "":
            SubmitFile.write('Executable = merge.py\n')
        elif CondorSubArgumentsSet[argument].has_key('Arguments') and CondorSubArgumentsSet[argument]['Arguments'] == "":
            SubmitFile.write('Arguments = $(Process) ' + '\n\n')
        elif CondorSubArgumentsSet[argument].has_key('Transfer_Input_files') and CondorSubArgumentsSet[argument]['Transfer_Input_files'] == "":
             datasetInfoString = ''
             for dataset in split_datasets:
                 datasetInfoString = datasetInfoString + './' + dataset + '/datasetInfo_' + dataset +'_cfg.py,'
             SubmitFile.write('Transfer_Input_files = merge.py,' + datasetInfoString + '\n')
        elif CondorSubArgumentsSet[argument].has_key('Queue'):
            SubmitFile.write('Queue ' + str(len(split_datasets)) +'\n')
        else:
            SubmitFile.write(CondorSubArgumentsSet[argument].keys()[0] + ' = ' + CondorSubArgumentsSet[argument].values()[0] + '\n')
    SubmitFile.close()

def GetCompleteOrderedArgumentsSet(InputArguments):
    NewArguments = copy.deepcopy(InputArguments)
    for argument in InputArguments:
        for index in CondorSubArgumentsSet:
            if CondorSubArgumentsSet[index].has_key(argument):
                CondorSubArgumentsSet[index][argument] = InputArguments[argument]
                NewArguments.pop(argument)
                break
    for newArgument in NewArguments:
        CondorSubArgumentsSet.setdefault(len(CondorSubArgumentsSet.keys()) + 1,{newArgument : NewArguments[newArgument]})

###############################################################################
#                 Make the configuration for condor to run over.              #
###############################################################################
def MakeMergingConfigForCondor(Directory):
        os.system('touch ' + Directory + '/merge.py')
        MergeScript = open(Directory + '/merge.py','r+w')
        MergeScript.write('#!/usr/bin/env python\n')
        MergeScript.write('import sys\n')
        MergeScript.write('import os\n\n')
        MergeScript.write('datasets = [\n')
        for dataset in split_datasets:
            MergeScript.write("'" + dataset + "',\n")
        MergeScript.write(']\n')
        MergeScript.write('Index = int(sys.argv[1])\n\n')
        MergeScript.write('dataset = datasets[Index]\n\n')
        MergeScript.write('sys.path.append(os.getcwd() + \'/\' + dataset)\n')
        MergeScript.write('os.chdir(os.getcwd() + \'/\' + dataset)\n')
        MergeScript.write('exec(\'import outputInfo_\' + dataset +\'_cfg as outputInfo\')\n')
        MergeScript.write('InputFileString = outputInfo.InputFileString\n')
        MergeScript.write('InputWeightString = outputInfo.InputWeightString\n')
        MergeScript.write('os.system(\'mergeTFileServiceHistograms -i \' + InputFileString + \' -o \' + \'../\' + dataset + \'.root\' + \' -w \' + InputWeightString)\n')
        MergeScript.write('print \'Finish merging dataset \' + dataset')
        MergeScript.close()
        os.system('chmod 777 ' + Directory + '/merge.py')

###############################################################################
#                       Get the exit code of condor jobs.                     #
###############################################################################
def MessageDecoder(Message, Good):
    Pattern = r'condor_(.*).log.*value (.*)\)'
    Decoded = re.match(Pattern,Message)    
    if not Good:
        print "Warning!!! Job " + Decoded.group(1) +" has non ZERO exit code: " + Decoded.group(2)
    return Decoded.group(1)   
###############################################################################
#  Get the string of good root files and the corresponding string of weights  #
###############################################################################
def GetGoodRootFiles(Index):
    return os.popen('ls *_' + str(Index) + '.root').read().rstrip('\n')
def MakeInputFileString(FilesSet):
    Str = ''
    for i in range(0,len(FilesSet)):
	Str = Str + ' ' + str(FilesSet[i])
    return Str
def MakeWeightsString(Weight,FilesSet):
    Str = ''
    for i in range(0,len(FilesSet)):
        if i == 0:
            Str = Str + str(Weight)
        else:
            Str = Str + ',' + str(Weight)
    return Str
###############################################################################
#   Get the total number of events from cutFlows to calculate the weights     #
###############################################################################
def GetNumberOfEvents(FilesSet):
    NumberOfEvents = {'SkimNumber' : {}, 'TotalNumber' : 0}
    for File in FilesSet:
        ScoutFile = TFile(File)
        if ScoutFile.IsZombie(): 
            print File + " is a bad root file."
            FilesSet.remove(File)
            continue
        randomChannelDirectory = ""
	TotalNumberTmp = 0 
        for key in ScoutFile.GetListOfKeys():
            if key.GetClassName() != "TDirectoryFile" or "CutFlow" not in key.GetName():
                continue
            randomChannelDirectory = key.GetName()
            channelName = randomChannelDirectory[0:len(randomChannelDirectory)-14]
            if not NumberOfEvents['SkimNumber'].has_key(channelName):
		NumberOfEvents['SkimNumber'][channelName] = 0
            OriginalCounterObj = ScoutFile.Get(randomChannelDirectory + "/eventCounter")
            SkimCounterObj = ScoutFile.Get(randomChannelDirectory + "/cutFlow")
	    TotalNumberTmp = 0 
            if not OriginalCounterObj:
	        print "Could not find eventCounter histogram in " + str(File) + " !"
	        continue
            elif not SkimCounterObj:
	        print "Could not find cutFlow histogram in " + str(File) + " !"
            else:
	        OriginalCounter = OriginalCounterObj.Clone()
                OriginalCounter.SetDirectory(0)
                TotalNumberTmp = TotalNumberTmp + OriginalCounter.GetBinContent(1) 
	        SkimCounter = SkimCounterObj.Clone()
                SkimCounter.SetDirectory(0)
                NumberOfEvents['SkimNumber'][channelName] = NumberOfEvents['SkimNumber'][channelName] + SkimCounter.GetBinContent(SkimCounter.GetXaxis().GetNbins())
        NumberOfEvents['TotalNumber'] = NumberOfEvents['TotalNumber'] + TotalNumberTmp 
    return NumberOfEvents
###############################################################################
#                 Produce important files for the skim directory.             #
###############################################################################
def MakeFilesForSkimDirectory(Directory, TotalNumber, SkimNumber):
    for Member in os.listdir(Directory):
        if os.path.isfile(os.path.join(Directory, Member)):
            continue;
        os.system('echo ' + str(TotalNumber) + ' > ' +Directory + '/' + Member + '/OriginalNumberOfEvents.txt')
        os.system('echo ' + str(SkimNumber[Member]) + ' > ' +Directory + '/' + Member + '/SkimNumberOfEvents.txt')
        os.chdir(Directory + '/' + Member)
        listOfSkimFiles = os.popen('ls *.root').readlines()
        sys.path.append(Directory + '/' + Member)
        for file in listOfSkimFiles:
            if not SkimFileValidator(file.rstrip('\n')):
                os.system('rm ' + file.rstrip('\n'))
            #print SkimFileValidator('/home/bing/CMSSW_6_2_7_patch2/src/OSUT3Analysis/AnaTools/test/condor/Jan9_test2/SingleT_s/Preselection/skim_16.root')
        os.chdir(Directory)
###############################################################################
#                 Make submission script for the failed jobs.                 #
###############################################################################
def MakeResubmissionScript(badIndices, originalSubmissionScript):
    os.system('touch condor_resubmit.sub')
    resubScript = open('condor_resubmit.sub','r+w') 
    originalScript = open(originalSubmissionScript,'r')
    indexDependence = []

    for line in originalScript:
        if '$(Process)' not in line and 'Queue' not in line:
            resubScript.write(line) 
        elif '$(Process)' in line:
            indexDependence.append(line)
            resubScript.write(line.replace('$(Process)',str(badIndices[0])))      
        else:
            resubScript.write('Queue 1\n\n')

    for index in range(1,len(badIndices)):
        for item in indexDependence:
            resubScript.write(item.replace('$(Process)',str(badIndices[index])))
        resubScript.write('Queue 1\n\n')

    resubScript.close()
    originalScript.close()
###############################################################################
#                       Determine whether a skim file is valid.               #
###############################################################################
def SkimFileValidator(File):
    FileToTest = TFile(File)
    Valid = True
    Valid = Valid and FileToTest.Get('MetaData') and FileToTest.Get('ParameterSets') and FileToTest.Get('Parentage') and FileToTest.Get('Events') and FileToTest.Get('LuminosityBlocks') and FileToTest.Get('Runs')
    if Valid:
	Valid = Valid and FileToTest.Get('Events').GetEntries()
    return Valid
###############################################################################
#                           Getting the working directory.                    #
###############################################################################
CondorDir = ''
if arguments.Directory == "":
    print "No working directory is given, aborting."
    sys.exit()
else:
    CondorDir = os.getcwd() + '/condor/' + arguments.Directory
###############################################################################
#Check whether the necessary arguments or the local config are given correctly#
###############################################################################
split_datasets = []
IntLumi = 0.0
if arguments.localConfig:
    sys.path.append(os.getcwd())
    exec("from " + re.sub (r".py$", r"", arguments.localConfig) + " import *")
    composite_datasets = get_composite_datasets(datasets, composite_dataset_definitions)
    split_datasets = split_composite_datasets(datasets, composite_dataset_definitions)
    IntLumi = intLumi    

if not arguments.localConfig:
    if arguments.Dataset == "":
        print "There are no datasets to merge!"
    else:
        split_datasets.append(arguments.Dataset) 
    IntLumi = float(arguments.IntLumi)


for dataSet in split_datasets:
    directory = CondorDir + '/' + dataSet
    if not os.path.exists(directory):
        print directory + " does not exist, will skip it and continue!"
        continue
    print "....................Merging dataset " + dataSet + " ...................."
    os.chdir(directory)
    ReturnValues = []
    LogFiles = os.popen('ls condor_*.log').readlines()
    for i in range(0,len(LogFiles)):
        ReturnValues.append('condor_' + str(i) + '.log' + str(os.popen('grep "return value" condor_' + str(i)  + '.log | tail -1').readline().rstrip('\n')))
    GoodIndices = []
    GoodRootFiles = []
    BadIndices = []   
 
    for i in range(0,len(ReturnValues)):
        if "return value 0" not in ReturnValues[i] and "return value" in ReturnValues[i]:
            print ReturnValues[i]
            BadIndex = MessageDecoder(ReturnValues[i],False)
            BadIndices.append(BadIndex) 
        elif "return value" in ReturnValues[i]:    
            GoodIndex = MessageDecoder(ReturnValues[i], True)
            GoodIndices.append(GoodIndex)
    if BadIndices:
        MakeResubmissionScript(BadIndices, 'condor.sub')
    for i in range(0,len(GoodIndices)):
        GoodRootFiles.append(GetGoodRootFiles(GoodIndices[i]))
    if not len(GoodRootFiles):
        print "Unfortunately there are no good root files to merge!"
        continue
    InputFileString = MakeInputFileString(GoodRootFiles)
    sys.path.append(directory)
    exec('import datasetInfo_' + dataSet + '_cfg as datasetInfo')
    TotalNumber = GetNumberOfEvents(GoodRootFiles)['TotalNumber']
    SkimNumber = GetNumberOfEvents(GoodRootFiles)['SkimNumber']
    if not TotalNumber:
        MakeFilesForSkimDirectory(directory,TotalNumber,SkimNumber)
        continue
    Weight = 1.0
    crossSection = float(datasetInfo.crossSection)
    runOverSkim = True
    try:
        datasetInfo.originalNumberOfEvents
    except AttributeError:
        runOverSkim = False
    if crossSection > 0:
        if runOverSkim:
            Weight = IntLumi*crossSection*float(datasetInfo.skimNumberOfEvents)/(float(datasetInfo.originalNumberOfEvents)*float(TotalNumber))
        else:
            Weight = IntLumi*crossSection/float(TotalNumber)
    InputWeightString = MakeWeightsString(Weight, GoodRootFiles)
    if runOverSkim:
        MakeFilesForSkimDirectory(directory,datasetInfo.originalNumberOfEvents,SkimNumber)    
    else:
        MakeFilesForSkimDirectory(directory,TotalNumber,SkimNumber)
    if not arguments.UseCondor: 
        os.system('mergeTFileServiceHistograms -i ' + InputFileString + ' -o ' + dataSet + '.root' + ' -w ' + InputWeightString)
        os.system('mv ' + dataSet + '.root ' + '../')
        print "\nFinish merging dataset " + dataSet + ":"
        print "    "+ str(len(GoodRootFiles)) + " good files are used for merging out of " + str(len(LogFiles)) + " submitted jobs."
        print "    "+ str(TotalNumber) + " events were successfully run over."
        print "    The target luminosity is " + str(IntLumi) + " inverse pb."
        if crossSection != -1:
            print "    The crossSection of dataset " + dataSet + " is " + str(crossSection) + " pb."
        print "    The weighting factor is " + str(Weight) + "." 
        if crossSection != -1:
    	    print "    " + str(Weight*TotalNumber) + " weighted events and the effective luminosity is " + str(TotalNumber/crossSection) + " inverse pb." 
        print "...............................................................\n"
        os.chdir(CondorDir) 
    #If run on condor, save the parameters in outputInfo_*_cfg.py for condor jobs to use.
    else:
        outputInfoPath = directory + '/outputInfo_' + dataSet + '_cfg.py'
        if os.path.exists(outputInfoPath):
            os.system('rm ' + outputInfoPath)
            os.system('touch ' + outputInfoPath)
        else:
            os.system('touch ' + outputInfoPath)
        configTmp = open(outputInfoPath,'r+w') 
        configTmp.write('InputFileString = "' + InputFileString + '"\n')	
        configTmp.write('InputWeightString = "' + InputWeightString + '"')
        configTmp.close()	

if not arguments.UseCondor:
    for dataSet_component in composite_datasets:
        print "................Merging composite dataset " + dataSet_component + " ................"
        memberList = []
        for dataset in composite_dataset_definitions[dataSet_component]:
            if not os.path.exists(dataset + '.root'):
                print dataset + '.root does not exist, component dataset ' + dataSet_component + ' wont be complete!'
                continue  	
            memberList.append(dataset + '.root')
        InputFileString = MakeInputFileString(memberList)
        os.system('mergeTFileServiceHistograms -i ' + InputFileString + ' -o ' + dataSet_component + '.root')
        print 'Finish merging composite dataset ' + dataSet_component
        print "...............................................................\n"
#Make necessary files for condor and submit condor jobs.
if arguments.UseCondor:
    print '......................Using Condor!...........................'
    GetCompleteOrderedArgumentsSet(InputCondorArguments)
    MakeSubmissionScriptForMerging(CondorDir)
    MakeMergingConfigForCondor(CondorDir)
    os.chdir(CondorDir)
    os.system('condor_submit condorMerging.sub')
