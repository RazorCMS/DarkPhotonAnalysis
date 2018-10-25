//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Oct 25 05:31:42 2018 by ROOT version 6.10/09
// from TTree tree/tree
// found on file: /eos/cms/store/group/phys_exotica/darkPhoton/jakob/2017/ScoutingCaloMuon/crab_20180903_080033/180903_060039/0000/scout_96.root
//////////////////////////////////////////////////////////

#ifndef tree_h
#define tree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>

class tree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.
   // Declaration of leaf types
   UInt_t          lumSec;
   UInt_t          run;
   UInt_t          nvtx;
   UChar_t         trig;
   std::vector<bool>    *l1Result;
   //UInt_t          nvtx;
   Double_t        rho;
   std::vector<float>   *vtxX;
   std::vector<float>   *vtxY;
   std::vector<float>   *vtxZ;
   std::vector<float>   *muonpt;
   std::vector<float>   *muoneta;
   std::vector<float>   *muonphi;
   std::vector<float>   *muoncharge;
   std::vector<unsigned char> *nMuonHits;
   std::vector<unsigned char> *nPixelHits;
   std::vector<unsigned char> *nTkLayers;
   std::vector<unsigned char> *nStations;
   std::vector<float>   *chi2;
   std::vector<float>   *dxy;
   std::vector<float>   *dz;
   std::vector<float>   *cpiso;
   std::vector<float>   *chiso;
   std::vector<float>   *nhiso;
   std::vector<float>   *phiso;
   std::vector<float>   *puiso;
   std::vector<float>   *tkiso;
   std::vector<float>   *eciso;
   std::vector<float>   *hciso;
   std::vector<char>    *muonid;
   std::vector<char>    *muonLooseid;

   // List of branches
   TBranch        *b_lumSec;   //!
   TBranch        *b_run;   //!
   TBranch        *b_nvtx;   //!
   TBranch        *b_trig;   //!
   TBranch        *b_l1Result;   //!
   //TBranch        *b_nvtx;   //!
   TBranch        *b_rho;   //!
   TBranch        *b_vtxX;   //!
   TBranch        *b_vtxY;   //!
   TBranch        *b_vtxZ;   //!
   TBranch        *b_muonpt;   //!
   TBranch        *b_muoneta;   //!
   TBranch        *b_muonphi;   //!
   TBranch        *b_muoncharge;   //!
   TBranch        *b_nMuonHits;   //!
   TBranch        *b_nPixelHits;   //!
   TBranch        *b_nTkLayers;   //!
   TBranch        *b_nStations;   //!
   TBranch        *b_chi2;   //!
   TBranch        *b_dxy;   //!
   TBranch        *b_dz;   //!
   TBranch        *b_cpiso;   //!
   TBranch        *b_chiso;   //!
   TBranch        *b_nhiso;   //!
   TBranch        *b_phiso;   //!
   TBranch        *b_puiso;   //!
   TBranch        *b_tkiso;   //!
   TBranch        *b_eciso;   //!
   TBranch        *b_hciso;   //!
   TBranch        *b_muonid;   //!
   TBranch        *b_muonLooseid;   //!

   tree(TTree *tree=0);
   virtual ~tree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef tree_cxx
tree::tree(TTree *tree) : fChain(0)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/eos/cms/store/group/phys_exotica/darkPhoton/jakob/2017/ScoutingCaloMuon/crab_20180903_080033/180903_060039/0000/scout_96.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/eos/cms/store/group/phys_exotica/darkPhoton/jakob/2017/ScoutingCaloMuon/crab_20180903_080033/180903_060039/0000/scout_96.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("/eos/cms/store/group/phys_exotica/darkPhoton/jakob/2017/ScoutingCaloMuon/crab_20180903_080033/180903_060039/0000/scout_96.root:/mmtree");
      dir->GetObject("tree",tree);

   }
   Init(tree);
}

tree::~tree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t tree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t tree::LoadTree(Long64_t entry)
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

void tree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   l1Result = 0;
   vtxX = 0;
   vtxY = 0;
   vtxZ = 0;
   muonpt = 0;
   muoneta = 0;
   muonphi = 0;
   muoncharge = 0;
   nMuonHits = 0;
   nPixelHits = 0;
   nTkLayers = 0;
   nStations = 0;
   chi2 = 0;
   dxy = 0;
   dz = 0;
   cpiso = 0;
   chiso = 0;
   nhiso = 0;
   phiso = 0;
   puiso = 0;
   tkiso = 0;
   eciso = 0;
   hciso = 0;
   muonid = 0;
   muonLooseid = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("lumSec", &lumSec, &b_lumSec);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("nvtx", &nvtx, &b_nvtx);
   fChain->SetBranchAddress("trig", &trig, &b_trig);
   fChain->SetBranchAddress("l1Result", &l1Result, &b_l1Result);
//    fChain->SetBranchAddress("nvtx", &nvtx, &b_nvtx);
   fChain->SetBranchAddress("rho", &rho, &b_rho);
   fChain->SetBranchAddress("vtxX", &vtxX, &b_vtxX);
   fChain->SetBranchAddress("vtxY", &vtxY, &b_vtxY);
   fChain->SetBranchAddress("vtxZ", &vtxZ, &b_vtxZ);
   fChain->SetBranchAddress("muonpt", &muonpt, &b_muonpt);
   fChain->SetBranchAddress("muoneta", &muoneta, &b_muoneta);
   fChain->SetBranchAddress("muonphi", &muonphi, &b_muonphi);
   fChain->SetBranchAddress("muoncharge", &muoncharge, &b_muoncharge);
   fChain->SetBranchAddress("nMuonHits", &nMuonHits, &b_nMuonHits);
   fChain->SetBranchAddress("nPixelHits", &nPixelHits, &b_nPixelHits);
   fChain->SetBranchAddress("nTkLayers", &nTkLayers, &b_nTkLayers);
   fChain->SetBranchAddress("nStations", &nStations, &b_nStations);
   fChain->SetBranchAddress("chi2", &chi2, &b_chi2);
   fChain->SetBranchAddress("dxy", &dxy, &b_dxy);
   fChain->SetBranchAddress("dz", &dz, &b_dz);
   fChain->SetBranchAddress("cpiso", &cpiso, &b_cpiso);
   fChain->SetBranchAddress("chiso", &chiso, &b_chiso);
   fChain->SetBranchAddress("nhiso", &nhiso, &b_nhiso);
   fChain->SetBranchAddress("phiso", &phiso, &b_phiso);
   fChain->SetBranchAddress("puiso", &puiso, &b_puiso);
   fChain->SetBranchAddress("tkiso", &tkiso, &b_tkiso);
   fChain->SetBranchAddress("eciso", &eciso, &b_eciso);
   fChain->SetBranchAddress("hciso", &hciso, &b_hciso);
   fChain->SetBranchAddress("muonid", &muonid, &b_muonid);
   fChain->SetBranchAddress("muonLooseid", &muonLooseid, &b_muonLooseid);
   Notify();
}

Bool_t tree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void tree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t tree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef tree_cxx
