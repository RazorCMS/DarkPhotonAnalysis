import os,sys,re,fileinput,string,shutil

##             Dataset        Name   
datasets = [
#["xaa","xaa"]
  #["xac","xac"] 
 #["xae","xae"]
 #["xag","xag"]
#["xai","xai"]
 #["xak","xak"]
  #["xam","xam"]
  #["xao","xao"]
  #["xaq","xaq"]
  #["xas","xas"]
  #["xau","xau"]
  #["xaw","xaw"]
  #["xay","xay"]
  #["xba","xba"]
  #["xbc","xbc"]
  #["xbe","xbe"]
  #["xbg","xbg"]
  #["xbi","xbi"]
  #["xbk","xbk"]
  #["xbm","xbm"]
  #["xbo","xbo"]
  #["xbq","xbq"]
  #["xbs","xbs"]
  #["xbu","xbu"]
  #["xbw","xbw"]
  #["xby","xby"]
  #["xca","xca"]
  #["xcc","xcc"]
  #["xce","xce"]
  #["xcg","xcg"]
  #["xab","xab"]
  #["xad","xad"]
  #["xaf","xaf"]
  #["xah","xah"]
  #["xaj","xaj"] 
 #["xal","xal"]
  #["xan","xan"]
  #["xap","xap"]
  #["xar","xar"]
  #["xat","xat"]
  #["xav","xav"]
  #["xax","xax"]
  #["xaz","xaz"]
  #["xbb","xbb"]
  #["xbd","xbd"]
  #["xbf","xbf"]
  #["xbh","xbh"]
  #["xbj","xbj"]
  #["xbl","xbl"]
  #["xbn","xbn"]
  #["xbp","xbp"]
  #["xbr","xbr"]
  #["xbt","xbt"]
  #["xbv","xbv"]
  #["xbx","xbx"]
  #["xbz","xbz"]
  #["xcb","xcb"]
  #["xcd","xcd"]
  ["xcf","xcf"]
]

NSections = 10
readFiles = ""

