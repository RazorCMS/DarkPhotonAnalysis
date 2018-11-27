#include <iostream>
#include <string>
#include <fstream>

//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TTree.h>

//LOCAL
#include "dataTree.h"

TTree* makeDataTree(const char* txt_file)
{
    std::string skimmed_tree;
    std::ifstream file(txt_file);
    TChain* chain = new TChain("tree");

    if (file.is_open()) {
        std::cout << "Opening file " << txt_file << std::endl;

        while (std::getline(file, skimmed_tree)) {
            std::cout << "Adding to chain: " << skimmed_tree << std::endl;
            chain->Add(skimmed_tree.c_str());
        }
    }

    else {
        std::cerr << "Unable to open file " << txt_file << std::endl;
        exit(1);
    }
    
    TFile *outfile = new TFile("mergedDataTrees.root", "recreate");
    //TTree *tree = chain->MergeTrees();   

    Long64_t events_chain = chain->GetEntries();
    TTree *tree = chain;
    Long64_t events_tree = tree->GetEntries();
 
    std::cout << "[INFO]: Events in TChain: " << events_chain << std::endl;
    std::cout << "[INFO]: Events in TTree assigned to TChain: " << events_tree << std::endl;
    
    outfile->cd();
    tree->Write();
        
    return tree;
}
