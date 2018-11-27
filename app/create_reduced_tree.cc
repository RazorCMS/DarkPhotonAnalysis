#include <iostream>
#include <fstream>
#include <string>

//ROOT
#include <TFile.h>
#include <TTree.h>

//LOCAL
#include <tree.h>
#include "Filename.h"

int main( int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "usage: " << argv[0] << " txt_file outfilepath" << std::endl;
        exit(1);
    }
    
    std::string ntuple;
    std::ifstream file(argv[1]);
    TChain* chain = new TChain("mmtree/tree");

    if (file.is_open()) {
        std::cout << "Opening file " << argv[1] << std::endl;

        while (std::getline(file, ntuple)) {
            std::cout << "Adding to chain: " << ntuple << std::endl;
            chain->Add(ntuple.c_str());
        }
    }

    else {
        std::cerr << "Unable to open file " << argv[1] << std::endl;
        exit(1);
    }
  
    tree* mytree = new tree(chain);
    mytree->Loop(argv[2]);
    return 0;
}