for data in datasets:
    jobidx = 0
    if ( data[0]=="xaa"):
        dataname  = "xaa"
        inputfname = "../ntuples/split/xaa"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 5

    
    elif ( data[0]=="xac"):
        dataname = "xac"
        inputfname = "../ntuples/split/xac"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 5    

    elif ( data[0]=="xae"):
        dataname = "xae"
        inputfname = "../ntuples/split/xae"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 5

    elif ( data[0]=="xag"):
        dataname = "xag"
        inputfname = "../ntuples/split/xag"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 5

    elif ( data[0]=="xai"):
        dataname = "xai"
        inputfname = "../ntuples/split/xai"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10

    elif ( data[0]=="xak"):
        dataname = "xak"
        inputfname = "../ntuples/split/xak"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10

    elif ( data[0]=="xam"):
        dataname = "xam"
        inputfname = "../ntuples/split/xam"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10
    elif ( data[0]=="xao"):
        dataname = "xao"
        inputfname = "../ntuples/split/xao"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10
    
    elif ( data[0]=="xaq"):
        dataname = "xaq"
        inputfname = "../ntuples/split/xaq"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10

        
    elif ( data[0]=="xas"):
        dataname = "xas"
        inputfname = "../ntuples/split/xas"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10



    elif ( data[0]=="xau"):
        dataname = "xau"
        inputfname = "../ntuples/split/xau"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xaw"):
        dataname = "xaw"
        inputfname = "../ntuples/split/xaw"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xay"):
        dataname = "xay"
        inputfname = "../ntuples/split/xay"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xba"):
        dataname = "xba"
        inputfname = "../ntuples/split/xba"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xbc"):
        dataname = "xbc"
        inputfname = "../ntuples/split/xbc"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10

    elif ( data[0]=="xbe"):
        dataname = "xbe"
        inputfname = "../ntuples/split/xbe"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 5

    elif ( data[0]=="xbg"):
        dataname = "xbg"
        inputfname = "../ntuples/split/xbg"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 5

        
    elif ( data[0]=="xbi"):
        dataname = "xbi"
        inputfname = "../ntuples/split/xbi"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10



    elif ( data[0]=="xbk"):
        dataname = "xbk"
        inputfname = "../ntuples/split/xbk"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xbm"):
        dataname = "xbm"
        inputfname = "../ntuples/split/xbm"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xbo"):
        dataname = "xbo"
        inputfname = "../ntuples/split/xbo"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xbq"):
        dataname = "xbq"
        inputfname = "../ntuples/split/xbq"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xbs"):
        dataname = "xbs"
        inputfname = "../ntuples/split/xbs"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10

    elif ( data[0]=="xbu"):
        dataname = "xbu"
        inputfname = "../ntuples/split/xbu"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10

    elif ( data[0]=="xby"):
        dataname = "xby"
        inputfname = "../ntuples/split/xby"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10

    elif ( data[0]=="xbw"):
        dataname = "xbw"
        inputfname = "../ntuples/split/xbw"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10

    elif ( data[0]=="xca"):
        dataname = "xca"
        inputfname = "../ntuples/split/xca"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xcc"):
        dataname = "xcc"
        inputfname = "../ntuples/split/xcc"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xce"):
        dataname = "xce"
        inputfname = "../ntuples/split/xce"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xcg"):
        dataname = "xcg"
        inputfname = "../ntuples/split/xcg"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10

    elif ( data[0]=="xab"):
        dataname = "xab"
        inputfname = "../ntuples/split/xab"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xad"):
        dataname = "xad"
        inputfname = "../ntuples/split/xad"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xaf"):
        dataname = "xaf"
        inputfname = "../ntuples/split/xaf"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10

    elif ( data[0]=="xah"):
        dataname = "xah"
        inputfname = "../ntuples/split/xah"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10

    elif ( data[0]=="xaj"):
        dataname = "xaj"
        inputfname = "../ntuples/split/xaj"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xal"):
        dataname = "xal"
        inputfname = "../ntuples/split/xal"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xan"):
        dataname = "xan"
        inputfname = "../ntuples/split/xan"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10
    
    elif ( data[0]=="xap"):
        dataname = "xap"
        inputfname = "../ntuples/split/xap"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10

    elif ( data[0]=="xar"):
        dataname = "xar"
        inputfname = "../ntuples/split/xar"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xat"):
        dataname = "xat"
        inputfname = "../ntuples/split/xat"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xav"):
        dataname = "xav"
        inputfname = "../ntuples/split/xav"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10

    elif ( data[0]=="xax"):
        dataname = "xax"
        inputfname = "../ntuples/split/xax"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10

    elif ( data[0]=="xaz"):
        dataname = "xaz"
        inputfname = "../ntuples/split/xaz"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xbb"):
        dataname = "xbb"
        inputfname = "../ntuples/split/xbb"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xbd"):
        dataname = "xbd"
        inputfname = "../ntuples/split/xbd"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10

    elif ( data[0]=="xbf"):
        dataname = "xbf"
        inputfname = "../ntuples/split/xbf"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10

    elif ( data[0]=="xbh"):
        dataname = "xbh"
        inputfname = "../ntuples/split/xbh"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xbj"):
        dataname = "xbj"
        inputfname = "../ntuples/split/xbj"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xbl"):
        dataname = "xbl"
        inputfname = "../ntuples/split/xbl"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10

    elif ( data[0]=="xbn"):
        dataname = "xbn"
        inputfname = "../ntuples/split/xbn"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10

    elif ( data[0]=="xbp"):
        dataname = "xbp"
        inputfname = "../ntuples/split/xbp"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xbr"):
        dataname = "xbr"
        inputfname = "../ntuples/split/xbr"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xbt"):
        dataname = "xbt"
        inputfname = "../ntuples/split/xbt"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10

    elif ( data[0]=="xbv"):
        dataname = "xbv"
        inputfname = "../ntuples/split/xbv"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10

    elif ( data[0]=="xbx"):
        dataname = "xbx"
        inputfname = "../ntuples/split/xbx"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xbz"):
        dataname = "xbz"
        inputfname = "../ntuples/split/xbz"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xcb"):
        dataname = "xcb"
        inputfname = "../ntuples/split/xcb"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10

    elif ( data[0]=="xcd"):
        dataname = "xcd"
        inputfname = "../ntuples/split/xcd"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10


    elif ( data[0]=="xcf"):
        dataname = "xcf"
        inputfname = "../ntuples/split/xcf"
        with open(inputfname) as inputfile:
            readFiles = inputfile.readlines()
            print "len(readFiles)", len(readFiles)
        NSections = 10

    NFilesTotal = len(readFiles)
    TotalFiles = NFilesTotal
    print "Dataset ",  data[0], " NFilesTotal ", NFilesTotal
    NFilesDone  = 0

    outDir="/mnt/hadoop/store/user/idutta/DarkPhoton/Samples/"+data[0]+"2Dec2018"
    print outDir
    if not os.path.exists(outDir):
        os.mkdir(outDir)
    while( NFilesDone < NFilesTotal ) :
        thisList = readFiles[NFilesDone : NFilesDone+NSections]
        print "NFilesDone ", NFilesDone, "len(thisList)", len(thisList)

        ##you may have to give full path i.e. CurrentDIR/condor_submit/runlist_...
        inputRunListName = "/data/idutta/CMSSW_9_4_9/src/DarkPhotonAnalysis/condor/condor_submit/runList_"+data[0]+"_"+str(jobidx)+".txt"
        inputRunList = open(inputRunListName, "w")
        for line in thisList:
            inputRunList.write(line)

        condorSubmit = "condor_submit/submitCondor_"+data[0]+"_"+str(jobidx)
        jobName      = "12Jan2019"+data[0]+"_job"+str(jobidx)
        outHistFile = jobName+"_scout_skimmed.root"
        #isData       ="T"
        #isData       ="F"
        shutil.copyfile("proto_condor_submit",condorSubmit)
        for line in fileinput.FileInput(condorSubmit, inplace=1):
            line=line.replace("JOBNAME", jobName)
            line=line.replace("FILELIST",inputRunListName)
            line=line.replace("ROOTOUT",outHistFile)
            #line=line.replace("DATANAME",dataname)
            #line=line.replace("ISDATA",isData)
            line=line.replace("OUTDIR",outDir)
            print line.rstrip()
        
        submitCommand = "condor_submit "+condorSubmit
        print submitCommand
        os.system(submitCommand)     
        jobidx = jobidx+1
        NFilesDone = NFilesDone + len(thisList)

    print "Final NFilesDone ", NFilesDone
