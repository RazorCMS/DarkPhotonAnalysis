#define tree_cxx
#include <iostream>
#include "tree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>

void tree::Loop(const char* outfilename="test.txt")
{
  TFile* outfile = new TFile(outfilename, "RECREATE");
  TTree* outtree = new TTree("tree", "tree");
  bool isMC = false;

  double wgtsum  = 1.0;//isMC ? sumwgt(treepath) : 1.0;
  float  puwgt   = 1.0;
  float  mcwgt   = 1.0;
  float  m1pt    = 0.0;
  float  m1eta   = 0.0;
  float  m1phi   = 0.0;
  float  m1iso   = 0.0;
  float  m2pt    = 0.0;
  float  m2eta   = 0.0;
  float  m2phi   = 0.0;
  float  m2iso   = 0.0;
  float  mass    = 0.0;
  unsigned char  category = 255;
  //float  massCatB    = 0.0;
  //float  massCatC    = 0.0;
  float  mass4   = 0.0;
  char   m1id    = 0;
  char   m2id    = 0;
  char   m3id    = 0;
  char   m4id    = 0;
  float  m1ch    = 0.;
  float  m2ch    = 0.;
  float  m3ch    = 0.;
  float  m4ch    = 0.;
  std::vector<bool> l1Bools;
  //unsigned nvtx  = 0;
  unsigned Run   = 0;
  unsigned LumSec   = 0;

  if (isMC)
  {
    outtree->Branch("mcwgt" , &mcwgt , "mcwgt/F" );
    outtree->Branch("puwgt" , &puwgt , "puwgt/F" );
  }
  outtree->Branch("mass"  , &mass  , "mass/F"  );
  outtree->Branch("category"  , &category  , "category/b"  );
  //outtree->Branch("massCatB"  , &massCatB  , "massCatB/F"  );
  //outtree->Branch("massCatC"  , &massCatC  , "massCatC/F"  );
  //outtree->Branch("nvtx"  , &nvtx  , "nvtx/i"  );
  //outtree->Branch("Run"   , &Run   , "Run/i"  );
  //outtree->Branch("LumSec", &LumSec, "LumSec/i"  );
  if (fChain == 0) return;
  int removed = 0;
  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++)
  {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if( jentry % 100000 == 0 )
    {
      std::cout <<  "==entry: " << jentry << std::endl;
      //std::cout << (int)(trig) << std::endl;
    }
    if ((trig & 2) == 0)//trigger bit is 0x00000010 or 2 in integer
    {
      removed++;
      continue;
    }
    bool passIso=false;
    bool passIsoLoose=false;
    double ea = (isMC ? 0.3 : 0.45);
    std::vector<unsigned> goodmuons;
    for (std::size_t i = 0; i < muonpt->size(); i++)
    {
      // if ((*nmhits)->at(i) == 0)     continue;
      if ( (int)(nPixelHits->at(i)) == 0 ) continue;
      if ( (int)(nTkLayers->at(i)) <= 5 ) continue;
      if ( chi2->at(i) > 10. ) continue;
      if ( fabs(muoneta->at(i)) > 2.4 ) continue;
      double iso = cpiso->at(i) + nhiso->at(i) + phiso->at(i) - ea*rho;
      //std::cout << nPixelHits->at(i) << std::endl;
      //	  if (iso > 10.0) continue;
      goodmuons.push_back(i);
    }

    if (goodmuons.size() < 2) continue;
    unsigned idx1 = goodmuons[0];
    unsigned idx2 = goodmuons[1];
    unsigned idx3 = goodmuons[2];
    unsigned idx4 = goodmuons[3];
    if( tkiso->at(goodmuons[0]) < 0.02 && tkiso->at(goodmuons[1]) < 0.02 ) passIso = true;
    if( tkiso->at(goodmuons[0]) < 0.05 && tkiso->at(goodmuons[1]) < 0.05 ) passIsoLoose = true;
    if( muonpt->at(goodmuons[0]) < muonpt->at(goodmuons[1]) )
    {
      idx1 = goodmuons[1];
      idx2 = goodmuons[0];
    }

    TLorentzVector mm;
    TLorentzVector mmmm;
    TLorentzVector m1;
    TLorentzVector m2;
    TLorentzVector m3;
    TLorentzVector m4;
    m1.SetPtEtaPhiM(muonpt->at(idx1), muoneta->at(idx1), muonphi->at(idx1), 0.1057);
    m2.SetPtEtaPhiM(muonpt->at(idx2), muoneta->at(idx2), muonphi->at(idx2), 0.1057);
    //m3.SetPtEtaPhiM((*mpt)[idx3], (*meta)[idx3], (*mphi)[idx3], 0.1057);
    //m4.SetPtEtaPhiM((*mpt)[idx4], (*meta)[idx4], (*mphi)[idx4], 0.1057);
    mm += m1;
    mm += m2;
    m1pt   = m1.Pt();
    m1eta  = m1.Eta();
    m1phi  = m1.Phi();
    m1iso  = cpiso->at(idx1) + phiso->at(idx1) + nhiso->at(idx1) - ea*rho;
    m1id   = muonid->at(idx1);
    m1ch   = muoncharge->at(idx1);
    m2pt   = m2.Pt();
    m2eta  = m2.Eta();
    m2phi  = m2.Phi();
    m2iso  = cpiso->at(idx2) + phiso->at(idx2) + nhiso->at(idx2) - ea*rho;
    m2id   = muonid->at(idx2);
    m2ch   = muoncharge->at(idx2);
    Run    = run;
    LumSec = lumSec;
    if(m1ch*m2ch!=-1)continue;
    mass   = mm.M();
    if( mass > 100. ) continue;

    //nvtx = nverts;
    bool passPVconstraint = false;
    float BSx = 0.084;
    float BSy = -0.03;
    //	if(treepath.find("_13TeV") != string::npos){ BSx = -0.029; BSy = 0.07;}
    //Baseline muon pt cut from TRIGGER
    if( m1pt < 3.0 || m2pt < 3.0 ) continue;
    float slidePt1 = mm.M()/3.;
    float slidePt2 = mm.M()/4.;
    if ( m1pt < slidePt1 || m2pt < slidePt2 ) continue;
    float maxEta = TMath::Max( fabs(m1eta), fabs(m2eta));
    if( nvtx > 0. )
    {
      if(  sqrt( ((*vtxX)[0] - BSx)*((*vtxX)[0] - BSx) + ((*vtxY)[0] - BSy)*((*vtxY)[0] - BSy) )  < 0.2 ) passPVconstraint = true;
    }

    //make double sure that selected muons are within acceptance
    if( fabs(m1eta) > 2.4 || fabs(m2eta) > 2.4 ) continue;

    if ( maxEta < 0.9) category = 0;
    else if ( maxEta < 1.2 ) category = 1;
    else if ( maxEta < 2.4 ) category = 2;
    else
    {
      std::cout << "eta category not found, THERE MUST BE A BUUUG!!!" << std::endl;
    }

    //Fill TTree
    outtree->Fill();
  }
  std::cout << "removed " << removed << " events" << std::endl;
  outtree->Write();
  outfile->Close();

};
