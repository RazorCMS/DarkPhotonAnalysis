import os,sys,re,fileinput,string,shutil

NSections = 10
readFiles = ""
inputfname="mass_hists.txt"
#inputfname="test.txt"
jobidx=0
with open(inputfname) as inputfile:
    readFiles = inputfile.readlines()
    print "len(readFiles)", len(readFiles)
NSections = 1

NFilesTotal = len(readFiles)
TotalFiles = NFilesTotal
print " NFilesTotal ", NFilesTotal
NFilesDone  = 0

outDir="/mnt/hadoop/store/user/idutta/DarkPhoton/Samples/outHist_1p5To5p5_5Feb2019"
print outDir
if not os.path.exists(outDir):
    os.mkdir(outDir)
while( NFilesDone < NFilesTotal ) :
    thisList = readFiles[NFilesDone : NFilesDone+NSections]
    print "NFilesDone ", NFilesDone, "len(thisList)", len(thisList)

    ##you may have to give full path i.e. CurrentDIR/condor_submit/runlist_...
    inputRunListName = "/data/idutta/CMSSW_9_4_9/src/DarkPhotonAnalysis/condor/condor_submit/runList_"+str(jobidx)+".txt"
    inputRunList = open(inputRunListName, "w")
    for line in thisList:
        inputRunList.write(line)
    
    condorSubmit = "condor_submit/submitCondor_"+str(jobidx)
    jobName      = "5Feb2019_1p5To5p5_job"+str(jobidx)
    outHistFile = jobName+"_outHist.root"
    shutil.copyfile("proto_condor_submit_mass",condorSubmit)
    for line in fileinput.FileInput(condorSubmit, inplace=1):
        line=line.replace("JOBNAME", jobName)
        line=line.replace("FILELIST",inputRunListName)
        line=line.replace("ROOTOUT",outHistFile)
        line=line.replace("OUTDIR",outDir)
        print line.rstrip()
        
    submitCommand = "condor_submit "+condorSubmit
    print submitCommand
    os.system(submitCommand)     
       
    jobidx = jobidx+1
    NFilesDone = NFilesDone + len(thisList)

print "Final NFilesDone ", NFilesDone
