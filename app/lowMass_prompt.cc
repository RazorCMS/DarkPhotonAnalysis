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
        std::cerr << "[ERROR]: please provide an input file using --inputFile=<path_to_file>" << std::endl;
        return -1;
    }

    std::string sigFit = ParseCommandLine ( argc, argv, "-sigFit=" );
    if ( sigFit == "" ) {
        std::cerr << "[INFO]: sigFit has not been set, using default sigFit = bw" << std::endl;
        sigFit = "bw";
    }

    std::string bkgFit = ParseCommandLine( argc, argv, "-bkgFit=" );
    if (bkgFit == "") {
        std::cerr << "[INFO]: bkgFit has not been set, using default bkgFit = expo" << std::endl;
        sigFit = "expo";
    }

    std::cout << std::endl;
    std::cout << "[INFO]: sigFit: " << sigFit << std::endl;
    std::cout << "[INFO]: bkgFit: " << bkgFit << std::endl;
    std::cout << std::endl;

    // Get files
    TFile *file = new TFile(inputFile.c_str(), "read");
    TTree *tree = (TTree*) file->Get("tree"); 

    // Do fit
    SplusB_fit(tree, sigFit, bkgFit);
}       
    
    

