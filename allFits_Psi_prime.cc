#include "RooRealVar.h"
 #include "RooDataSet.h"
 #include "RooDataHist.h"
 #include "RooGaussian.h"
 #include "RooConstVar.h"
 #include "TCanvas.h"
 #include "TAxis.h"
 #include "RooPlot.h"
 #include "RooWorkspace.h"
 #include "RooExpensiveObjectCache.h"
 #include "TFile.h"
 #include "TH1.h"
#include "TLegend.h"
#include "./include/CustomPdfs.hh" 
using namespace RooFit;
int main(){
//void allFits(){
  //gSystem->Load("./include/libCustomPdfs.so");
  TFile* f1 = new TFile("LowMassFits_Psi_prime_single_exp.root") ;
  TFile* f2 = new TFile("LowMassFits_Psi_prime_double_exp.root") ;
  TFile* f3 = new TFile("LowMassFits_Psi_prime_single_pow.root") ;
  TFile* f4 = new TFile("LowMassFits_Psi_prime_poly2.root") ;
  TFile* f5 = new TFile("LowMassFits_Psi_prime_poly3.root") ;
  TFile* f6 = new TFile("LowMassFits_Psi_prime_poly4.root") ;
  RooWorkspace* w1 = (RooWorkspace*) f1->Get("LowMassFits") ;
  RooWorkspace* w2 = (RooWorkspace*) f2->Get("LowMassFits") ;
  RooWorkspace* w3 = (RooWorkspace*) f3->Get("LowMassFits") ;
  RooWorkspace* w4 = (RooWorkspace*) f4->Get("LowMassFits") ;
  RooWorkspace* w5 = (RooWorkspace*) f5->Get("LowMassFits") ;
  RooWorkspace* w6 = (RooWorkspace*) f6->Get("LowMassFits") ;

  RooRealVar m_mumu("mass", "m_{#mu#mu}", 1, 10., "GeV");
  m_mumu.setRange("Psi_p", 3.3, 5.5);
  m_mumu.setRange("sig_plot", 3.5, 3.8);
  TCanvas *csb = new TCanvas("csb", "csb", 900, 600);
  RooPlot *frameSB = new RooPlot("frameSB", "frameSB", m_mumu, 3.3, 5.5, 880);
  RooPlot *frameS = new RooPlot("frameS", "frameS", m_mumu, 3.3, 5.5, 880);
  w1->data("binned_mass")->plotOn(frameSB);
  //w1->data("binned_mass")->plotOn(frameS);
  w1->pdf("sb_model")->plotOn(frameSB, RooFit::Name("single_exp"), RooFit::LineColor(kBlue), RooFit::Range("full"), RooFit::NormRange("full"));
  w2->pdf("sb_model")->plotOn(frameSB, RooFit::Name("double_exp"), RooFit::LineColor(kRed), RooFit::Range("full"), RooFit::NormRange("full"));
  w3->pdf("sb_model")->plotOn(frameSB, RooFit::Name("single_pow"), RooFit::LineColor(kGreen), RooFit::Range("full"), RooFit::NormRange("full"));
  w4->pdf("sb_model")->plotOn(frameSB, RooFit::Name("poly2"), RooFit::LineColor(kYellow), RooFit::Range("full"), RooFit::NormRange("full"));
  w5->pdf("sb_model")->plotOn(frameSB, RooFit::Name("poly3"), RooFit::LineColor(kMagenta), RooFit::Range("full"), RooFit::NormRange("full"));
  w6->pdf("sb_model")->plotOn(frameSB, RooFit::Name("poly4"), RooFit::LineColor(kCyan), RooFit::Range("full"), RooFit::NormRange("full"));
  RooRealVar *alpha1_1=w1->var("DCB_NE_sig4_CB_alpha1");
  RooRealVar *alpha1_2=w2->var("DCB_NE_sig4_CB_alpha1");
  RooRealVar *alpha1_3=w3->var("DCB_NE_sig4_CB_alpha1");
  RooRealVar *alpha1_4=w4->var("DCB_NE_sig4_CB_alpha1");
  RooRealVar *alpha1_5=w5->var("DCB_NE_sig4_CB_alpha1");
  RooRealVar *alpha1_6=w6->var("DCB_NE_sig4_CB_alpha1");

  RooRealVar *alpha2_1=w1->var("DCB_NE_sig4_CB_alpha2");
  RooRealVar *alpha2_2=w2->var("DCB_NE_sig4_CB_alpha2");
  RooRealVar *alpha2_3=w3->var("DCB_NE_sig4_CB_alpha2");
  RooRealVar *alpha2_4=w4->var("DCB_NE_sig4_CB_alpha2");
  RooRealVar *alpha2_5=w5->var("DCB_NE_sig4_CB_alpha2");
  RooRealVar *alpha2_6=w6->var("DCB_NE_sig4_CB_alpha2");

  RooRealVar *mu_1=w1->var("DCB_NE_sig4_CB_mu");
  RooRealVar *mu_2=w2->var("DCB_NE_sig4_CB_mu");
  RooRealVar *mu_3=w3->var("DCB_NE_sig4_CB_mu");
  RooRealVar *mu_4=w4->var("DCB_NE_sig4_CB_mu");
  RooRealVar *mu_5=w5->var("DCB_NE_sig4_CB_mu");
  RooRealVar *mu_6=w6->var("DCB_NE_sig4_CB_mu");

  RooRealVar *sigma_1=w1->var("DCB_NE_sig4_CB_sigma");
  RooRealVar *sigma_2=w2->var("DCB_NE_sig4_CB_sigma");
  RooRealVar *sigma_3=w3->var("DCB_NE_sig4_CB_sigma");
  RooRealVar *sigma_4=w4->var("DCB_NE_sig4_CB_sigma");
  RooRealVar *sigma_5=w5->var("DCB_NE_sig4_CB_sigma");
  RooRealVar *sigma_6=w6->var("DCB_NE_sig4_CB_sigma");

  RooRealVar *n1_1=w1->var("DCB_NE_sig4_CB_n1");
  RooRealVar *n1_2=w2->var("DCB_NE_sig4_CB_n1");
  RooRealVar *n1_3=w3->var("DCB_NE_sig4_CB_n1");
  RooRealVar *n1_4=w4->var("DCB_NE_sig4_CB_n1");
  RooRealVar *n1_5=w5->var("DCB_NE_sig4_CB_n1");
  RooRealVar *n1_6=w6->var("DCB_NE_sig4_CB_n1");

  RooRealVar *n2_1=w1->var("DCB_NE_sig4_CB_n2");
  RooRealVar *n2_2=w2->var("DCB_NE_sig4_CB_n2");
  RooRealVar *n2_3=w3->var("DCB_NE_sig4_CB_n2");
  RooRealVar *n2_4=w4->var("DCB_NE_sig4_CB_n2");
  RooRealVar *n2_5=w5->var("DCB_NE_sig4_CB_n2");
  RooRealVar *n2_6=w6->var("DCB_NE_sig4_CB_n2");

  RooDoubleCB *dcb1 = new RooDoubleCB("dcb1", "dcb1", m_mumu,*mu_1, *sigma_1, *alpha1_1, *n1_1, *alpha2_1, *n2_1);
  RooDoubleCB *dcb2 = new RooDoubleCB("dcb2", "dcb2", m_mumu,*mu_2, *sigma_2, *alpha1_2, *n1_2, *alpha2_2, *n2_2);
  RooDoubleCB *dcb3 = new RooDoubleCB("dcb3", "dcb3", m_mumu,*mu_3, *sigma_3, *alpha1_3, *n1_3, *alpha2_3, *n2_3);
  RooDoubleCB *dcb4 = new RooDoubleCB("dcb4", "dcb4", m_mumu,*mu_4, *sigma_4, *alpha1_4, *n1_4, *alpha2_4, *n2_4);
  RooDoubleCB *dcb5 = new RooDoubleCB("dcb5", "dcb5", m_mumu,*mu_5, *sigma_5, *alpha1_5, *n1_5, *alpha2_5, *n2_5);
  RooDoubleCB *dcb6 = new RooDoubleCB("dcb6", "dcb6", m_mumu,*mu_6, *sigma_6, *alpha1_6, *n1_6, *alpha2_6, *n2_6);
  
    
  dcb1->plotOn(frameS, RooFit::Name("single_exp"), RooFit::LineColor(kRed), RooFit::Range("sig_plot") );
  dcb2->plotOn(frameS, RooFit::Name("double_exp"), RooFit::LineColor(kBlue), RooFit::Range("sig_plot") );
  dcb3->plotOn(frameS, RooFit::Name("single_pow"), RooFit::LineColor(kGreen), RooFit::Range("sig_plot") );
  dcb4->plotOn(frameS, RooFit::Name("poly2"), RooFit::LineColor(kYellow), RooFit::Range("sig_plot") );
  dcb5->plotOn(frameS, RooFit::Name("poly3"), RooFit::LineColor(kMagenta), RooFit::Range("sig_plot") );
  dcb6->plotOn(frameS, RooFit::Name("poly4"), RooFit::LineColor(kCyan), RooFit::Range("sig_plot") );
  
  csb->cd();
  frameSB->Draw();
  frameSB->SetTitle("");
  frameSB->GetYaxis()->SetTitle("Entries");
  TLegend *legSB = new TLegend(0.6,0.7,0.9,0.9);
  
  legSB->AddEntry(frameSB->findObject("single_exp"), "dcb + single_exp" );
  legSB->AddEntry(frameSB->findObject("double_exp"), "dcb + double_exp" );
  legSB->AddEntry(frameSB->findObject("single_pow"), "dcb + single_pow" );
  legSB->AddEntry(frameSB->findObject("poly2"), "dcb + poly2" );
  legSB->AddEntry(frameSB->findObject("poly3"), "dcb + poly3" );
  legSB->AddEntry(frameSB->findObject("poly4"), "dcb + poly4" );
  legSB->Draw();
  
  //csb->SetLogy();
  csb->Update();
  csb->SaveAs("allFits_Psi_p_.png");


  TCanvas *cs = new TCanvas("cs", "cs", 900, 600);

  cs->cd();
  frameS->Draw();
  frameS->SetTitle("");
  frameS->GetYaxis()->SetTitle("Entries");
  legSB->Draw();
  
  //cs->SetLogy();
  cs->Update();
  cs->SaveAs("allSigFits_Psi_p_.png");
  
}
