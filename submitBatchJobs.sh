#!/bin/bash

# Command line arguments: directory with txt files
# directory with txt files is ntuples/split

script=skimJobs.sh
eos=/eos/user/u/ufay/2017Data_Jakob/scout_skimmed

if [ -z "$1" ]
then 
    echo "No file directory given"
    exit 1
fi

chmod 755 ${script} 

files=`ls $1`
count=0

echo "files: $files"

for file in $files
do 
    echo "txtfile: $file"
    inputFile=$1/${file}
    outputFile=${eos}/scout_skimmed_${count}.root
    bsub -o skimmed_out/out.%J -q 8nh ${script} ${inputFile} ${outputFile}
    count=`expr $count + 1`
done
