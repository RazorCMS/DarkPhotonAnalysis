# DarkPhotonAnalysis

## Prompt Low Mass
### Getting the Skimmed Ntuples
To run:
```
make 
./create_reduced_tree txt_file outfilepath
```

`txt_file` is a text file containing the LFN of the Ntuples. In the directory `ntuples/split` are text files containing ~30 Ntuples each. So for example, we can do:
```
./create_reduced_tree ntuples/split/xaa scout_skimmed_1.root
```
to get a skimmed Ntuple `scout_skimmed_1.root` from the Ntuples in file `xaa`.

The files `submitBatchJobs.sh` and `skimJobs.sh` are used to submit batch jobs to do the skimming. `submitBatchJobs.sh` takes the path to the directory containing the text files as a command-line argument. `skimJobs.sh` is the bash script submitted to batch.

You can run it with:
```
. submitBatchJobs.sh ntuples/split
```

### Fitting
The executable to do the fitting is `./lowMass_prompt`. There are two ways to run it:

1. Running on a single skimmed Ntuple
    ```
    make
    ./lowMass_prompt -inputFiles=inputfile
    ```
    If `-inputFiles` is not specified, the default skimmed Ntuple used is `/mnt/hadoop/store/user/idutta/DarkPhoton/Samples/xcg2Dec2018/2Dec2018xcg_job0_scout_skimmed.root`.

2. Running on multiple skimmed Ntuples
    ```
    make
    ./lowMass_prompt -tchain -inputFiles=scout_skimmed.txt
    ```
    In this case `scout_skimmed.txt` is a text file containing the LFN of the skimmed Ntuples. The `-tchain` flag specifies that a TChain is used to create a TTree from multiple ROOT files.

### Additional Optional Flags
`-help`
Displays a help message with information about the flags.

`-fitOutFile`
Used to specify the path to the txt file to write fit results. Default path is `fitSB_JPsi_output.txt`.  
Example:
```
./lowMass_prompt -fitOutFile=fit.txt
```

`-imgtag`
Used to specify tag for identifying output image file and ROOT file storing RooWorkspace. No tags are used as default.  
Example:
```
./lowMass_prompt -imgtag=allEvents
```

`-totalEntries`
Specifies program to use the number of entries in the range 0 - 10 GeV from the input file for setting initial values of `nsig` and `nbkg`. Default uses the number of entries in the JPsi range (2.0 - 3.5 GeV).  
Example:
```
./lowMass_prompt -totalEntries
```

### Skimmed NTuples
```
/mnt/hadoop/store/user/ufay/lowMassPromptDP/subTree_500000.root
/mnt/hadoop/store/user/ufay/lowMassPromptDP/subTree_1000000.root
```
These are smaller NTuples containing 500 000 and 1 000 000 total entries respectively in the whole range 0 - 10 GeV.

## Displaced Low Mass Limits 
To run:
```
make getLimits
./getLimits --inputFileSig=<input signal ROOT file path> --inputFileBkg=<input bkg ROOT file path> \
  --treeName=tree --outputFile=<output ROOT file name> --datacard=<datacard file path> \
  --mzd=<mzd [GeV]> --tau0=<tau0 [mm]> --binNumber=<binNumber> --vtxCut=<vertex cut [cm]> \
  --sigFit=<signal fit PDF> --bkgFit=<bkg fit PDF> > output.txt
```

Example:
```
make getLimits
./getLimits --inputFileSig=../trimscoutV2/all-trimscoutV2-darkphoton_mzd_20_tau0_100mm_full.root \
  --inputFileBkg=../trimscoutV2/all-trimscoutV2-darkphoton_mzd_20_tau0_1000mm_full.root \
  --treeName=tree --outputFile=combine10.root --datacard=datacards/combineDatacard10.txt \
  --mzd=20 --tau0=100 --binNumber=1 --vtxCut=10 \
  --sigFit=bw --bkgFit=expo > output.txt
```

