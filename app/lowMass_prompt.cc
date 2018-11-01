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

int main(int argc, char* argv[]) {
    gROOT->Reset();

    // Parse command line inputs
    std::string inputFile = ParseCommandLine( argc, argv, "-inputFile=" );
    if (  inputFile == "" ) {
        std::cerr << "[INFO]: No input file provided. Using default /eos/user/u/ufay/2017Data_Jakob/scout_skimmed/scout_skimmed_errored_0.root." << std::endl;
        inputFile = "/eos/user/u/ufay/2017Data_Jakob/scout_skimmed/scout_skimmed_errored_0.root";
    }

    std::string sigFit = ParseCommandLine ( argc, argv, "-sigFit=" );
    if ( sigFit == "" ) {
//        std::cerr << "[INFO]: sigFit has not been set, using default sigFit = bw" << std::endl;
        sigFit = "bw";
    }

    std::string bkgFit = ParseCommandLine( argc, argv, "-bkgFit=" );
    if (bkgFit == "") {
//        std::cerr << "[INFO]: bkgFit has not been set, using default bkgFit = expo" << std::endl;
        bkgFit = "expo";
    }

    /*
    std::cout << std::endl;
    std::cout << "[INFO]: sigFit: " << sigFit << std::endl;
    std::cout << "[INFO]: bkgFit: " << bkgFit << std::endl;
    std::cout << std::endl;
    */

    // Get files
    TFile *file = new TFile(inputFile.c_str(), "read");
    TTree *tree = (TTree*) file->Get("tree"); 
    
    // Categories
    TString cut = "category==0";

    // Do fit for Category A
    // For now, sigFit and bkgFit input have no influence in how fit is done
    SplusB_fit(tree->CopyTree(cut), sigFit, bkgFit);
}       
    
    

