#include <iostream>
#include <fstream>
#include <string>

// ROOT
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>

// LOCAL
#include <subTree.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "usage: " << argv[0] << " subentries [skimmed_ntuples] [-tchain] [txt_file]" << std::endl;
        std::cout << "subentries = -1 denotes all entries" << std::endl;
        exit(1);
    }

    subTree *tree = 0;
    Long64_t subentries = std::atoll(argv[1]);
    std::cout << "Subentries: " << subentries << std::endl;

    if (argc > 2) {
        bool tchain = 0;
        TChain *chain = new TChain("tree");

        if (strcmp(argv[2], "-tchain") == 0) {
            tchain = 1;
        }
        
        if (tchain) {
            std::string skimmed_ntuple;
            std::ifstream file(argv[3]);

            if (file.is_open()) {
                std::cout << "Opening file " << argv[3] << std::endl;

                while (std::getline(file, skimmed_ntuple)) {
                    std::cout << "Adding to chain: " << skimmed_ntuple << std::endl;
                    chain->Add(skimmed_ntuple.c_str());
                }
            }

            else {
                std::cerr << "Unable to open file " << argv[3] << std::endl;
                exit(1);
            }
        }

        else {
            std::cout <<"Adding to chain: " << argv[2] << std::endl;
            chain->Add(argv[2]);
        }
        
        tree = new subTree(chain);
    }

    else {
        tree = new subTree();
    }

    tree->Loop(subentries);
    return 0;
}
