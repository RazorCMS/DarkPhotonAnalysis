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
    bool tchain = false;
    std::string tchain_opt = ParseCommandLine( argc, argv, "-tchain" );

    if (tchain_opt != "") {
        tchain = true;
    }

    std::string inputFiles = ParseCommandLine( argc, argv, "-inputFiles=" );

    if (inputFiles == "" && tchain) {
        std::cerr << "[ERROR]: Please provide input txt file to chain up." << std::endl;
        exit(1);
    }

    else if (inputFiles == "") {
        std::cerr << "[INFO]: No input file provided. Using default /eos/user/u/ufay/2017Data_Jakob/scout_skimmed/scout_skimmed_errored_0.root." << std::endl;
        inputFiles = "/eos/user/u/ufay/2017Data_Jakob/scout_skimmed/scout_skimmed_errored_0.root";
    }

    TTree *tree;

    if (tchain) {
        // Get TTree from files
        tree = makeDataTree(inputFiles.c_str());
    }

    else {
        // Get TTree
        TFile *file = new TFile(inputFiles.c_str(), "read");
        tree = (TTree*) file->Get("tree"); 
    }
    
    tree->GetEntries();

    // Categories
    TString cut = "";

    // Do S+B fitting
    SplusB_fit_test(tree->CopyTree(cut));
}       
    
    

