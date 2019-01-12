//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Dec 19 07:43:26 2018 by ROOT version 6.10/09
// from TTree tree/tree
// found on file: /mnt/hadoop/store/user/idutta/DarkPhoton/Samples/xcg2Dec2018/2Dec2018xcg_job0_scout_skimmed.root
//////////////////////////////////////////////////////////

#ifndef subTree_h
#define subTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class subTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Float_t         mass;
   UChar_t         category;

   // List of branches
   TBranch        *b_mass;   //!
   TBranch        *b_category;   //!

   subTree(TTree *tree=0);
   virtual ~subTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(const Long64_t &subentries);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef subTree_cxx
subTree::subTree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/mnt/hadoop/store/user/idutta/DarkPhoton/Samples/xcg2Dec2018/2Dec2018xcg_job0_scout_skimmed.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/mnt/hadoop/store/user/idutta/DarkPhoton/Samples/xcg2Dec2018/2Dec2018xcg_job0_scout_skimmed.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

subTree::~subTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t subTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t subTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void subTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("mass", &mass, &b_mass);
   fChain->SetBranchAddress("category", &category, &b_category);
   Notify();
}

Bool_t subTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void subTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t subTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef subTree_cxx
