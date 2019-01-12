#define subTree_cxx
#include "subTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <sys/stat.h>
#include <errno.h>

void subTree::Loop(const Long64_t &subentries)
{
//   In a ROOT session, you can do:
//      root> .L subTree.C
//      root> subTree t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   const char *dirname = "subTrees";
   int dir_err = mkdir(dirname, S_IRUSR | S_IWUSR | S_IXUSR);

   if (errno == EEXIST) {
       std::cout << "Directory '" << dirname << "' exists" << std::endl;
   }

   else if (dir_err) {
       std::cout << "[ERROR] Unable to create directory '" << dirname << "'" << std::endl;
       exit(1);
   }

   else {
       std::cout << "Directory '" << dirname << "' created" << std:: endl;
   }

   TFile *outfile = new TFile(Form("%s/subTree_%d.root", dirname, subentries), "recreate");
   TTree *outtree = new TTree("tree", "tree");
   
   float mass2 = 0.0;
   unsigned char category2 = 255;

   outtree->Branch("mass", &mass2, "mass/F");
   outtree->Branch("category", &category2, "category/b");

   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   
   for (Long64_t jentry=0; jentry<nentries; jentry++) {
      Long64_t ientry = LoadTree(jentry);
   
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      
      if (jentry % 10000 == 0) {
         std::cout << "==entry: " << jentry << std::endl;
      }

      if (subentries != -1 && jentry > subentries - 1) break;
    
      mass2 = mass;
      category2 = category;
      outtree->Fill();
   }

   outtree->Write();
   outfile->Close();
}
