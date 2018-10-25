//#include "sumwgt.h"
#include <string>
#include "TFileCollection.h"
#include "TChain.h"
#include "TFile.h"
#include "TTreeReader.h"
#include "TH1D.h"
#include "TH2D.h"
#include <TTreeReaderValue.h>
#include "TLorentzVector.h"

void trimscout_lowMassFits(string txtfile, const char* outfilename = "test.root", bool isMC=false, bool isData=true) {
    TFile* outfile = new TFile(outfilename, "RECREATE");
    TTree* outtree = new TTree("tree", "tree");


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
    short  category = 0.0;
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
    vector<bool> l1Bools;
    unsigned nvtx  = 0;
    unsigned Run   = 0;
    unsigned LumSec   = 0;

    if (isMC)
    {
      outtree->Branch("mcwgt" , &mcwgt , "mcwgt/F" );
      outtree->Branch("puwgt" , &puwgt , "puwgt/F" );
    }
    outtree->Branch("mass"  , &mass  , "mass/F"  );
    outtree->Branch("category"  , &category  , "category/i"  );
    //outtree->Branch("massCatB"  , &massCatB  , "massCatB/F"  );
    //outtree->Branch("massCatC"  , &massCatC  , "massCatC/F"  );
    outtree->Branch("nvtx"  , &nvtx  , "nvtx/i"  );
    outtree->Branch("Run"   , &Run   , "Run/i"  );
    outtree->Branch("LumSec", &LumSec, "LumSec/i"  );

    // Full mass spectrum
    TH1F* fullMassCatA = new TH1F("massCatA", "massCatA", 1000, 0.2, 12.);
    TH1F* fullMassCatB = new TH1F("massCatB", "massCatB", 1000, 0.2, 12.);
    TH1F* fullMassCatC = new TH1F("massCatC", "massCatC", 1000, 0.2, 12.);

    TH1F* forLimitMassZ = new TH1F("forLimitMassZ","forLimitMassZ",110,10.,120.);
    TH1F* forResolutionAMassZ = new TH1F("forResolutionAMassZ","forResolutionAMassZ",240,60.,120.);
    TH1F* forResolutionBMassZ = new TH1F("forResolutionBMassZ","forResolutionBMassZ",240,60.,120.);
    TH1F* forResolutionCMassZ = new TH1F("forResolutionCMassZ","forResolutionCMassZ",240,60.,120.);
    TH1F* forResolutionAMassJPsi = new TH1F("forResolutionAMassJPsi","forResolutionAMassJPsi",45,2.6,3.5);
    TH1F* forResolutionBMassJPsi = new TH1F("forResolutionBMassJPsi","forResolutionBMassJPsi",45,2.6,3.5);
    TH1F* forResolutionCMassJPsi = new TH1F("forResolutionCMassJPsi","forResolutionCMassJPsi",45,2.6,3.5);

    TH1D *hist_massCatA[5];
    TH1D *hist_massCatB[5];
    TH1D *hist_massCatC[5];

    // Masses
    // Category A
    double m1 = 0.55;
    double m2 = 0.78;
    double m3 = 1.02;
    double m4 = 3.1;
    double m5 = 3.68;

    double masses[] = {m1, m2, m3, m4, m5};
    float frac = 0.2;

    for (int i = 0; i < 5; i++)
    {
      if (i >= 2)
      {
        frac = 0.1;
      }

      hist_massCatA[i] = new TH1D(Form("hist_massCatA%d", i), Form("hist_massCatA%d", i), 100, (1.-frac)*masses[i], (1.+frac)*masses[i]);
      hist_massCatA[i]->Sumw2();
      hist_massCatB[i] = new TH1D(Form("hist_massCatB%d", i), Form("hist_massCatB%d", i), 100, (1.-frac)*masses[i], (1.+frac)*masses[i]);
      hist_massCatB[i]->Sumw2();
      hist_massCatC[i] = new TH1D(Form("hist_massCatC%d", i), Form("hist_massCatC%d", i), 100, (1.-frac)*masses[i], (1.+frac)*masses[i]);
      hist_massCatC[i]->Sumw2();
    }

    string ntuple;
    ifstream file(txtfile);
    TChain* chain = new TChain("mmtree/tree");

    if (file.is_open())
    {
      while (getline(file, ntuple))
      {
        cout << ntuple << endl;
        chain->Add((TString)ntuple.c_str());
        std::cout<<"here2"<< std::endl;
      }
    }
    else
    {
      std::cout << "Unable to open file." << std::endl;
    }
        TTreeReader reader(chain);
        cout<<"here3"<<endl;
        TTreeReaderValue<double>*                      wgt;
        TTreeReaderValue<double>*                      xsec;
        //TTreeReaderValue<std::vector<int>* >            pdgid   ;
        //TTreeReaderValue<std::vector<int>* >            motherid ;

        TTreeReaderValue<std::vector<float> >          mpt      (reader, "muonpt"     );
        TTreeReaderValue<std::vector<float> >          meta     (reader, "muoneta"    );
        TTreeReaderValue<std::vector<float> >          mphi     (reader, "muonphi"    );
        TTreeReaderValue<std::vector<float> >          mcharge  (reader, "muoncharge" );
        TTreeReaderValue<std::vector<char>  >          mid      (reader, "muonid"     );
        TTreeReaderValue<std::vector<float> >          chi2     (reader, "chi2"       );
        TTreeReaderValue<std::vector<float> >          dxy      (reader, "dxy"        );
        TTreeReaderValue<std::vector<float> >          dz       (reader, "dz"         );
        TTreeReaderValue<std::vector<float> >          cpiso    (reader, "cpiso"      );
        TTreeReaderValue<std::vector<float> >          chiso    (reader, "chiso"      );
        TTreeReaderValue<std::vector<float> >          phiso    (reader, "phiso"      );
        TTreeReaderValue<std::vector<float> >          nhiso    (reader, "nhiso"      );
        TTreeReaderValue<std::vector<float> >          puiso    (reader, "puiso"      );
        TTreeReaderValue<std::vector<float> >          tkiso    (reader, "tkiso"      );
        TTreeReaderValue<std::vector<bool> >           l1Result (reader, "l1Result"   );
        TTreeReaderValue<std::vector<unsigned char> >  nmhits   (reader, "nMuonHits"  );
        TTreeReaderValue<std::vector<unsigned char> >  nphits   (reader, "nPixelHits" );
        TTreeReaderValue<std::vector<unsigned char> >  ntklayers(reader, "nTkLayers"  );
        TTreeReaderValue<unsigned char>                hlt      (reader, "trig"       );
        TTreeReaderValue<unsigned>                     nverts   (reader, "nvtx"       );
        cout<<"here4"<<endl;
        TTreeReaderValue<std::vector<float> >          vtxX     (reader, "vtxX"       );
        TTreeReaderValue<std::vector<float> >          vtxY     (reader, "vtxY"       );
        TTreeReaderValue<std::vector<float> >          vtxZ     (reader, "vtxZ"       );
        //TTreeReaderValue<std::vector<float> >          vtxchi2  (reader, "vtxchi2"    );
        cout<<"here5"<<endl;
        TTreeReaderValue<double>                       rho      (reader, "rho"        );
        TTreeReaderValue<unsigned int>                 run      (reader, "run"        );
        TTreeReaderValue<unsigned int>                 lumSec   (reader, "lumSec"     );
        cout<<"here6"<<endl;
        //TTreeReaderValue<std::vector<int> >		   pdgid    (reader, "pdgid" );
        //TTreeReaderValue<std::vector<int> > 	   motherid (reader, "motherid");
        cout<<"here7"<<endl;

        if (isMC || !isData) {
            wgt  = new TTreeReaderValue<double>(reader, "wgt" );
            xsec = new TTreeReaderValue<double>(reader, "xsec");
        }
         cout<<"here8"<<endl;

         wgtsum  = 1.0;//isMC ? sumwgt(treepath) : 1.0;
         puwgt   = 1.0;
         mcwgt   = 1.0;
         m1pt    = 0.0;
         m1eta   = 0.0;
         m1phi   = 0.0;
         m1iso   = 0.0;
         m2pt    = 0.0;
         m2eta   = 0.0;
         m2phi   = 0.0;
         m2iso   = 0.0;
         mass    = 0.0;
         category    = -1;
         //massCatB    = 0.0;
         //massCatC    = 0.0;
         mass4   = 0.0;
         m1id    = 0;
         m2id    = 0;
         m3id    = 0;
         m4id    = 0;
         m1ch    = 0.;
         m2ch    = 0.;
         m3ch    = 0.;
         m4ch    = 0.;
         //vector<bool> l1Bools;
         nvtx  = 0;
         Run   = 0;
         LumSec   = 0;
        //outtree->Branch("l1Bools"  , &l1Bools , "l1Bools" );
        int p=0;
        while(reader.Next())
        {
          //if(!isData[f]) {if(pdgid->size()<2 || motherid->size()<2) continue;}
          if (((*hlt) & 2) == 0) continue;
          bool passIso=false;
          bool passIsoLoose=false;
          double ea = (isMC ? 0.3 : 0.45);
          std::vector<unsigned> goodmuons;
          for (std::size_t i = 0; i < mpt->size(); i++)
          {
            // if ((*nmhits)[i] == 0)     continue;
            if ((*nphits)[i] == 0)     continue;
            if ((*ntklayers)[i] <= 5)  continue;
            if ((*chi2)[i] > 10.)      continue;
            double iso = (*cpiso)[i] + (*nhiso)[i] + (*phiso)[i] - ea*(*rho);
            //	  if (iso > 10.0) continue;

            goodmuons.push_back(i);
          }
          if (goodmuons.size() < 2) continue;
          unsigned idx1 = goodmuons[0];
          unsigned idx2 = goodmuons[1];
          unsigned idx3 = goodmuons[2];
          unsigned idx4 = goodmuons[3];
          if((*tkiso)[goodmuons[0]]<0.02 && (*tkiso)[goodmuons[1]]<0.02 ) passIso=true;
          if((*tkiso)[goodmuons[0]]<0.05 && (*tkiso)[goodmuons[1]]<0.05 ) passIsoLoose=true;
          if ((*mpt)[goodmuons[0]] < (*mpt)[goodmuons[1]])
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
          m1.SetPtEtaPhiM((*mpt)[idx1], (*meta)[idx1], (*mphi)[idx1], 0.1057);
          m2.SetPtEtaPhiM((*mpt)[idx2], (*meta)[idx2], (*mphi)[idx2], 0.1057);
          //m3.SetPtEtaPhiM((*mpt)[idx3], (*meta)[idx3], (*mphi)[idx3], 0.1057);
          //m4.SetPtEtaPhiM((*mpt)[idx4], (*meta)[idx4], (*mphi)[idx4], 0.1057);
          mm += m1;
          mm += m2;
          m1pt   = m1.Pt();
          m1eta  = m1.Eta();
          m1phi  = m1.Phi();
          m1iso  = (*cpiso)[idx1] + (*phiso)[idx1] + (*nhiso)[idx1] - ea*(*rho);
          m1id   = (*mid)[idx1];
          m1ch   = (*mcharge)[idx1];
          m2pt   = m2.Pt();
          m2eta  = m2.Eta();
          m2phi  = m2.Phi();
          m2iso  = (*cpiso)[idx2] + (*phiso)[idx2] + (*nhiso)[idx2] - ea*(*rho);
          m2id   = (*mid)[idx2];
          m2ch   = (*mcharge)[idx2];
          Run=*run;
          LumSec=*lumSec;
          // cout<<"4 Muons in the Event"<<endl;
          /*
          HIGH MASS TRIGGER NO CORRELATION: ID  0 - 4 - 9
'L1_DoubleMu_12_5',
'L1_DoubleMu_12_8',
'L1_DoubleMu_13_6',
'L1_DoubleMu_15_5',
'L1_DoubleMu_15_7',

'L1_TripleMu_4_4_4',
'L1_TripleMu_5_0_0',
'L1_TripleMu_5_3_3',
'L1_TripleMu_5_5_3',
'L1_QuadMu0',

THE LOW MASS TRIGGER: ID 10
'L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4',

THE LOW MASS TRIGGER WITH CORR: 11 - 17
'L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18',
'L1_DoubleMu4_SQ_OS_dR_Max1p2',
'L1_DoubleMu5_SQ_OS_Mass7to18',
'L1_DoubleMu_20_2_SQ_Mass_Max20',

'L1_DoubleMu0er1p4_SQ_OS_dR_Max1p4',

'L1_DoubleMu4p5_SQ_OS_dR_Max1p2',
'L1_DoubleMu6_SQ_OS',

THE LOW MASS TRIGGER TO MEASURE FAKE: ID 18 - 20 [ONLY IN FROM RUN 305405]
'L1_DoubleMu0er1p5_SQ_dR_Max1p4',
'L1_DoubleMu0er2_SQ_dR_Max1p4',
'L1_DoubleMu0_SQ'
	*/

	//'L1_DoubleMu4_SQ_OS_dR_Max1p2', [12]
	//'L1_DoubleMu4p5_SQ_OS_dR_Max1p2', [16]
	//
        mass   = mm.M();
        if( mass > 10. ) continue;
	//	mass4  = mmmm.M();
        nvtx = *nverts;
        //outtree->Fill();

        bool passPVconstraint = false;
        float BSx = 0.084;
        float BSy = -0.03;
//	if(treepath.find("_13TeV") != string::npos){ BSx = -0.029; BSy = 0.07;}
	float slidePt1 = mm.M()/3.;
	float slidePt2 = mm.M()/4.;

	float maxEta=TMath::Max(abs(m1eta),abs(m2eta));


	//cout<<nvtx<<endl;
	if(nvtx>0. )
  {
    if(  sqrt( ((*vtxX)[0] - BSx)*((*vtxX)[0] - BSx) + ((*vtxY)[0] - BSy)*((*vtxY)[0] - BSy) )  < 0.2 ) passPVconstraint = true;
  }


	if(m1ch*m2ch<0. && passPVconstraint && m1pt>slidePt1 && m2pt>slidePt2 && passIsoLoose) forLimitMassZ->Fill(mass);

	if(mass>60. && mass<120.)
  {
    if(m1ch*m2ch<0. && passPVconstraint && m1pt>slidePt1 && m2pt>slidePt2 && maxEta<0.9 && passIsoLoose){ forResolutionAMassZ->Fill(mass); }
    if(m1ch*m2ch<0. && passPVconstraint && m1pt>slidePt1 && m2pt>slidePt2 && maxEta>=0.9 && maxEta<1.2 && passIsoLoose){ forResolutionBMassZ->Fill(mass); }
    if(m1ch*m2ch<0. && passPVconstraint && m1pt>slidePt1 && m2pt>slidePt2 && maxEta>=1.2 && maxEta<2.4 && passIsoLoose){ forResolutionCMassZ->Fill(mass); }
  }


	if(mass>2.6 && mass<3.5)
  {
    if(m1ch*m2ch<0. && passPVconstraint && m1pt>slidePt1 && m2pt>slidePt2 && maxEta<0.9 && 		  passIsoLoose){ forResolutionAMassJPsi->Fill(mass); }
    if(m1ch*m2ch<0. && passPVconstraint && m1pt>slidePt1 && m2pt>slidePt2 && maxEta>=0.9 && maxEta<1.2 && passIsoLoose){ forResolutionBMassJPsi->Fill(mass); }
    if(m1ch*m2ch<0. && passPVconstraint && m1pt>slidePt1 && m2pt>slidePt2 && maxEta>=1.2 && maxEta<2.4 && passIsoLoose){ forResolutionCMassJPsi->Fill(mass); }
  }

  if (mass > 0.2 && mass < 12.)
  {
    if(m1ch*m2ch<0. && passPVconstraint && m1pt>slidePt1 && m2pt>slidePt2 && maxEta<0.9 && passIsoLoose)
    {
      fullMassCatA->Fill(mass);
      category = 0;
    }
  }

  if(m1ch*m2ch<0. && passPVconstraint && m1pt>slidePt1 && m2pt>slidePt2 && maxEta>=0.9 && maxEta<1.2 && passIsoLoose)
  {
    fullMassCatB->Fill(mass);
    category = 1;
  }

  if(m1ch*m2ch<0. && passPVconstraint && m1pt>slidePt1 && m2pt>slidePt2 && maxEta>=1.2 && maxEta<2.4 && passIsoLoose)
  {
    fullMassCatC->Fill(mass);
    category = 2;
  }

  float frac2 = 0.2;
  for (int i = 0; i < 5; i++)
  {
    if (i >= 2)
    {
      frac = 0.1;
    }

    if (mass > masses[i]*(1.-frac) && mass < masses[i]*(1.+frac))
    {
      if(m1ch*m2ch<0. && passPVconstraint && m1pt>slidePt1 && m2pt>slidePt2 && maxEta<0.9 && passIsoLoose)
      {
        hist_massCatA[i]->Fill(mass);
      }
      if(m1ch*m2ch<0. && passPVconstraint && m1pt>slidePt1 && m2pt>slidePt2 && maxEta>=0.9 && maxEta<1.2 && passIsoLoose)
      {
        hist_massCatB[i]->Fill(mass);
      }
      if(m1ch*m2ch<0. && passPVconstraint && m1pt>slidePt1 && m2pt>slidePt2 && maxEta>=1.2 && maxEta<2.4 && passIsoLoose)
      {
        hist_massCatC[i]->Fill(mass);
      }
    }
  }

  outtree->Fill();
}
file.close();
outtree->Write();
forLimitMassZ->Write();
forResolutionAMassZ->Write();
forResolutionBMassZ->Write();
forResolutionCMassZ->Write();
forResolutionAMassJPsi->Write();
forResolutionBMassJPsi->Write();
forResolutionCMassJPsi->Write();

   fullMassCatA->Write();
   fullMassCatB->Write();
   fullMassCatC->Write();

   for (int i = 0; i < 5; i++) {
       hist_massCatA[i]->Write();
       hist_massCatB[i]->Write();
       hist_massCatC[i]->Write();
   }

    outfile->Close();


}
