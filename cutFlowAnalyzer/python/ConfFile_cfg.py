import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

inputFile = "simpleAnalyzer/cutFlowAnalyzer/filelists/aaTo4b_M-12.txt"
#inputFile = "simpleAnalyzer/cutFlowAnalyzer/filelists/aaTo4b_M-15.txt"
#inputFile = "simpleAnalyzer/cutFlowAnalyzer/filelists/aaTo4b_M-20.txt"
#inputFile = "simpleAnalyzer/cutFlowAnalyzer/filelists/aaTo4b_M-25.txt"
#inputFile = "simpleAnalyzer/cutFlowAnalyzer/filelists/aaTo4b_M-30.txt"
#inputFile = "simpleAnalyzer/cutFlowAnalyzer/filelists/aaTo4b_M-40.txt"
#inputFile = "simpleAnalyzer/cutFlowAnalyzer/filelists/aaTo4b_M-50.txt"
#inputFile = "simpleAnalyzer/cutFlowAnalyzer/filelists/aaTo4b_M-60.txt"
filelist = []
with open(inputFile) as f:
    for line in f:
        line = line.strip()
        if line == '': continue
        if line[0] == '#': continue
        filelist.append(line)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(filelist)
)

process.demo = cms.EDAnalyzer('cutFlowAnalyzer',
    triggerResults = cms.InputTag("TriggerResults","","HLT")
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('histo.root')
)

process.p = cms.Path(process.demo)
