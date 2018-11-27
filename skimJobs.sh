#!/bin/bash

txtfile=$1
outfilename=$2
repoDir=/afs/cern.ch/work/u/ufay/public/CMSSW_9_4_0_patch1/src/DarkPhotonAnalysis

echo "txtfile: $1"
echo "outfilename: $2"
echo "repo directory: $repoDir"

cd $repoDir
pwd
eval `scramv1 runtime -sh`

make
./create_reduced_tree $1 $2
make clean