This creates two new directories `output` and `datacards` in the current directory. `output` contains plots of the background and signal fits (`bkgFit_bin1_cut_10cm.png` and `sigFit_bin1_cut_10cm.png`), and the toy data thrown from the fitted PDF (`dataToy_bin1_cut_10cm.png`). `datacards` contains the combine datacard (`combineDatacard10.txt`) and the ROOT file with the RooWorkspace for the combine input (`combine10.root`). 

The datacard can then be put into combine with 
```
combine -M Asymptotic datacards/combineDatacard10.txt --minimizerStrategy=1 --X-rtd ADDNLL_RECURSIVE=0 > limit.txt
```

To get the correct normalization for the signal shape, the number of signal events without any cut must be changed for each lifetime. This is done by changing the `N_sigNoCut` variable here:
https://github.com/shufay/DarkPhotonAnalysis/blob/master/src/fitDarkphoton.cc#L752.

The signal yield without any cut should also be changed for the different background fits used. This is done by changing the `sigYieldNoCut` variable here: 
https://github.com/shufay/DarkPhotonAnalysis/blob/master/src/fitDarkphoton.cc#L751.

The implemented PDFs for fitting could be specified with the following:
   * Exponential ![equation](http://www.sciweavers.org/tex2img.php?eq=Ne%5E%7B%5Clambda%20x%7D%0A&bc=White&fc=Black&im=jpg&fs=12&ff=modern&edit=0) — expo 
   * Double Exponential ![equation](http://www.sciweavers.org/tex2img.php?eq=N%5Bfe%5E%7B%5Clambda_1%20x%7D%20%2B%20%281-f%29e%5E%7B%5Clambda_2%20x%7D%5D%0A&bc=White&fc=Black&im=jpg&fs=12&ff=modern&edit=0) — doubleExpo
   * Power ![equation](http://www.sciweavers.org/tex2img.php?eq=Nx%5E%5Calpha%0A&bc=White&fc=Black&im=jpg&fs=12&ff=modern&edit=0) — pow
   * Double Power ![equation](http://www.sciweavers.org/tex2img.php?eq=N%5Bfx%5E%7B%5Calpha_1%7D%20%2B%20%281-f%29x%5E%7B%5Calpha_2%7D%5D%0A%0A&bc=White&fc=Black&im=jpg&fs=12&ff=modern&edit=0) — doublePow
   * 2nd Order Bernstein Polynomial — bernPoly2
   * 3rd Order Bernstein Polynomial — bernPoly3
   * 2nd Order Chebychev Polynomial — chebPoly2
   
## Input Files
### Signal
#### tau<sub>0</sub> = 10e-10 mm
`/afs/cern.ch/work/u/ufay/public/CMSSW_9_4_0_patch1/src/trimscoutV2/all-trimscoutV2-darkphoton_mzd_20_tau0_10e-10mm_full.root`

#### tau<sub>0</sub> = 1 mm
`/afs/cern.ch/work/u/ufay/public/CMSSW_9_4_0_patch1/src/trimscoutV2/all-trimscoutV2-darkphoton_mzd_20_tau0_1mm_full.root`

#### tau<sub>0</sub> = 10 mm
`/afs/cern.ch/work/u/ufay/public/CMSSW_9_4_0_patch1/src/trimscoutV2/all-trimscoutV2-darkphoton_mzd_20_tau0_10mm_full.root`

#### tau<sub>0</sub> = 50 mm
`/afs/cern.ch/work/u/ufay/public/CMSSW_9_4_0_patch1/src/trimscoutV2/all-trimscoutV2-darkphoton_mzd_20_tau0_50mm_full.root`

#### tau<sub>0</sub> = 100 mm
`/afs/cern.ch/work/u/ufay/public/CMSSW_9_4_0_patch1/src/trimscoutV2/all-trimscoutV2-darkphoton_mzd_20_tau0_100mm_full.root`

#### tau<sub>0</sub> = 1000 mm
`/afs/cern.ch/work/u/ufay/public/CMSSW_9_4_0_patch1/src/trimscoutV2/all-trimscoutV2-darkphoton_mzd_20_tau0_1000mm_full.root`

### Background
`/afs/cern.ch/work/u/ufay/public/CMSSW_9_4_0_patch1/src/trimscoutV2/all-trimscoutV2_2017C_primaryVtx_sub_full.root`
