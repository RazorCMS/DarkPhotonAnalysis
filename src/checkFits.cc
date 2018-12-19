// C++ includes
#include <string>
#include <iostream>

// ROOT includes
#include <TFile.h>
#include <TString.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TTree.h>

// ROOFIT includes
#include <RooDataSet.h>
#include <RooExponential.h>
#include <RooCBShape.h>
#include <RooAddPdf.h>
#include <RooPlot.h>
#include <RooArgList.h>
#include <RooRealVar.h>

// LOCAL includes
#include "pdfs.h"

int main(void) {
    TFile *file = new TFile("/eos/user/u/ufay/2017Data_Jakob/scout_skimmed_OS/2Dec2018xcg_job0_scout_skimmed.root", "read");
    TTree *tree = (TTree*) file->Get("tree");

    // Define variable
    RooRealVar mzd("mass", "m_{#mu#mu}", 0., 10., "GeV");
    mzd.setMin(0.);
    mzd.setMax(10.);
    mzd.setRange("JPsi", 2., 3.5);
    mzd.setRange("low", 2.0, 2.9);
    mzd.setRange("high", 3.225, 3.5);
    mzd.setRange("sig", 2.9, 3.225);

    // Make RooDataSet
    tree->GetBranch("mass");
    RooDataSet *treemass = new RooDataSet("mass", "", RooArgSet(mzd), RooFit::Import(*tree));

    // Make dummy RooWorkspace
    RooWorkspace *w = new RooWorkspace("checkFits", "");

    // Fit JPsi
    double m_JPsi = 3.1;

    // Define PDFs
    TString dcbfit_NE = MakeDoubleCB(false, "DCB_sig_check", m_JPsi, mzd, *w);
    TString dexpofit_NE = MakeDoubleExpo(false, "DExpo_bkg_check", mzd, *w);

    // Set parameters
    RooRealVar nsig("nsig", "", 9.24227e+05);
    RooRealVar nbkg("nbkg", "", 4.36363e+06);
    nsig.setConstant(kFALSE);
    nbkg.setConstant(kFALSE);
    
    // DCB
    w->var(dcbfit_NE+"_CB_mu1")->setVal(3.08436e+00);
    w->var(dcbfit_NE+"_CB_alpha1")->setVal(1.85519e+00);
    w->var(dcbfit_NE+"_CB_sigma1")->setVal(5.63970e-02);
    w->var(dcbfit_NE+"_CB_n1")->setVal(1.01862e+00);
    w->var(dcbfit_NE+"_CB_mu2")->setVal(3.09119e+00);
    w->var(dcbfit_NE+"_CB_alpha2")->setVal(-3.77213e+00);
    w->var(dcbfit_NE+"_CB_sigma2")->setVal(2.75309e-02);
    w->var(dcbfit_NE+"_CB_n2")->setVal(1.97169e-07);
    w->var(dcbfit_NE+"_frac")->setVal(4.98426e-01);

    // DExpo
    w->var(dexpofit_NE+"_lambdaExpo1")->setVal(-2.73926e-01);
    w->var(dexpofit_NE+"_lambdaExpo2")->setVal(-7.30272e-01);
    w->var(dexpofit_NE+"_frac")->setVal(6.13955e-08);

    // S + B
    RooAddPdf *sb7 = new RooAddPdf("sb7", "sb7", RooArgList(*w->pdf(dcbfit_NE), *w->pdf(dexpofit_NE)), RooArgList(nsig, nbkg));

    // Plotting
    // Plot B
    TCanvas *cb = new TCanvas("cb", "cb", 800, 700);
    RooPlot *frameB = mzd.frame(2.0, 3.5, 100);
    treemass->plotOn(frameB);
    w->pdf(dexpofit_NE)->plotOn(frameB, RooFit::Name("dexpofit_NE"), RooFit::LineColor(46), RooFit::Range("low,high"), RooFit::NormRange("low,high"));
    frameB->Draw();
    frameB->SetTitle("");

    TLegend *legB = new TLegend(0.1,0.7,0.4,0.9);
    legB->AddEntry(frameB->findObject("dexpofit_NE"), "dexpo_NE");
    legB->Draw();

    cb->SetLogy();
    cb->Update();
    cb->SaveAs("check_b_JPsi.png");

    // Plot S
    TCanvas *cs = new TCanvas("cs", "cs", 800, 700);
    RooPlot *frameS = mzd.frame(2.0, 3.5, 100);
    treemass->plotOn(frameS);
    w->pdf(dcbfit_NE)->plotOn(frameS, RooFit::Name("dcbfit_NE"), RooFit::LineColor(kGreen), RooFit::Range("sig"), RooFit::NormRange("sig"));
    frameS->Draw();
    frameS->SetTitle("");

    TLegend *legS = new TLegend(0.1,0.7,0.4,0.9);
    legS->AddEntry(frameS->findObject("dcbfit_NE"), "dcb_NE");
    legS->Draw();

    cs->SetLogy();
    cs->Update(); 
    cs->SaveAs("check_s_JPsi.png");

    // Plot S + B
    TCanvas *csb = new TCanvas("csb", "csb", 800, 700);
    RooPlot *frameSB = mzd.frame(2.0, 3.5, 100);
    treemass->plotOn(frameSB);
    sb7->plotOn(frameSB, RooFit::Name("sb7"), RooFit::LineColor(40), RooFit::Range("JPsi"), RooFit::NormRange("JPsi"));

    csb->cd(); 
    frameSB->Draw();
    frameSB->SetTitle("");

    TLegend *legSB = new TLegend(0.1,0.7,0.4,0.9);
    legSB->AddEntry(frameSB->findObject("sb7"), "dcb + dexpo");
    legSB->Draw();

    csb->SetLogy();
    csb->Update();
    csb->SaveAs("check_sb_JPsi.png");

    return 0;
}



