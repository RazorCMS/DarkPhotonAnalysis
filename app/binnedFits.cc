// C++ includes
#include <iostream>
#include <fstream>

// ROOT includes
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TStopwatch.h>

// ROOFIT includes 
#include <RooDataHist.h>
#include <RooAddPdf.h>
#include <RooPlot.h>
#include <RooFitResult.h>

// LOCAL includes
#include "pdfs.h"

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cout << "usage: " << argv[0] << " inputFile nsig nbkg" << std::endl;
        exit(1);
    }

    TFile *file = new TFile(argv[1]);
    TTree *tree = (TTree*) file->Get("tree");
    TH1D *massH = new TH1D("massH", "massH", 1000, 0., 0.);
    tree->Draw("mass>>massH");

    RooWorkspace *w = new RooWorkspace("lowMassFits", "");

    double JPsi = 3.1;

    // Make var
    RooRealVar mzd("mass", "m_{#mu#mu}", 0., 10., "GeV");
    mzd.setMin(0.);                                                                                                                                                                                                                      
    mzd.setMax(10.);                                                                                                                                                                                                                     
    
    mzd.setRange("JPsi", 2.0, 3.5);
    mzd.setRange("sig", 2.9, 3.225);
    mzd.setRange("low", 2.0, 2.9);
    mzd.setRange("high", 3.225, 3.5);

    // Get histogram of data
    RooDataHist *massRooH = new RooDataHist("massRooH", "massRooH", RooArgSet(mzd), massH);
    
    // Define shapes for fitting 
    TString dcbfit_NE = MakeDoubleCB(false, "DCB_NE_sig", JPsi, mzd, *w);
    TString dexpofit_NE = MakeDoubleExpo(false, "DExpo_NE_bkg", mzd, *w);

    // Define shapes - not fitted
    TString dcbfit_NE_nofit = MakeDoubleCB(false, "DCB_NE_sig_nofit", JPsi, mzd, *w);
    TString dexpofit_NE_nofit = MakeDoubleExpo(false, "DExpo_NE_bkg_nofit", mzd, *w);

    RooRealVar nsig("nsig", "", atof(argv[2]));
    RooRealVar nbkg("nbkg", "", atof(argv[3]));
    nsig.setConstant(kFALSE);
    nbkg.setConstant(kFALSE);
    
    // Not fitted
    RooRealVar nsig_nofit("nsig_nofit", "", atof(argv[2]));
    RooRealVar nbkg_nofit("nbkg_nofit", "", atof(argv[3]));

    // Set parameters
    // For fitting
    // DCB
    w->var(dcbfit_NE+"_CB_mu1")->setVal(3.06050e+00);                                                                                                                                                                                    
    w->var(dcbfit_NE+"_CB_alpha1")->setVal(1.33886e+00);                                                                                                                                                                                
    w->var(dcbfit_NE+"_CB_sigma1")->setVal(9.57192e-02);                                                                                                                                                                                 
    w->var(dcbfit_NE+"_CB_n1")->setVal(9.47739e+00);                                                                                                                                                                                     
    w->var(dcbfit_NE+"_CB_mu2")->setVal(3.09025e+00);                                                                                                                                                                                    
    w->var(dcbfit_NE+"_CB_alpha2")->setVal(-3.66052e+00);                                                                                                                                                                                
    w->var(dcbfit_NE+"_CB_sigma2")->setVal(3.53358e-02);                                                                                                                                                                                 
    w->var(dcbfit_NE+"_CB_n2")->setVal(2.10568e+01);                                                                                                                                                                                     
    w->var(dcbfit_NE+"_frac")->setVal(1.50803e-01); 

    // DExpo
    w->var(dexpofit_NE+"_lambdaExpo1")->setVal(3.06624e-01);                                                                                                                                                                             
    w->var(dexpofit_NE+"_lambdaExpo2")->setVal(-8.05551e-01);                                                                                                                                                                            
    w->var(dexpofit_NE+"_frac")->setVal(5.97124e-02); 

    // Not fitted
    // DCB
    w->var(dcbfit_NE_nofit+"_CB_mu1")->setVal(3.06050e+00);
    w->var(dcbfit_NE_nofit+"_CB_alpha1")->setVal(1.33886e+00);
    w->var(dcbfit_NE_nofit+"_CB_sigma1")->setVal(9.57192e-02);
    w->var(dcbfit_NE_nofit+"_CB_n1")->setVal(9.47739e+00);
    w->var(dcbfit_NE_nofit+"_CB_mu2")->setVal(3.09025e+00);
    w->var(dcbfit_NE_nofit+"_CB_alpha2")->setVal(-3.66052e+00);
    w->var(dcbfit_NE_nofit+"_CB_sigma2")->setVal(3.53358e-02);
    w->var(dcbfit_NE_nofit+"_CB_n2")->setVal(2.10568e+01);
    w->var(dcbfit_NE_nofit+"_frac")->setVal(1.50803e-01); 

    // DExpo
    w->var(dexpofit_NE_nofit+"_lambdaExpo1")->setVal(3.06624e-01);
    w->var(dexpofit_NE_nofit+"_lambdaExpo2")->setVal(-8.05551e-01);
    w->var(dexpofit_NE_nofit+"_frac")->setVal(5.97124e-02);

    // Combined shape for fitting
    RooAddPdf *sb = new RooAddPdf("sb", "sb", RooArgList(*w->pdf(dcbfit_NE), *w->pdf(dexpofit_NE)), RooArgList(nsig, nbkg));

    // Combined shape - not fitted
    RooAddPdf *sb_nofit = new RooAddPdf("sb_nofit", "sb_nofit", RooArgList(*w->pdf(dcbfit_NE_nofit), *w->pdf(dexpofit_NE_nofit)), RooArgList(nsig_nofit, nbkg_nofit));

    TStopwatch t;
    t.Start();
    RooFitResult *sbres = sb->fitTo(*massRooH, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range("JPsi"), RooFit::Timer(1));
    t.Print();

    // Plot
    // S+B - fitted
    TCanvas *csb = new TCanvas("csb", "csb", 800, 700);
    RooPlot *frameSB = mzd.frame(2.0, 3.5);
    massRooH->plotOn(frameSB, RooFit::Name("datasb"));
    sb->plotOn(frameSB, RooFit::Name("sb"), RooFit::Range("JPsi"), RooFit::NormRange("JPsi"));
    csb->cd();
    frameSB->Draw();
    frameSB->SetTitle("");
    csb->SetLogy();
    csb->Update();
    csb->SaveAs("sb_binned.png");

    // S+B - not fitted
    TCanvas *csb_nofit = new TCanvas("csb_nofit", "csb_nofit", 800, 700);
    RooPlot *frameSB_nofit = mzd.frame(2.0, 3.5);
    massRooH->plotOn(frameSB_nofit, RooFit::Name("datasb_nofit"));
    sb_nofit->plotOn(frameSB_nofit, RooFit::Name("sb_nofit"), RooFit::Range("JPsi"), RooFit::NormRange("JPsi"));
    csb_nofit->cd();
    frameSB_nofit->Draw();
    frameSB_nofit->SetTitle("");
    csb_nofit->SetLogy();
    csb_nofit->Update();
    csb_nofit->SaveAs("sb_binned_nofit.png");

    // S
    TCanvas *cs = new TCanvas("cs", "cs", 800, 700);
    RooPlot *frameS = mzd.frame(2.0, 3.5);
    massRooH->plotOn(frameS, RooFit::Name("datas"));
    w->pdf(dcbfit_NE)->plotOn(frameS, RooFit::Name("s"), RooFit::Range("sig"), RooFit::NormRange("sig"));
    cs->cd();
    frameS->Draw();
    frameS->SetTitle("");
    cs->SetLogy();
    cs->Update();
    cs->SaveAs("s_binned.png");
    
    // B
    TCanvas *cb = new TCanvas("cb", "cb", 800, 700);
    RooPlot *frameB = mzd.frame(2.0, 3.5);
    massRooH->plotOn(frameB, RooFit::Name("datab"));
    w->pdf(dexpofit_NE)->plotOn(frameB, RooFit::Name("b"), RooFit::Range("low,high"), RooFit::NormRange("low,high"));
    cb->cd();
    frameB->Draw();
    frameB->SetTitle("");
    cb->SetLogy();
    cb->Update();
    cb->SaveAs("b_binned.png");

// Get fit output to text file
    std::ofstream fit_file;
    fit_file.open("JPsi_binnedFit.txt");

    if (fit_file.is_open()) {
        sbres->printMultiline(fit_file, 0);
        fit_file << "Number of NLL evaluations with problems: " << sbres->numInvalidNLL() << std::endl;
        fit_file << "Fitted chi2 / ndof: " << frameSB->chiSquare("sb", "datasb") << std::endl;
        fit_file << "Not Fitted chi2 / ndof: " << frameSB_nofit->chiSquare("sb_nofit", "datasb_nofit") << std::endl;
        fit_file.close();
    }

    else { 
        std::cerr << "Unable to open fit output file" << std::endl;
        exit(1);
    }

    return 0;
}
