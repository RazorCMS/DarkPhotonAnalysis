// C++ INCLUDES
#include <string>
#include <iostream>

// ROOT INCLUDES
#include <TTree.h>
#include <TFile.h>

// ROOFIT INCLUDES
#include <RooWorkspace.h>

// LOCAL INCLUDES
#include "fitDarkphoton.h"
#include "CommandLineInput.h"
#include "dataTree.h"

int main(int argc, char* argv[]) {
    gROOT->Reset();

    // Parse command line inputs
    // Display help message
    std::string help = ParseCommandLine( argc, argv, "-help");

    if (help != "") {
        std::cerr << "Flags:" << std::endl;
        std::cerr << "  -help :" << std::endl;
        std::cerr << "      Displays this help message." << std::endl;

        std::cerr << std::endl;
        std::cerr << "  -tchain :" << std::endl;
        std::cerr << "      Specifies to use TChain to get TTree from many files." << std::endl;

        std::cerr << std::endl;
        std::cerr << "  -inputFiles :" << std::endl;
        std::cerr << "      Path to input file to use." << std::endl;
        std::cerr << "      If non specified, uses default file." << std::endl;

        std::cerr << std::endl;
        std::cerr << "  -fitOutFile :" << std::endl;
        std::cerr << "      Path to output txt file to write fit results to." << std::endl;
        std::cerr << "      If non specified, uses default file." << std::endl;

        std::cerr << std::endl;
        std::cerr << "  -imgtag :" << std::endl;
        std::cerr << "      Tag to identify image files, etc." << std::endl;
        std::cerr << "      If non specified, uses default tag." << std::endl;

        std::cerr << std::endl;
        std::cerr << "  -totalEntries :" << std::endl;
        std::cerr << "      Specifies to use the number of entries in the range 0 - 10 GeV" << std::endl;
        std::cerr << "      from the input file for initial values of nsig and nbkg." << std::endl;
        std::cerr << "      If non specified, uses the number of entries in JPsi range." << std::endl;

        std::cerr << std::endl;
        exit(0);
    }

    // Whether input is from several skimmed ntuples
    bool tchain = false;
    std::string tchain_ = ParseCommandLine( argc, argv, "-tchain" );

    if (tchain_ != "") {
        tchain = true;
    }

    // Input file
    std::string inputFiles = ParseCommandLine( argc, argv, "-inputFiles=" );

    if (inputFiles == "" && tchain) {
        std::cerr << "[ERROR]: Please provide input txt file to chain up." << std::endl;
        exit(1);
    }

    else if (inputFiles == "") {
//        inputFiles = "/eos/user/u/ufay/2017Data_Jakob/scout_skimmed_OS/2Dec2018xcg_job0_scout_skimmed.root";
//        inputFiles = "/afs/cern.ch/work/u/ufay/public/Run2017_data/scout_skimmed_errored_0.root";
        inputFiles = "/mnt/hadoop/store/user/idutta/DarkPhoton/Samples/xcg2Dec2018/2Dec2018xcg_job0_scout_skimmed.root";
        std::cerr << "[INFO]: No input file provided. Using default " << inputFiles << std::endl;
    }

    else {
        std::cout << "Input File: " << inputFiles << std::endl;
    }

    // Fit output file
    std::string fitOutFile = ParseCommandLine( argc, argv, "-fitOutFile=" );

    if (fitOutFile == "") {
        fitOutFile = "fitSB_JPsi_output.txt";
        std::cerr << "[INFO]: No fit output file provided. Using default " << fitOutFile << std::endl;
    }

    // Image tag
    std::string imgTag = ParseCommandLine( argc, argv, "-imgtag=" );

    if (imgTag == "") {
        std::cerr << "[INFO]: No image tag provided. Using none as default" << imgTag << std::endl;
    }
    //----------------------------------
    //Getting background function to fit
    //----------------------------------
    std::string f_bkg = ParseCommandLine( argc, argv, "-f_bkg=" );

    if ( f_bkg == "")
    {
        f_bkg = "single_exp";
        std::cerr << "[WARNING]: No bkg function for fitting m_mumu provided . Using default: " << f_bkg << std::endl;
    }
    else
    {
      std::cout << "[INFO]: Using:" << f_bkg  << " as bkg function for fitting m_mumu"<< std::endl;
    }


    // Total entries
    bool totalEntries = false;
    std::string totalEntries_ = ParseCommandLine( argc, argv, "-totalEntries" );

    if (totalEntries_ != "") {
        totalEntries = true;
    }

    // For removing the 'memory-resident TTree' errors
    TFile *temp = new TFile("temp.root", "recreate");
    TTree *tree;

    if (tchain) {
        // Get TTree from files
        tree = makeDataTree(inputFiles.c_str());
    }

    else {
        // Get TTree
        TFile *file = new TFile(inputFiles.c_str(), "read");
        temp->cd();
        tree = (TTree*) file->Get("tree");
    }

    //tree->Print();

    // Categories
    TString cut = "";

    // Do S+B fitting
    SplusB_fit_test(tree->CopyTree(cut), totalEntries, fitOutFile.c_str(), imgTag.c_str(), f_bkg.c_str());
}
