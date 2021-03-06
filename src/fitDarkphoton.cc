//C++ INCLUDES
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

//ROOT INCLUDES
#include <TSystem.h>
#include <TStopwatch.h>
#include <TTree.h>
#include <TLatex.h>
#include <TString.h>
#include <TFile.h>
#include <TH1D.h>
#include <TF1.h>
#include <TCut.h>
#include <TBox.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TRandom3.h>
#include <TLegend.h>
#include <TMath.h>
#include <TROOT.h>
#include <Math/GaussIntegrator.h>
#include <Math/IntegratorOptions.h>

//ROOFIT INCLUDES
#include <RooWorkspace.h>
#include <RooDataSet.h>
#include <RooRealVar.h>
#include <RooExponential.h>
#include <RooAddPdf.h>
#include <RooBreitWigner.h>
#include <RooMinimizer.h>
#include <RooFitResult.h>
#include <RooPlot.h>
#include <RooExtendPdf.h>
#include <RooStats/SPlot.h>
#include <RooStats/ModelConfig.h>
#include <RooGenericPdf.h>
#include <RooFormulaVar.h>
#include <RooBernstein.h>
#include <RooChebychev.h>
#include <RooMinuit.h>
#include <RooNLLVar.h>
#include <RooRandom.h>
#include <RooDataHist.h>
#include <RooHistPdf.h>
#include <RooArgList.h>
//#include <RealVar.h>

// LOCAL INCLUDES
#include "fitDarkphoton.h"
#include "pdfs.h"

// Margins
const float leftMargin   = 0.12;
const float rightMargin  = 0.05;
const float topMargin    = 0.07;
const float bottomMargin = 0.12;

/*
 * Makes plot of the data.
 *
 * Args
 *      tree: TTree contaning data
 *      tag: tag for the name of saved plot
 *      vtxCut: vertex cut data is used with -- does not affect plot, just as a reminder
 *      sideband_min: lower mass of the sideband region
 *      sideband_max: upper mass of the sideband region
 */
void MakePlotTree(TTree *tree, TString tag, TString vtxCut, double sideband_min, double sideband_max) {
	RooRealVar mzd("mass", "m_{#mu#mu}", sideband_min, sideband_max, "GeV");
	
	TCanvas* c1 = new TCanvas( "c", "c", 2119, 33, 800, 700 );
	c1->SetHighLightColor(2);
	c1->SetFillColor(0);
	c1->SetBorderMode(0);
	c1->SetBorderSize(2);
	c1->SetLeftMargin(leftMargin);
	c1->SetRightMargin(rightMargin);
	c1->SetTopMargin(topMargin);
	c1->SetBottomMargin(bottomMargin);
	c1->SetFrameBorderMode(0);
	c1->SetFrameBorderMode(0);

	RooPlot *fmzd = mzd.frame();
	RooDataSet data("data", "", RooArgSet(mzd), RooFit::Import(*tree));
	data.plotOn(fmzd);
	fmzd->Draw();
	fmzd->SetTitle("");
	c1->Update();
        system("mkdir -p output/");
	c1->SaveAs("output/dataPlot_" + tag + "_cut_" + vtxCut + "cm.png");
}
	

/*
 * Makes plots of the fitted signal and background.
 *
 * Args
 *      mzd: RooRealVar for the mass
 *      w: RooWorkspace with fitted shapes
 */
void MakePlot(RooRealVar &mzd, RooWorkspace &w, TString binNumber, TString vtxCut, TString tag_sig, TString tag_bkg, double bkgYield, RooDataSet *data_sig, RooDataSet *data_bkg, RooDataHist *data_toy) {
	//-------------------------------------------
	// Plotting
	// ------------------------------------------
	TCanvas* c1 = new TCanvas( "c", "c", 2119, 33, 800, 700 );
	c1->SetHighLightColor(2);
	c1->SetFillColor(0);
	c1->SetBorderMode(0);
	c1->SetBorderSize(2);
	c1->SetLeftMargin(leftMargin);
	c1->SetRightMargin(rightMargin);
	c1->SetTopMargin(topMargin);
	c1->SetBottomMargin(bottomMargin);
	c1->SetFrameBorderMode(0);
	c1->SetFrameBorderMode(0);

	// Background
	RooPlot *fmzd = mzd.frame();
	RooDataSet* dataCut = (RooDataSet*) data_bkg->reduce(RooFit::Name("dataCut"),RooFit::SelectVars(RooArgSet(mzd)),RooFit::CutRange("low"));
	RooDataSet* dataHigh = (RooDataSet*) data_bkg->reduce(RooFit::Name("dataHigh"),RooFit::SelectVars(RooArgSet(mzd)),RooFit::CutRange("high"));
	dataCut->append(*dataHigh);
//	dataCut->plotOn(fmzd);
	data_bkg->plotOn(fmzd);
//
	w.pdf(tag_bkg)->plotOn(fmzd);
//	w.pdf(tag_bkg)->plotOn(fmzd, RooFit::Normalization(bkgYield, RooAbsReal::NumEvent), RooFit::Range("full"), RooFit::NormRange("full"));
//	w.pdf(tag_bkg)->plotOn(fmzd, RooFit::LineColor(kGreen), RooFit::Range("full"), RooFit::NormRange("full"));
//	w.pdf(tag_bkg)->plotOn(fmzd,RooFit::LineColor(kBlue), RooFit::Normalization(bkgYield, RooAbsReal::NumEvent), RooFit::Range("signal"), RooFit::NormRange("signal"));
	
	float maxC = fmzd->GetMaximum();
	fmzd->SetAxisRange(0.1, maxC, "Y");
	fmzd->Draw();
	fmzd->SetTitle("");
	c1->Update();
        system("mkdir -p output/");
	c1->SaveAs("output/bkgFit_bin" + binNumber + "_cut_" + vtxCut + "cm.png");

	fmzd->SetName("BkgFitPlot");

	w.import(*fmzd);

	std::cout << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << "[BKG GOODNESS OF FIT]:" << std::endl;
	std::cout << fmzd->chiSquare() << std::endl;;
	std::cout << "================================================================================" << std::endl;
	std::cout << std::endl;

	// Signal
	RooPlot *fmzd2 = mzd.frame();
	double npoints_signal = data_sig->numEntries();
	data_sig->plotOn(fmzd2);
	w.pdf(tag_sig)->plotOn(fmzd2, RooFit::LineColor(kRed), RooFit::Range("full"), RooFit::NormRange("full"));
	fmzd2->Draw();
	fmzd2->SetTitle("");
	c1->Update();
	c1->SaveAs("output/signalFit_bin" + binNumber + "_cut_" + vtxCut + "cm.png");
	fmzd2->SetName("SignalFitPlot");

	w.import(*fmzd2);

	std::cout << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << "[SIGNAL GOODNESS OF FIT]:" << std::endl;
	std::cout << fmzd2->chiSquare() << std::endl;;
	std::cout << "================================================================================" << std::endl;
	std::cout << std::endl;

        
	// Toy Data
	RooPlot *fmzd3 = mzd.frame();
	data_toy->plotOn(fmzd3);
	w.pdf(tag_bkg)->plotOn(fmzd3, RooFit::LineColor(kBlue), RooFit::Range("full"), RooFit::NormRange("full"));
	fmzd3->Draw();
	fmzd3->SetTitle("");
	c1->Update();
	c1->SaveAs("output/dataToy_bin" + binNumber + "_cut_" + vtxCut + "cm.png");
	fmzd3->SetName( "dataToyPlot" );

	w.import(*fmzd3);
}


/*
 * The functions below fit the MC signal or background with the specified shapes. 
 * They return custom structs that contain the tag given to the shapes and the 
 * shape parameters. Mainly used in the function 'MakeDataCard'.
 */
bwFit_output BreitWignerFit(double mass, double lifetime, RooDataSet *data, RooRealVar &mzd, RooWorkspace &w) {
	int npoints = data->numEntries();

	TString tag_signal = MakeBreitWigner(true, "BW_SG", mass, lifetime, mzd, w);
	w.var(tag_signal + "_Ns")->setVal((double) npoints);

	RooFitResult *sres = w.pdf(tag_signal)->fitTo(*data, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range("full"));
	sres->SetName("SigFitResult");
	
	double decay_width = 1. / lifetime;
	double BW_Ns = w.var(tag_signal + "_Ns")->getVal();
	double BW_mean_s = w.var(tag_signal + "_meanBW")->getVal();
	double BW_width_s = w.var(tag_signal + "_widthBW")->getVal();
	
	std::cout << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << "[INFO]:" << std::endl;
	std::cout << "Tag Signal: " << tag_signal << std::endl;
	std::cout << "Mass: " << mass << std::endl;
	std::cout << "Decay Width: " << decay_width << std::endl;
	std::cout << "Fitted Ns: " << BW_Ns << std::endl;
	std::cout << "Fitted Mean: " << BW_mean_s << std::endl;
	std::cout << "Fitted Width: " << BW_width_s << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << std::endl;

	w.import(*sres);
	w.import(*data);
	
	return {tag_signal, BW_mean_s, BW_width_s, BW_Ns, npoints};
}
	
expoFit_output SidebandExpoFit(RooDataSet *data, RooRealVar &mzd, RooWorkspace &w) {
        int npoints = data->numEntries();
	
	TString tag_bkg = MakeExpo(true, "EXPO_BKG", mzd, w);
	w.var(tag_bkg + "_Nbkg")->setVal((double) npoints);

	RooFitResult *bres = w.pdf(tag_bkg)->fitTo(*data, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range("low,high"));
	bres->SetName("BkgFitResult");

	double sE_Nbkg = w.var(tag_bkg + "_Nbkg")->getVal();
	double sE_lambda_bkg = w.var(tag_bkg + "_lambdaExpo")->getVal();
	
	float NbkgUn = w.var(tag_bkg + "_Nbkg")->getError();
	float BkgNormUn = 1.0 + NbkgUn/sE_Nbkg; //input a lnN to combine
	
	std::cout << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << "[INFO]:" << std::endl;
	std::cout << "Tag Bkg: " << tag_bkg << std::endl;
	std::cout << "Fitted Nbkg: " << sE_Nbkg << std::endl;
	std::cout << "Fitted Nbkg Uncertainty: " << NbkgUn << std::endl;
	std::cout << "Fitted Nbkg Normalized Uncertainty: " << BkgNormUn << std::endl;
	std::cout << "Fitted Lambda: " << sE_lambda_bkg << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << std::endl;

	w.import(*bres);
	w.import(*data);

	return {tag_bkg, sE_lambda_bkg, sE_Nbkg, npoints};
}

doubleExpoFit_output SidebandDExpoFit(RooDataSet *data, RooRealVar &mzd, RooWorkspace &w) {
        int npoints = data->numEntries();
	
	TString tag_bkg = MakeDoubleExpo(true, "DEXPO_BKG", mzd, w);
	w.var(tag_bkg + "_Nbkg")->setVal((double) npoints);

	RooFitResult *bres = w.pdf(tag_bkg)->fitTo(*data, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range("low,high"));
	bres->SetName("BkgFitResult");

	double dE_Nbkg = w.var(tag_bkg + "_Nbkg")->getVal();
	double dE_lambda1_bkg = w.var(tag_bkg + "_lambdaExpo1")->getVal();
	double dE_lambda2_bkg = w.var(tag_bkg + "_lambdaExpo2")->getVal();
	
	float NbkgUn = w.var(tag_bkg + "_Nbkg")->getError();
	float BkgNormUn = 1.0 + NbkgUn/dE_Nbkg; //input a lnN to combine
	
	std::cout << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << "[INFO]:" << std::endl;
	bres->Print();
	std::cout << std::endl;

	std::cout << "[INFO]:" << std::endl;
	std::cout << "Tag Bkg: " << tag_bkg << std::endl;
	std::cout << "Fitted Nbkg: " << dE_Nbkg << std::endl;
	std::cout << "Fitted Nbkg Uncertainty: " << NbkgUn << std::endl;
	std::cout << "Fitted Nbkg Normalized Uncertainty: " << BkgNormUn << std::endl;
	std::cout << "Fitted Lambda1: " << dE_lambda1_bkg << std::endl;
	std::cout << "Fitted Lambda1sq: " << dE_lambda1_bkg * dE_lambda1_bkg << std::endl;
	std::cout << "Fitted Lambda2: " << dE_lambda2_bkg << std::endl;
	std::cout << "Fitted Lambda2sq: " << dE_lambda2_bkg * dE_lambda2_bkg << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << std::endl;

	w.import(*bres);
	w.import(*data);

	return {tag_bkg, dE_lambda1_bkg, dE_lambda2_bkg, dE_Nbkg, npoints};
}

doubleExpoN1N2Fit_output SidebandDExpoN1N2Fit(RooDataSet *data, RooRealVar &mzd, RooWorkspace &w) {
        int npoints = data->numEntries();
	
	TString tag_bkg = MakeDoubleExpoN1N2("DEXPON1N2_BKG", mzd, w);
	w.var(tag_bkg + "_Nbkg1")->setVal(((double) npoints)/2.);
	w.var(tag_bkg + "_Nbkg2")->setVal(((double) npoints)/2.);

	RooFitResult *bres = w.pdf(tag_bkg)->fitTo(*data, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range("low,high"));
	bres->SetName("BkgFitResult");

	double dE_Nbkg1 = w.var(tag_bkg + "_Nbkg1")->getVal();
	double dE_Nbkg2 = w.var(tag_bkg + "_Nbkg2")->getVal();
	double dE_lambda1_bkg = w.var(tag_bkg + "_lambdaExpo1")->getVal();
	double dE_lambda2_bkg = w.var(tag_bkg + "_lambdaExpo2")->getVal();
	
	float Nbkg1Un = w.var(tag_bkg + "_Nbkg1")->getError();
	float Nbkg2Un = w.var(tag_bkg + "_Nbkg2")->getError();
	float BkgNormUn1 = 1.0 + Nbkg1Un/dE_Nbkg1; //input a lnN to combine
	float BkgNormUn2 = 1.0 + Nbkg2Un/dE_Nbkg2; //input a lnN to combine
	
	std::cout << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << "[INFO]:" << std::endl;
	std::cout << "Tag Bkg: " << tag_bkg << std::endl;
	std::cout << "Fitted Nbkg1: " << dE_Nbkg1 << std::endl;
	std::cout << "Fitted Nbkg1 Uncertainty: " << Nbkg1Un << std::endl;
	std::cout << "Fitted Nbkg1 Normalized Uncertainty: " << BkgNormUn1 << std::endl;
	std::cout << "Fitted Nbkg2: " << dE_Nbkg2 << std::endl;
	std::cout << "Fitted Nbkg2 Uncertainty: " << Nbkg2Un << std::endl;
	std::cout << "Fitted Nbkg2 Normalized Uncertainty: " << BkgNormUn2 << std::endl;
	std::cout << "Fitted Lambda1: " << dE_lambda1_bkg << std::endl;
	std::cout << "Fitted Lambda2: " << dE_lambda2_bkg << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << std::endl;

	w.import(*bres);
	w.import(*data);

	return {tag_bkg, dE_lambda1_bkg, dE_lambda2_bkg, dE_Nbkg1, dE_Nbkg2, npoints};
}

powFit_output SidebandPowFit(RooDataSet *data, RooRealVar &mzd, RooWorkspace &w) {
        int npoints = data->numEntries();
	
	TString tag_bkg = MakeSinglePow("POW_BKG", mzd, w);
	w.var(tag_bkg + "_Nbkg")->setVal((double) npoints);

	RooFitResult *bres = w.pdf(tag_bkg)->fitTo(*data, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range("low,high"));
	bres->SetName("BkgFitResult");

	double sP_Nbkg = w.var(tag_bkg + "_Nbkg")->getVal();
	double sP_alpha_bkg = w.var(tag_bkg + "_alpha")->getVal();
	
	float NbkgUn = w.var(tag_bkg + "_Nbkg")->getError();
	float BkgNormUn = 1.0 + NbkgUn/sP_Nbkg; //input a lnN to combine
	
	std::cout << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << "[INFO]:" << std::endl;
	std::cout << "Tag Bkg: " << tag_bkg << std::endl;
	std::cout << "Fitted Nbkg: " << sP_Nbkg << std::endl;
	std::cout << "Fitted Nbkg Uncertainty: " << NbkgUn << std::endl;
	std::cout << "Fitted Nbkg Normalized Uncertainty: " << BkgNormUn << std::endl;
	std::cout << "Fitted Alpha: " << sP_alpha_bkg << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << std::endl;

	w.import(*bres);
	w.import(*data);

	return {tag_bkg, sP_alpha_bkg, sP_Nbkg, npoints};
}

doublePowFit_output SidebandDPowFit(RooDataSet *data, RooRealVar &mzd, RooWorkspace &w) {
        int npoints = data->numEntries();
	
	TString tag_bkg = MakeDoublePow("DPOW_BKG", mzd, w);
	w.var(tag_bkg + "_Nbkg")->setVal((double) npoints);

	RooFitResult *bres = w.pdf(tag_bkg)->fitTo(*data, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range("low,high"));
	bres->SetName("BkgFitResult");

	double dP_Nbkg = w.var(tag_bkg + "_Nbkg")->getVal();
	double dP_alpha1_bkg = w.var(tag_bkg + "_alpha1")->getVal();
	double dP_alpha2_bkg = w.var(tag_bkg + "_alpha2")->getVal();
	
	float NbkgUn = w.var(tag_bkg + "_Nbkg")->getError();
	float BkgNormUn = 1.0 + NbkgUn/dP_Nbkg; //input a lnN to combine
	
	std::cout << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << "[INFO]:" << std::endl;
	std::cout << "Tag Bkg: " << tag_bkg << std::endl;
	std::cout << "Fitted Nbkg: " << dP_Nbkg << std::endl;
	std::cout << "Fitted Nbkg Uncertainty: " << NbkgUn << std::endl;
	std::cout << "Fitted Nbkg Normalized Uncertainty: " << BkgNormUn << std::endl;
	std::cout << "Fitted Alpha1: " << dP_alpha1_bkg << std::endl;
	std::cout << "Fitted Alpha2: " << dP_alpha2_bkg << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << std::endl;

	w.import(*bres);
	w.import(*data);

	return {tag_bkg, dP_alpha1_bkg, dP_alpha2_bkg, dP_Nbkg, npoints};
}

bernPoly2Fit_output SidebandBernPoly2Fit(RooDataSet *data, RooRealVar &mzd, RooWorkspace &w) {
        int npoints = data->numEntries();
	
	TString tag_bkg = MakeBernPoly2(true, "BERNPOLY2_BKG", mzd, w);
	w.var(tag_bkg + "_Nbkg")->setVal((double) npoints);

	RooFitResult *bres = w.pdf(tag_bkg)->fitTo(*data, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range("low,high"));
	bres->SetName("BkgFitResult");

	double poly2_Nbkg = w.var(tag_bkg + "_Nbkg")->getVal();
	double poly2_pC_bkg = w.var(tag_bkg + "_pC")->getVal();
	double poly2_p0_bkg = w.var(tag_bkg + "_p0")->getVal();
	double poly2_p1_bkg = w.var(tag_bkg + "_p1")->getVal();
	
	float NbkgUn = w.var(tag_bkg + "_Nbkg")->getError();
	float BkgNormUn = 1.0 + NbkgUn/poly2_Nbkg; //input a lnN to combine
		
	std::cout << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << "[INFO]:" << std::endl;
	bres->Print();
	std::cout << std::endl;

	std::cout << "[INFO]:" << std::endl;
	bres->correlationMatrix().Print();
	std::cout << std::endl;

	std::cout << "[INFO]:" << std::endl;
	std::cout << "Tag Bkg: " << tag_bkg << std::endl;
	std::cout << "Fitted Nbkg: " << poly2_Nbkg << std::endl;
	std::cout << "Fitted Nbkg Uncertainty: " << NbkgUn << std::endl;
	std::cout << "Fitted Nbkg Normalized Uncertainty: " << BkgNormUn << std::endl;
	std::cout << "Fitted p0: " << poly2_p0_bkg << std::endl;
	std::cout << "Fitted p1: " << poly2_p1_bkg << std::endl;
	std::cout << "Fitted pC: " << poly2_pC_bkg << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << std::endl;

	w.import(*bres);
	w.import(*data);

	return {tag_bkg, poly2_pC_bkg, poly2_p0_bkg, poly2_p1_bkg, poly2_Nbkg, npoints};
}

chebPoly2Fit_output SidebandChebPoly2Fit(RooDataSet *data, RooRealVar &mzd, RooWorkspace &w) {
        int npoints = data->numEntries();
	
	TString tag_bkg = MakeChebychevPoly2("CHEBPOLY2_BKG", mzd, w);
	w.var(tag_bkg + "_Nbkg")->setVal((double) npoints);

	RooFitResult *bres = w.pdf(tag_bkg)->fitTo(*data, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range("low,high"));
	bres->SetName("BkgFitResult");

	double poly2_Nbkg = w.var(tag_bkg + "_Nbkg")->getVal();
	double poly2_pC_bkg = w.var(tag_bkg + "_pC")->getVal();
	double poly2_p0_bkg = w.var(tag_bkg + "_p0")->getVal();
	
	float NbkgUn = w.var(tag_bkg + "_Nbkg")->getError();
	float BkgNormUn = 1.0 + NbkgUn/poly2_Nbkg; //input a lnN to combine
	
	std::cout << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << "[INFO]:" << std::endl;
	std::cout << "Tag Bkg: " << tag_bkg << std::endl;
	std::cout << "Fitted Nbkg: " << poly2_Nbkg << std::endl;
	std::cout << "Fitted Nbkg Uncertainty: " << NbkgUn << std::endl;
	std::cout << "Fitted Nbkg Normalized Uncertainty: " << BkgNormUn << std::endl;
	std::cout << "Fitted p0: " << poly2_p0_bkg << std::endl;
	std::cout << "Fitted pC: " << poly2_pC_bkg << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << std::endl;

	w.import(*bres);
	w.import(*data);

	return {tag_bkg, poly2_pC_bkg, poly2_p0_bkg, poly2_Nbkg, npoints};
}

bernPoly3Fit_output SidebandBernPoly3Fit(RooDataSet *data, RooRealVar &mzd, RooWorkspace &w) {
        int npoints = data->numEntries();
	
	TString tag_bkg = MakeBernPoly3(true, "BERNPOLY3_BKG", mzd, w);
	w.var(tag_bkg + "_Nbkg")->setVal((double) npoints);

	RooFitResult *bres = w.pdf(tag_bkg)->fitTo(*data, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range("low,high"));
	bres->SetName("BkgFitResult");

	double poly3_Nbkg = w.var(tag_bkg + "_Nbkg")->getVal();
	double poly3_pC_bkg = w.var(tag_bkg + "_pC")->getVal();
	double poly3_p0_bkg = w.var(tag_bkg + "_p0")->getVal();
	double poly3_p1_bkg = w.var(tag_bkg + "_p1")->getVal();
	double poly3_p2_bkg = w.var(tag_bkg + "_p2")->getVal();
	
	float NbkgUn = w.var(tag_bkg + "_Nbkg")->getError();
	float BkgNormUn = 1.0 + NbkgUn/poly3_Nbkg; //input a lnN to combine
	
	std::cout << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << "[INFO]:" << std::endl;
	bres->Print();
	std::cout << std::endl;

	std::cout << "[INFO]:" << std::endl;
	std::cout << "Tag Bkg: " << tag_bkg << std::endl;
	std::cout << "Fitted Nbkg: " << poly3_Nbkg << std::endl;
	std::cout << "Fitted Nbkg Uncertainty: " << NbkgUn << std::endl;
	std::cout << "Fitted Nbkg Normalized Uncertainty: " << BkgNormUn << std::endl;
	std::cout << "Fitted p0: " << poly3_p0_bkg << std::endl;
	std::cout << "Fitted p1: " << poly3_p1_bkg << std::endl;
	std::cout << "Fitted p2: " << poly3_p2_bkg << std::endl;
	std::cout << "Fitted pC: " << poly3_pC_bkg << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << std::endl;

	w.import(*bres);
	w.import(*data);

	return {tag_bkg, poly3_pC_bkg, poly3_p0_bkg, poly3_p1_bkg, poly3_p2_bkg, poly3_Nbkg, npoints};
}


#if 0
/*
 * This function would run over the different resonances and fit them. 
 * Not final and not tested. 
 */
int SplusB_fit(TTree *tree, bool testS=false, bool testB=false) {
//    system("mkdir -p SplusB_fits");   
    
    // Category A
    // ~0.547 GeV
    double mass0 = 0.55;
    // ~0.78 GeV
    double mass1 = 0.78;
    // ~1.02 GeV
    double mass2 = 1.02;
    // ~3.1 GeV - JPsi
    double mass3 = 3.1;
    // ~3.68 GeV
    double mass4 = 3.68;

    // Define Roo variables for different masses
    double masses[] = {mass0, mass1, mass2, mass3, mass4};
    RooRealVar mzd("mass", "m_{#mu#mu}", 0., 10., "GeV");
    mzd.setMin(0.);
    mzd.setMax(10.);

    // Dataset
    RooDataSet *treemass = new RooDataSet("mass", "", RooArgSet(mzd), RooFit::Import(*tree));
    RooWorkspace *w = new RooWorkspace("LowMassFits", "");

    TString *mlist[] = {"m0", "m1", "m2", "m3", "m4"};
    TString *siglist[] = {"sig0", "sig1", "sig2", "sig3", "sig4"};
    TString *lowlist[] = {"low0", "low1", "low2", "low3", "low4"};
    TString *highlist[] = {"high0", "high1", "high2", "high3", "high4"};

    for (int i = 0; i < 5; i++) {
        /*
         * Set roo variables for the masses
         */ 
        double k = 0.3;
        double sig_k = 0.02;

            if (i > 1) {
                k = 0.3;
            }
        
        double mass_lower = masses[i]*(1-k);
        double mass_upper = masses[i]*(1+k);
        double sig_lower = masses[i]*(1-sig_k);
        double sig_upper = masses[i]*(1+sig_k);
        sig_k += 0.01;

        mzd.setRange(Form("m%d", i), mass_lower, mass_upper);
        mzd.setRange(Form("sig%d", i), sig_lower, sig_upper);
        mzd.setRange(Form("low%d", i), mass_lower, sig_lower);
        mzd.setRange(Form("upper%d", i), sig_upper, mass_upper);

        /*
         * Define signal shape
         */ 
        TString bwfit = MakeBreitWigner(true, Form("BW_sig%d", i), mlist[i], 0.001, mzd, *w);
        TString cbfit = MakeCB(true, Form("CB_sig%d", i), mlist[i], mzd, *w);
        TString dcbfit = MakeDoubleCB(true, Form("DCB_sig%d", i), mlist[i], mzd, *w);
        TString dcbfit_NE = MakeDoubleCB(false, Form("DCB_NE_sig%d", i), mlist[i], mzd, *w);

        /*
         * Define background shape
         */ 
        TString dexpofit = MakeDoubleExpo(true, Form("DExpo_bkg%d", i), mzd, *w);
        TString dexpofit_NE = MakeDoubleExpo(false, Form("DExpo_NE_bkg%d", i), mzd, *w);
        TString expofit = MakeExpo(true, Form("Expo_bkg%d", i), mzd, *w);
        TString bernpoly2fit = MakeBernPoly2(true, Form("BernPoly2_bkg%d", i), mzd, *w);
        TString bernpoly2fit_NE = MakeBernPoly2(false, Form("BernPoly2_NE_bkg%d", i), mzd, *w);
        TString bernpoly3fit = MakeBernPoly3(true, Form("BernPoly3_bkg%d", i), mzd, *w);
        TString bernpoly3fit_NE = MakeBernPoly3(false, Form("BernPoly3_NE_bkg%d", i), mzd, *w);

        RooRealVar nsig("nsig", "", treemass->numEntries()/100.);
        RooRealVar nbkg("nbkg", "", treemass->numEntries()/100.);
        nsig.setConstant(kFALSE);
        nbkg.setConstant(kFALSE);
    
        /*
         * Set Parameters
         */ 
        w->var(cbfit+"_CB_sigma")->setVal(0.01);
        w->var(cbfit+"_CB_alpha")->setVal(1.);
        w->var(cbfit+"_CB_n")->setVal(1.);
        w->var(cbfit+"_Ns")->setVal(treemass->numEntries());

        w->var(dcbfit+"_CB_sigma1")->setVal(0.01);
        w->var(dcbfit+"_CB_alpha1")->setVal(1.);
        w->var(dcbfit+"_CB_n1")->setVal(1.);
        w->var(dcbfit+"_CB_sigma2")->setVal(0.02);
        w->var(dcbfit+"_CB_alpha2")->setVal(-2.);
        w->var(dcbfit+"_CB_n2")->setVal(1.);
        w->var(dcbfit+"_Ns")->setVal(treemass->numEntries());

        w->var(dcbfit_NE+"_CB_sigma1")->setVal(0.01);
        w->var(dcbfit_NE+"_CB_alpha1")->setVal(1.);
        w->var(dcbfit_NE+"_CB_n1")->setVal(1.);
        w->var(dcbfit_NE+"_CB_sigma2")->setVal(0.02);
        w->var(dcbfit_NE+"_CB_alpha2")->setVal(-2.);
        w->var(dcbfit_NE+"_CB_n2")->setVal(1.);

        w->var(bwfit+"_widthBW")->setVal(0.03);
        w->var(bwfit+"_Ns")->setVal(treemass->numEntries());

        w->var(dexpofit+"_lambdaExpo1")->setVal(-2.);
        w->var(dexpofit+"_lambdaExpo2")->setVal(-0.5);
        w->var(dexpofit+"_frac")->setVal(0.55);
        w->var(dexpofit+"_Nbkg")->setVal(treemass->numEntries());

        w->var(dexpofit_NE+"_lambdaExpo1")->setVal(-2.);
        w->var(dexpofit_NE+"_lambdaExpo2")->setVal(-0.5);
        w->var(dexpofit_NE+"_frac")->setVal(0.55);

        w->var(expofit+"_lambdaExpo")->setVal(-1.5);
        w->var(expofit+"_Nbkg")->setVal(treemass->numEntries());
        
        w->var(bernpoly2fit+"_pC")->setVal(10.);
        w->var(bernpoly2fit+"_p0")->setVal(1.);
        w->var(bernpoly2fit+"_p1")->setVal(-1.);
        w->var(bernpoly2fit+"_Nbkg")->setVal(treemass->numEntries());

        w->var(bernpoly2fit_NE+"_pC")->setVal(10.);
        w->var(bernpoly2fit_NE+"_p0")->setVal(1.);
        w->var(bernpoly2fit_NE+"_p1")->setVal(-1.);

        w->var(bernpoly3fit+"_pC")->setVal(10.);
        w->var(bernpoly3fit+"_p0")->setVal(1.);
        w->var(bernpoly3fit+"_p1")->setVal(-0.5);
        w->var(bernpoly3fit+"_p2")->setVal(-1.);
        w->var(bernpoly3fit+"_Nbkg")->setVal(treemass->numEntries());

        w->var(bernpoly3fit_NE+"_pC")->setVal(10.);
        w->var(bernpoly3fit_NE+"_p0")->setVal(1.);
        w->var(bernpoly3fit_NE+"_p1")->setVal(-0.5);
        w->var(bernpoly3fit_NE+"_p2")->setVal(-1.);


        /*
         * Define S+B model
         */ 
        // BW + Expo
        RooAddPdf *sb1 = new RooAddPdf("sb1", "sb1", RooArgList(*w->pdf(bwfit), *w->pdf(expofit)));
        // BW + Double Expo
        RooAddPdf *sb2 = new RooAddPdf("sb2", "sb2", RooArgList(*w->pdf(bwfit), *w->pdf(dexpofit)));
        // CB + Expo
        RooAddPdf *sb3 = new RooAddPdf("sb3", "sb3", RooArgList(*w->pdf(cbfit), *w->pdf(expofit)));
        // CB + Bernstein Poly Order 2
        RooAddPdf *sb4 = new RooAddPdf("sb4", "sb4", RooArgList(*w->pdf(cbfit), *w->pdf(bernpoly2fit)));
        // CB + Bernstein Poly Order 3
        RooAddPdf *sb5 = new RooAddPdf("sb5", "sb5", RooArgList(*w->pdf(cbfit), *w->pdf(bernpoly3fit)));
        // CB + Double Expo
        RooAddPdf *sb6 = new RooAddPdf("sb6", "sb6", RooArgList(*w->pdf(cbfit), *w->pdf(dexpofit)));
        // DCB + Double Expo
        RooAddPdf *sb7 = new RooAddPdf("sb7", "sb7", RooArgList(*w->pdf(dcbfit_NE), *w->pdf(dexpofit_NE)), RooArgList(nsig, nbkg));

        /*
         * Do S+B fit
         */ 
        sb1->fitTo(*treemass, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range(mlist[i]));
        sb2->fitTo(*treemass, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range(mlist[i]));
        sb3->fitTo(*treemass, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range(mlist[i]));
        sb4->fitTo(*treemass, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range(mlist[i]));
        sb5->fitTo(*treemass, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range(mlist[i]));
        sb7->fitTo(*treemass, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range(mlist[i]));


        /*
         * Plot S+B fit   
         */ 
        TCanvas *csb = new TCanvas("csb", "csb", 800, 700);
        RooPlot *frameSB = mzd.frame(mass_lower, mass_upper, 100);
        treemass->plotOn(frameSB);

        sb1->plotOn(frameSB, RooFit::Name("sb1"), RooFit::Range(mlist[i]));
        sb2->plotOn(frameSB, RooFit::Name("sb2"), RooFit::Range(mlist[i]), RooFit::LineColor(kGreen));
        sb3->plotOn(frameSB, RooFit::Name("sb3"), RooFit::Range(mlist[i]), RooFit::LineColor(kRed));
        sb4->plotOn(frameSB, RooFit::Name("sb4"), RooFit::Range(mlist[i]), RooFit::LineColor(kGray));
        sb5->plotOn(frameSB, RooFit::Name("sb5"), RooFit::Range(mlist[i]), RooFit::LineColor(kBlack));
        sb6->plotOn(frameSB, RooFit::Name("sb6"), RooFit::Range(mlist[i]), RooFit::LineColor(46));
        sb7->plotOn(frameSB, RooFit::Name("sb7"), RooFit::Range(mlist[i]), RooFit::LineColor(40));

        csb->cd();
        frameSB->Draw();
        frameSB->SetTitle("");
        
        TLegend *legSB = new TLegend(0.1,0.7,0.4,0.9);
        legSB->AddEntry(frameSB->findObject("sb1"), "bw + expo");
        legSB->AddEntry(frameSB->findObject("sb2"), "bw + dexpo");
        legSB->AddEntry(frameSB->findObject("sb3"), "cb + expo");
        legSB->AddEntry(frameSB->findObject("sb4"), "cb + bernpoly2");
        legSB->AddEntry(frameSB->findObject("sb5"), "cb + bernpoly3");
        legSB->AddEntry(frameSB->findObject("sb6"), "cb + dexpo");
        legSB->AddEntry(frameSB->findObject("sb7"), "dcb + expo");
        legSB->Draw();

        csb->SetLogy();
        csb->Update();
        csb->SaveAs(Form("sb%d.png", i));

        w->import(*frameSB);


        /*
         * Test S fit
         */ 
        if (testS) {
            RooFitResult *bwfitres = w->pdf(bwfit)->fitTo(*treemass, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range(siglist[i]));
            RooFitResult *cbfitres = w->pdf(cbfit)->fitTo(*treemass, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range(siglist[i]));
            RooFitResult *dcbfitres = w->pdf(dcbfit)->fitTo(*treemass, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range(siglist[i]));

            /*
             * Plot S fit
             */ 
            TCanvas *cs = new TCanvas("cs", "cs", 800, 700);
            RooPlot *frameS = mzd.frame(mass_lower, mass_upper, 100);
            tree_mass->plotOn(frameS);
            w->pdf(bwfit)->plotOn(frameS, RooFit::Name("bwfit"));
            w->pdf(cbfit)->plotOn(frameS, RooFit::Name("cbfit"), RooFit::LineColor(kRed));
            w->pdf(dcbfit)->plotOn(frameS, RooFit::Name("dcbfit"), RooFit::LineColor(46));
            w->pdf(dcbfit_NE)->plotOn(frameS, RooFit::Name("dcbfit_NE"), RooFit::LineColor(kGreen));
            frameS->Draw();
            frameS->SetTitle("");

            TLegend *legS = new TLegend(0.1,0.7,0.4,0.9);
            legS->AddEntry(frameS->findObject("bwfit"), "bw");
            legS->AddEntry(frameS->findObject("cbfit"), "cb");
            legS->AddEntry(frameS->findObject("dcbfit"), "dcb");
            legS->AddEntry(frameS->findObject("dcbfit_NE"), "dcb_NE");
            legS->Draw();

            cs->SetLogy();
            cs->Update();
            cs->SaveAs(Form("s%d.png", i));

            w->import(*frameS);
        }


        /*
         * Test B fit
         */ 
        if (testB) {
            RooFitResult *expofitres = w->pdf(expofit)->fitTo(*treemass, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range(lowlist[i]+","+highlist[i]));
            RooFitResult *dexpofitres = w->pdf(dexpofit)->fitTo(*treemass, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range(lowlist[i]+","+highlist[i]));
            RooFitResult *bernpoly2fitNEres = w->pdf(bernpoly2fit_NE)->fitTo(*treemass, RooFit::Save(kTRUE), RooFit::Range(lowlist[i]+","+highlist[i]));
            RooFitResult *bernpoly3fitres = w->pdf(bernpoly3fit)->fitTo(*treemass, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range(lowlist[i]+","+highlist[i]));
            RooFitResult *bernpoly3fitNEres = w->pdf(bernpoly3fit_NE)->fitTo(*treemass, RooFit::Save(kTRUE), RooFit::Range(lowlist[i]+","+highlist[i]));

            /*
             * Plot B fit
             */ 
            TCanvas *cb = new TCanvas("cb", "cb", 800, 700);
            RooPlot *frameB = mzd.frame(mass_lower, mass_upper, 100);
            treemass->plotOn(frameB);
            w->pdf(dexpofit)->plotOn(frameB, RooFit::Name("dexpofit"), RooFit::LineColor(kRed));
            w->pdf(dexpofit_NE)->plotOn(frameB, RooFit::Name("dexpofit_NE"), RooFit::LineColor(46));
            w->pdf(expofit)->plotOn(frameB, RooFit::Name("expofit"));
            w->pdf(bernpoly2fit_NE)->plotOn(frameB, RooFit::Name("bernpoly2fit_NE"), RooFit::LineColor(kGreen));
            w->pdf(bernpoly3fit)->plotOn(frameB, RooFit::Name("bernpoly3fit"), RooFit::LineColor(kGray));
            w->pdf(bernpoly3fit_NE)->plotOn(frameB, RooFit::Name("bernpoly3fit_NE"), RooFit::LineColor(kGray));
            frameB->Draw();
            frameB->SetTitle("");

            TLegend *legB = new TLegend(0.1,0.7,0.4,0.9);
            legB->AddEntry(frameB->findObject("expofit"), "expo");
            legB->AddEntry(frameB->findObject("dexpofit"), "dexpo");
            legB->AddEntry(frameB->findObject("dexpofit_NE"), "dexpo_NE");
            legB->AddEntry(frameB->findObject("bernpoly2fit_NE"), "bernpoly2_NE");
    //        legB->AddEntry(frameB->findObject("bernpoly3fit"), "bernpoly3");
            legB->AddEntry(frameB->findObject("bernpoly3fit_NE"), "bernpoly3_NE");
            legB->Draw();

            cb->SetLogy();
            cb->Update();
            cb->SaveAs(Form("b%d.png", i));

            w->import(*frameB);
        }
    }

    TFile *outfile = new TFile("outfile.root", "recreate");
    w->Write("LowMassFits");

    return 0;
}
#endif

/*
 * S+B fitting tested on JPsi resonance.
 *
 * Args 
 *      tree: input TTree containing data.
 *
 * Returns
 *      0 if no errors.
 */    
int SplusB_fit_test(TTree* tree, bool totalEntries, const char* fitOutFile, TString imgTag) {
    // Print tree for checking
//    tree->Print();

    // ~3.1 GeV - JPsi
    double mass4 = 3.1;
    
    TFile *file = new TFile("LowMassFits_" + imgTag + ".root", "recreate");
    RooWorkspace *w = new RooWorkspace("LowMassFits", "");

    // Define Roo variable for mass
    RooRealVar mzd("mass", "m_{#mu#mu}", 0., 10., "GeV");
    mzd.setMin(0.);
    mzd.setMax(10.);

    // Test fitting JPsi first
    mzd.setRange("m4", 2.0, 3.5);
    
    // Test fitting background only
    mzd.setRange("low", 2.0, 2.9);
    mzd.setRange("high", 3.2225, 3.5);

    // Test fitting signal only
    mzd.setRange("sig", 2.9, 3.225);

    // Dataset: TTree
    tree->GetBranch("mass");
    RooDataSet *treemass = new RooDataSet("mass", "", RooArgSet(mzd), RooFit::Import(*tree));
    delete tree;
  
    // mass around JPsi
    RooDataSet *massCut4 = (RooDataSet*) treemass->reduce(RooFit::Name("massCut4"), RooFit::SelectVars(RooArgSet(mzd)), RooFit::CutRange("m4"));

    // Some index for naming; remnant from previous code but I think won't remove for now.
    int i = 4;

    /*
     * Define signal shape
     * "_NE" means non-extended PDF.
     */ 
    TString dcbfit_NE = MakeDoubleCB(false, Form("DCB_NE_sig%d", i), mass4, mzd, *w);
/* 
    TString bwfit = MakeBreitWigner(true, Form("BW_sig%d", i), mass4, 0.001, mzd, *w);
    TString bwfit_NE = MakeBreitWigner(false, Form("BW_sig%d", i), mass4, 0.001, mzd, *w);
    TString cbfit = MakeCB(true, Form("CB_sig%d", i), mass4, mzd, *w);
    TString dcbfit = MakeDoubleCB(true, Form("DCB_sig%d", i), mass4, mzd, *w);
    TString dcbfit_NE_old = MakeDoubleCB_NE(Form("DCB_NE_sig_old%d", i), mass4, mzd, *w);
*/

    /*
     * Define background shape
     * "_NE" means non-extended PDF.
     */ 
    TString dexpofit_NE = MakeDoubleExpo(false, Form("DExpo_NE_bkg%d", i), mzd, *w);
/*
    TString dexpofit = MakeDoubleExpo(true, Form("DExpo_bkg%d", i), mzd, *w);
    TString expofit = MakeExpo(true, Form("Expo_bkg%d", i), mzd, *w);
    TString bernpoly2fit = MakeBernPoly2(true, Form("BernPoly2_bkg%d", i), mzd, *w);
    TString bernpoly2fit_NE = MakeBernPoly2(false, Form("BernPoly2_NE_bkg%d", i), mzd, *w);
    TString bernpoly3fit = MakeBernPoly3(true, Form("BernPoly3_bkg%d", i), mzd, *w);
    TString bernpoly3fit_NE = MakeBernPoly3(false, Form("BernPoly3_NE_bkg%d", i), mzd, *w);
    TString bernpoly4fit_NE = MakeBernPoly4(false, Form("BernPoly4_bkg%d", i), mzd, *w);
    TString bernpoly5fit_NE = MakeBernPoly5(false, Form("BernPoly5_bkg%d", i), mzd, *w);
*/

    // Estimate number of signal and background events.
    // Using first fit as guess
    // V1
/*
    RooRealVar nsig("nsig", "", 8.1164e+05);
    RooRealVar nbkg("nbkg", "", 3.9635e+06);
*/
/*
    // V2 - using values from fit with xcgjob0
    RooRealVar nsig("nsig", "", 9.24227e+05);
    RooRealVar nbkg("nbkg", "", 4.36363e+06);
*/    
    // V3 - for finding scaling of nentries with time taken to fit
    RooRealVar nsig("nsig", "", massCut4->numEntries() / 5);
    RooRealVar nbkg("nbkg", "", massCut4->numEntries());

    /*
     * option 'totalEntries' specifies whether to use total number of entries 
     * from 0-10GeV in the file for initial nsig or nbkg.
     *      totalEntries = 1 : use total number of entries from 0 -10 GeV
     *      totalEntries = 0 : use number of entries from 2.0 - 3.5 GeV
     */
    if (totalEntries) {
        nsig.setVal(treemass->numEntries() / 5);
        nbkg.setVal(treemass->numEntries());
    }

    std::cout << "nsig: " << nsig.getValV() << std::endl;
    std::cout << "nbkg: " << nbkg.getValV() << std::endl;

    nsig.setConstant(kFALSE);
    nbkg.setConstant(kFALSE);

    /*
     * Set Parameters
     */ 
/*  
    w->var(cbfit+"_CB_sigma")->setVal(0.01);
    w->var(cbfit+"_CB_alpha")->setVal(1.);
    w->var(cbfit+"_CB_n")->setVal(1.);
    w->var(cbfit+"_Ns")->setVal(massCut4->numEntries());

    w->var(dcbfit+"_CB_sigma1")->setVal(0.01);
    w->var(dcbfit+"_CB_alpha1")->setVal(0.01);
    w->var(dcbfit+"_CB_n1")->setVal(1.);
    w->var(dcbfit+"_CB_sigma2")->setVal(0.01);
    w->var(dcbfit+"_CB_alpha2")->setVal(-1.);
    w->var(dcbfit+"_CB_n2")->setVal(10.);
    w->var(dcbfit+"_Ns")->setVal(massCut4->numEntries());
    
    w->var(dcbfit_NE_old+"_CB_mu1")->setVal(3.0764e+00);
    w->var(dcbfit_NE_old+"_CB_alpha1")->setVal(1.8578e+00);
    w->var(dcbfit_NE_old+"_CB_sigma1")->setVal(4.2416e-02);
    w->var(dcbfit_NE_old+"_CB_n1")->setVal(3.6193e-01);
    w->var(dcbfit_NE_old+"_CB_mu2")->setVal(3.0896e+00);
    w->var(dcbfit_NE_old+"_CB_alpha2")->setVal(-3.6268e+00);
    w->var(dcbfit_NE_old+"_CB_sigma2")->setVal(2.1662e-02);
    w->var(dcbfit_NE_old+"_CB_n2")->setVal(4.8241e-01);
    w->var(dcbfit_NE_old+"_frac")->setVal(4.1662e-01);

    // V1
    w->var(dcbfit_NE+"_CB_mu1")->setVal(3.0764e+00);
    w->var(dcbfit_NE+"_CB_alpha1")->setVal(1.8578e+00);
    w->var(dcbfit_NE+"_CB_sigma1")->setVal(4.2416e-02);
    w->var(dcbfit_NE+"_CB_n1")->setVal(3.6193e-01);
    w->var(dcbfit_NE+"_CB_mu2")->setVal(3.0896e+00);
    w->var(dcbfit_NE+"_CB_alpha2")->setVal(-3.6268e+00);
    w->var(dcbfit_NE+"_CB_sigma2")->setVal(2.1662e-02);
    w->var(dcbfit_NE+"_CB_n2")->setVal(4.8241e-01);
    w->var(dcbfit_NE+"_frac")->setVal(4.1662e-01);
*/
/*
    // V2 - from xcgjob0 fit
    w->var(dcbfit_NE+"_CB_mu1")->setVal(3.08436e+00);
    w->var(dcbfit_NE+"_CB_alpha1")->setVal(1.85519e+00);
    w->var(dcbfit_NE+"_CB_sigma1")->setVal(5.63970e-02);
    w->var(dcbfit_NE+"_CB_n1")->setVal(1.01862e+00);
    w->var(dcbfit_NE+"_CB_mu2")->setVal(3.09119e+00);
    w->var(dcbfit_NE+"_CB_alpha2")->setVal(-3.77213e+00);
    w->var(dcbfit_NE+"_CB_sigma2")->setVal(2.75309e-02);
    w->var(dcbfit_NE+"_CB_n2")->setVal(1.97169e-07);
    w->var(dcbfit_NE+"_frac")->setVal(4.98426e-01);
*/ 
/*
    // V3 - for testing, from initial 500 000 entry file fit
    w->var(dcbfit_NE+"_CB_mu1")->setVal(3.06050e+00);
//    w->var(dcbfit_NE+"_CB_alpha1")->setVal(-3.33886e+09);
    w->var(dcbfit_NE+"_CB_alpha1")->setVal(1.33886e+00);
    w->var(dcbfit_NE+"_CB_sigma1")->setVal(9.57192e-02);
    w->var(dcbfit_NE+"_CB_n1")->setVal(9.47739e+00);
    w->var(dcbfit_NE+"_CB_mu2")->setVal(3.09025e+00);
//    w->var(dcbfit_NE+"_CB_alpha2")->setVal(-2.66052e+00);
    w->var(dcbfit_NE+"_CB_alpha2")->setVal(-3.66052e+00);
    w->var(dcbfit_NE+"_CB_sigma2")->setVal(3.53358e-02);
    w->var(dcbfit_NE+"_CB_n2")->setVal(2.10568e+01);
    w->var(dcbfit_NE+"_frac")->setVal(1.50803e-01);
*/
    // V4 - for testing, from 1 000 000 entry file fit, Strategy 1, total entries
    w->var(dcbfit_NE+"_CB_mu1")->setVal(3.0632e+00);
    w->var(dcbfit_NE+"_CB_alpha1")->setVal(1.6663e+00);
    w->var(dcbfit_NE+"_CB_sigma1")->setVal(5.1557e-02);
    w->var(dcbfit_NE+"_CB_n1")->setVal(7.8780e-01);
    w->var(dcbfit_NE+"_CB_mu2")->setVal(3.0931e+00);
    w->var(dcbfit_NE+"_CB_alpha2")->setVal(-1.0267e+00);
    w->var(dcbfit_NE+"_CB_sigma2")->setVal(2.9637e-02);
    w->var(dcbfit_NE+"_CB_n2")->setVal(1.0033e+02);
    w->var(dcbfit_NE+"_frac")->setVal(3.6957e-01);

/*    
    w->var(bwfit+"_widthBW")->setVal(0.03);
    w->var(bwfit+"_Ns")->setVal(massCut4->numEntries());

    w->var(bwfit_NE+"_widthBW")->setVal(0.03);

    w->var(dexpofit+"_lambdaExpo1")->setVal(-4.9988e-01);
    w->var(dexpofit+"_lambdaExpo2")->setVal(-9.4254e-01);
    w->var(dexpofit+"_frac")->setVal(1.2962e-04);
    w->var(dexpofit+"_Nbkg")->setVal(massCut4->numEntries());

    // V1
    w->var(dexpofit_NE+"_lambdaExpo1")->setVal(-4.9988e-01);
    w->var(dexpofit_NE+"_lambdaExpo2")->setVal(-9.4254e-01);
    w->var(dexpofit_NE+"_frac")->setVal(1.2962e-04);
*/
/*
    // V2 - from xcgjob0 fit
    w->var(dexpofit_NE+"_lambdaExpo1")->setVal(-2.73926e-01);
    w->var(dexpofit_NE+"_lambdaExpo2")->setVal(-7.30272e-01);
    w->var(dexpofit_NE+"_frac")->setVal(6.13955e-08);
*/   
/*
    // V3 - for testing, from initial 500 000 entry file fit
    w->var(dexpofit_NE+"_lambdaExpo1")->setVal(3.06624e-01);
    w->var(dexpofit_NE+"_lambdaExpo2")->setVal(-8.05551e-01);
    w->var(dexpofit_NE+"_frac")->setVal(5.97124e-02);
*/
    // V4 - for testing, from 1 000 000 entry file fit, Strategy 1, total entries
    w->var(dexpofit_NE+"_lambdaExpo1")->setVal(2.4502e-02);
    w->var(dexpofit_NE+"_lambdaExpo2")->setVal(-7.8504e-01);
    w->var(dexpofit_NE+"_frac")->setVal(2.7871e-02);

/*
    w->var(expofit+"_lambdaExpo")->setVal(-1.5);
    w->var(expofit+"_Nbkg")->setVal(massCut4->numEntries());
    
    w->var(bernpoly2fit+"_pC")->setVal(10.);
    w->var(bernpoly2fit+"_p0")->setVal(1.);
    w->var(bernpoly2fit+"_p1")->setVal(-1.);
    w->var(bernpoly2fit+"_Nbkg")->setVal(massCut4->numEntries());

    w->var(bernpoly2fit_NE+"_pC")->setVal(10.);
    w->var(bernpoly2fit_NE+"_p0")->setVal(1.);
    w->var(bernpoly2fit_NE+"_p1")->setVal(-1.);

    w->var(bernpoly3fit+"_pC")->setVal(10.);
    w->var(bernpoly3fit+"_p0")->setVal(1.);
    w->var(bernpoly3fit+"_p1")->setVal(-0.5);
    w->var(bernpoly3fit+"_p2")->setVal(-1.);
    w->var(bernpoly3fit+"_Nbkg")->setVal(massCut4->numEntries());

    w->var(bernpoly3fit_NE+"_pC")->setVal(10.);
    w->var(bernpoly3fit_NE+"_p0")->setVal(1.);
    w->var(bernpoly3fit_NE+"_p1")->setVal(-0.5);
    w->var(bernpoly3fit_NE+"_p2")->setVal(-1.);

    w->var(bernpoly4fit_NE+"_pC")->setVal(10.);
    w->var(bernpoly4fit_NE+"_p0")->setVal(1.);
    w->var(bernpoly4fit_NE+"_p1")->setVal(-0.5);
    w->var(bernpoly4fit_NE+"_p2")->setVal(-1.);
    w->var(bernpoly4fit_NE+"_p3")->setVal(-1.5);

    w->var(bernpoly5fit_NE+"_pC")->setVal(10.);
    w->var(bernpoly5fit_NE+"_p0")->setVal(1.);
    w->var(bernpoly5fit_NE+"_p1")->setVal(-0.5);
    w->var(bernpoly5fit_NE+"_p2")->setVal(-1.);
    w->var(bernpoly5fit_NE+"_p3")->setVal(-1.5);
    w->var(bernpoly5fit_NE+"_p4")->setVal(-1.5);
*/
    /*
     * Define S+B model
     */ 
/*  
    // BW + Expo
    RooAddPdf *sb1 = new RooAddPdf("sb1", "sb1", RooArgList(*w->pdf(bwfit), *w->pdf(expofit)));

    // BW + Double Expo
    RooAddPdf *sb2 = new RooAddPdf("sb2", "sb2", RooArgList(*w->pdf(bwfit_NE), *w->pdf(dexpofit_NE)), RooArgList(nsig, nbkg));
    // CB + Expo
    RooAddPdf *sb3 = new RooAddPdf("sb3", "sb3", RooArgList(*w->pdf(cbfit), *w->pdf(expofit)));

    // CB + Bernstein Poly Order 2
    RooAddPdf *sb4 = new RooAddPdf("sb4", "sb4", RooArgList(*w->pdf(cbfit), *w->pdf(bernpoly2fit)));

    // CB + Bernstein Poly Order 3
    RooAddPdf *sb5 = new RooAddPdf("sb5", "sb5", RooArgList(*w->pdf(cbfit), *w->pdf(bernpoly3fit)));

    // CB + Double Expo
    RooAddPdf *sb6 = new RooAddPdf("sb6", "sb6", RooArgList(*w->pdf(cbfit), *w->pdf(dexpofit)));
*/
    // DCB + Double Expo
    RooAddPdf *sb7 = new RooAddPdf("sb7", "sb7", RooArgList(*w->pdf(dcbfit_NE), *w->pdf(dexpofit_NE)), RooArgList(nsig, nbkg));

    /*
     * Do S+B fit
     */
/*
    sb1->fitTo(*massCut4, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range("m4"));
    sb2->fitTo(*massCut4, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range("m4"));
    sb3->fitTo(*massCut4, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range("m4"));
    sb4->fitTo(*massCut4, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range("m4"));
    sb5->fitTo(*massCut4, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range("m4"));
*/    
    TStopwatch t;
    t.Start();
    RooFitResult *sb7res = sb7->fitTo(*massCut4, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range("m4"), RooFit::Timer(1), RooFit::Strategy(1), RooFit::NumCPU(32));
    t.Print();

    /*
     * Test S fit
     */ 

//    RooFitResult *bwfitres = w->pdf(bwfit)->fitTo(*massCut4, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range("sig"));
//    RooFitResult *cbfitres = w->pdf(cbfit)->fitTo(*massCut4, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range("sig"));
//    RooFitResult *dcbfitres = w->pdf(dcbfit)->fitTo(*massCut4, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range("sig"), RooFit::Timer(1));
//    RooFitResult *dcbfitres_NE = w->pdf(dcbfit_NE)->fitTo(*massCut4, RooFit::Save(kTRUE), RooFit::Range("sig"), RooFit::Timer(1), RooFit::Strategy(1));


    /*
     * Test B fit
     */
//        RooFitResult *expofitres = w->pdf(expofit)->fitTo(*massCut4, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range("low,high"));
//        RooFitResult *dexpofitres = w->pdf(dexpofit)->fitTo(*massCut4, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range("low,high"));
//        RooFitResult *dexpofitres_NE = w->pdf(dexpofit_NE)->fitTo(*massCut4, RooFit::Save(kTRUE), RooFit::Range("low,high"), RooFit::Timer(1));
//      RooFitResult *bernpoly2fitNEres = w->pdf(bernpoly2fit_NE)->fitTo(*massCut4, RooFit::Save(kTRUE), RooFit::Range("low,high"));
//      RooFitResult *bernpoly3fitres = w->pdf(bernpoly3fit)->fitTo(*massCut4, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range("low,high"));
//      RooFitResult *bernpoly3fitNEres = w->pdf(bernpoly3fit_NE)->fitTo(*massCut4, RooFit::Save(kTRUE), RooFit::Range("low,high"));
//      RooFitResult *bernpoly4fitNEres = w->pdf(bernpoly4fit_NE)->fitTo(*massCut4, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range("low,high"));
//      RooFitResult *bernpoly5fitNEres = w->pdf(bernpoly5fit_NE)->fitTo(*massCut4, RooFit::Extended(kTRUE), RooFit::Save(kTRUE), RooFit::Range("low,high"));

    /*
     * Plot B fit
     */
    TCanvas *cb = new TCanvas("cb", "cb", 800, 700);
    RooPlot *frameB = mzd.frame(2.0, 3.5, 100);
    massCut4->plotOn(frameB);

    w->pdf(dexpofit_NE)->plotOn(frameB, RooFit::Name("dexpofit_NE"), RooFit::LineColor(46), RooFit::Range("low,high"), RooFit::NormRange("low,high"));
//    w->pdf(dexpofit)->plotOn(frameB, RooFit::Name("dexpofit"), RooFit::LineColor(kRed));
//    w->pdf(expofit)->plotOn(frameB, RooFit::Name("expofit"));
//    w->pdf(bernpoly2fit_NE)->plotOn(frameB, RooFit::Name("bernpoly2fit_NE"), RooFit::LineColor(kGreen));
//    w->pdf(bernpoly3fit)->plotOn(frameB, RooFit::Name("bernpoly3fit"), RooFit::LineColor(kGray));
//    w->pdf(bernpoly3fit_NE)->plotOn(frameB, RooFit::Name("bernpoly3fit_NE"), RooFit::LineColor(kGray));
//    W->PDF(Bernpoly4fit_NE)->plotOn(frameB, RooFit::Name("bernpoly4fit_NE"), RooFit::LineColor(kBlue));
//    w->pdf(bernpoly5fit_NE)->plotOn(frameB, RooFit::Name("bernpoly5fit_NE"), RooFit::LineColor(kRed));
    frameB->Draw();
    frameB->SetTitle("");

    TLegend *legB = new TLegend(0.1,0.7,0.4,0.9);
    legB->AddEntry(frameB->findObject("dexpofit_NE"), "dexpo_NE");
//    legB->AddEntry(frameB->findObject("expofit"), "expo");
//    legB->AddEntry(frameB->findObject("dexpofit"), "dexpo");
//    legB->AddEntry(frameB->findObject("bernpoly2fit_NE"), "bernpoly2_NE");
//    legB->AddEntry(frameB->findObject("bernpoly3fit"), "bernpoly3");
//    legB->AddEntry(frameB->findObject("bernpoly3fit_NE"), "bernpoly3_NE");
//    legB->AddEntry(frameB->findObject("bernpoly4fit_NE"), "bernpoly4_NE");
//    legB->AddEntry(frameB->findObject("bernpoly5fit_NE"), "bernpoly5_NE");
    legB->Draw();

    cb->SetLogy();
    cb->Update();
    cb->SaveAs("b_JPsi_" + imgTag + ".png");

    /*
     * Plot S fit
     */
    TCanvas *cs = new TCanvas("cs", "cs", 800, 700);
    RooPlot *frameS = mzd.frame(2.0, 3.5, 100);
    massCut4->plotOn(frameS);

    w->pdf(dcbfit_NE)->plotOn(frameS, RooFit::Name("dcbfit_NE"), RooFit::LineColor(kGreen), RooFit::Range("sig"), RooFit::NormRange("sig"));
//    w->pdf(bwfit)->plotOn(frameS, RooFit::Name("bwfit"));
//    w->pdf(cbfit)->plotOn(frameS, RooFit::Name("cbfit"), RooFit::LineColor(kRed));
//    w->pdf(dcbfit)->plotOn(frameS, RooFit::Name("dcbfit"), RooFit::LineColor(kGray));
//    w->pdf(dcbfit_NE_old)->plotOn(frameS, RooFit::Name("dcbfit_NE_old"), RooFit::LineColor(kRed));
    frameS->Draw();
    frameS->SetTitle("");

    TLegend *legS = new TLegend(0.1,0.7,0.4,0.9);
    legS->AddEntry(frameS->findObject("dcbfit_NE"), "dcb_NE");
//    legS->AddEntry(frameS->findObject("bwfit"), "bw");
//    legS->AddEntry(frameS->findObject("cbfit"), "cb");
//    legS->AddEntry(frameS->findObject("dcbfit"), "dcb");
//    legS->AddEntry(frameS->findObject("dcbfit_NE_old"), "dcb_NE_old");
    legS->Draw();

    cs->SetLogy();
    cs->Update();
    cs->SaveAs("s_JPsi_" + imgTag + ".png");


    /*
     * Plot S+B fit   
     */
    TCanvas *csb = new TCanvas("csb", "csb", 800, 700);
    RooPlot *frameSB = mzd.frame(2., 3.5, 100);
    massCut4->plotOn(frameSB, RooFit::Name("data"));
/*
    sb1->plotOn(frameSB, RooFit::Name("sb1"), RooFit::Range("m4"));
    SB2->plotOn(frameSB, RooFit::Name("sb2"), RooFit::Range("m4"), RooFit::LineColor(kGreen));
    sb3->plotOn(frameSB, RooFit::Name("sb3"), RooFit::Range("m4"), RooFit::LineColor(kRed));
    sb4->plotOn(frameSB, RooFit::Name("sb4"), RooFit::Range("m4"), RooFit::LineColor(kGray));
    sb5->plotOn(frameSB, RooFit::Name("sb5"), RooFit::Range("m4"), RooFit::LineColor(kBlack));
    sb6->plotOn(frameSB, RooFit::Name("sb6"), RooFit::Range("m4"), RooFit::LineColor(46));
*/
    sb7->plotOn(frameSB, RooFit::Name("sb7"), RooFit::LineColor(40), RooFit::Range("m4"), RooFit::NormRange("m4"));

    csb->cd();
    frameSB->Draw();
    frameSB->SetTitle("");
    
    TLegend *legSB = new TLegend(0.1,0.7,0.4,0.9);
    legSB->AddEntry(frameSB->findObject("sb7"), "dcb + dexpo");
/*
    legSB->AddEntry(frameSB->findObject("sb1"), "bw + expo");  
    legSB->AddEntry(frameSB->findObject("sb2"), "bw + dexpo");
    legSB->AddEntry(frameSB->findObject("sb3"), "cb + expo");
    legSB->AddEntry(frameSB->findObject("sb4"), "cb + bernpoly2");
    legSB->AddEntry(frameSB->findObject("sb5"), "cb + bernpoly3");
    legSB->AddEntry(frameSB->findObject("sb6"), "cb + dexpo");
*/
    legSB->Draw();

    csb->SetLogy();
    csb->Update();
    csb->SaveAs("sb_JPsi_" + imgTag + ".png");
    
    /*
     * Get fit output to txt file
     */ 
    std::ofstream fit_file;
    fit_file.open(fitOutFile);

    if (fit_file.is_open()) {
        sb7res->printMultiline(fit_file, 0, kTRUE);
        fit_file << "Number of NLL evaluations with problems: " << sb7res->numInvalidNLL() << std::endl;
        fit_file << "chi2 / ndof: " << frameSB->chiSquare("sb7", "data") << std::endl;

        fit_file.close();
    }
        
    else {
        std::cerr << "Unable to open fit output file." << std::endl;
    }

    // Import information to RooWorkspace
    w->import(*massCut4);
    w->import(*sb7res);
    w->import(nsig);
    w->import(nbkg);
    w->import(*frameSB);

    // Write file
    w->Write("LowMassFits");
    file->cd();
    file->Close();

    return 0;
}


/*
 * Fits the signal and background separately to get parametrized shapes. The shapes are used to write a data card for combine.
 *
 * Args
 *     sig_fit: signal shape
 *     bkg_fit: background shape
 *     treeSignal: TTree containing MC signal 
 *     treeData: TTree containing real data
 *     vtxCut: vertex cut to implement
 *     mass: mass of the dark photon
 *     lifetime: lifetime of the dark photon
 *     binNumber: number of bins 
 *     combineRootFileName: filename for the ROOT file containing the workspace.
 *     datacardName: filename for the data card
 *
 * Returns
 *      The workspace (not the combine workspace) containing the fitted shapes.
 */
RooWorkspace *MakeDataCard(std::string sig_fit, std::string bkg_fit, TTree* treeSignal, TTree* treeData, std::string vtxCut, double mass, double lifetime, TString binNumber, TString combineRootFileName, TString datacardName) {
        system("mkdir -p datacards/");
	TFile *file = new TFile("datacards/" + combineRootFileName, "recreate");

	float sideband_width = 2.;
	float window_min = 18.5;
	float window_max = 21.5;
	float sideband_min = window_min - sideband_width;
	float sideband_max = window_max + sideband_width;
	float window_width = window_max - window_min;
	float bkg_weight = window_width / sideband_width;

	RooWorkspace *ws = new RooWorkspace("ws", "");
	RooRealVar mzd("mass", "m_{#mu#mu}", sideband_min, sideband_max, "GeV");
	mzd.setMin(sideband_min);
	mzd.setMax(sideband_max);
	mzd.setUnit("GeV");
	mzd.setBins(50);
//	mzd.setBins(100);
	mzd.setRange("low", sideband_min, window_min);
	mzd.setRange("signal", window_min, window_max);
	mzd.setRange("high", window_max, sideband_max);
	mzd.setRange("full", sideband_min, sideband_max);

	//-------------------------------------------
	// Import Data and Signal
	// ------------------------------------------
	
	treeData->GetBranch("mass");
	// Full Data
	RooDataSet data("data", "", RooArgSet(mzd), RooFit::Import(*treeData));
	// Data in sidebands only
	RooDataSet* dataSB = (RooDataSet*) data.reduce(RooFit::Name("dataSB"), RooFit::SelectVars(RooArgSet(mzd)), RooFit::CutRange("low"));
	RooDataSet* dataHigh = (RooDataSet*) data.reduce(RooFit::Name("dataHigh"), RooFit::SelectVars(RooArgSet(mzd)), RooFit::CutRange("high"));
	dataSB->append(*dataHigh);
	
	std::cout << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << "[INFO]:" << std::endl;
	std::cout << "Full Data Info" << std::endl;
	data.Print();
	std::cout << "================================================================================" << std::endl;
	std::cout << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << "[INFO]:" << std::endl;
	std::cout << "Sideband Data Info" << std::endl;
	dataSB->Print();
	std::cout << "================================================================================" << std::endl;
	std::cout << std::endl;

	treeSignal->GetBranch("mass");
	// Full Data
	RooDataSet dataSignal("dataSignal", "", RooArgSet(mzd), RooFit::Import(*treeSignal));
	// Data in window only
	RooDataSet* dataSigWindow = (RooDataSet*) dataSignal.reduce(RooFit::Name("dataSigWindow"), RooFit::SelectVars(RooArgSet(mzd)), RooFit::CutRange("signal"));

	std::cout << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << "[INFO]:" << std::endl;
	std::cout << "Signal Data Info" << std::endl;
	dataSignal.Print();
	std::cout << "================================================================================" << std::endl;
	std::cout << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << "[INFO]:" << std::endl;
	std::cout << "Signal Window Data Info" << std::endl;
	dataSigWindow->Print();
	std::cout << "================================================================================" << std::endl;
	std::cout << std::endl;

	//-------------------------------------------
	// Background Fit
	// ------------------------------------------
	expoFit_output bkgExpoFit;
	doubleExpoFit_output bkgDExpoFit;
	doubleExpoN1N2Fit_output bkgDExpoN1N2Fit;
	powFit_output bkgPowFit;
	doublePowFit_output bkgDPowFit;
	bernPoly2Fit_output bkgBernPoly2Fit;
	chebPoly2Fit_output bkgChebPoly2Fit;
	bernPoly3Fit_output bkgBernPoly3Fit;

	TString tag_bkg;	

	double Nbkg, Nbkg1, Nbkg2;

	// Do Fitting
	if (bkg_fit == "expo") {
		// bkgExpoFit = {tag, sE_lambda_bkg, sE_Nbkg, npoints}
		bkgExpoFit = SidebandExpoFit(dataSB, mzd, *ws);
		tag_bkg = bkgExpoFit.tag;
		Nbkg = bkgExpoFit.Nbkg;
	}

	else if (bkg_fit == "doubleExpo") {
		// bkgDExpoFit = {tag, dE_lambda1_bkg, dE_lambda2_bkg, dE_Nbkg, npoints}
		bkgDExpoFit = SidebandDExpoFit(dataSB, mzd, *ws);
		tag_bkg = bkgDExpoFit.tag;
		Nbkg = bkgDExpoFit.Nbkg;
	}

	else if (bkg_fit == "doubleExpoN1N2") {
		// bkgDExpoFit = {tag, dE_lambda1_bkg, dE_lambda2_bkg, dE_Nbkg1,dE_Nbkg2,  npoints}
		bkgDExpoN1N2Fit = SidebandDExpoN1N2Fit(dataSB, mzd, *ws);
		tag_bkg = bkgDExpoN1N2Fit.tag;
		Nbkg1 = bkgDExpoN1N2Fit.Nbkg1;
		Nbkg2 = bkgDExpoN1N2Fit.Nbkg2;
		Nbkg = Nbkg1*Nbkg1 + Nbkg2*Nbkg2;
	}

	else if (bkg_fit == "pow") {
		// bkgPowFit = {tag, sP_alpha_bkg, sP_Nbkg, npoints}
		bkgPowFit = SidebandPowFit(dataSB, mzd, *ws);
		tag_bkg = bkgPowFit.tag;
		Nbkg = bkgPowFit.Nbkg;
	}

	else if (bkg_fit == "doublePow") {
		// bkgDPowFit = {tag, dP_alpha1_bkg, dP_alpha2_bkg, dP_Nbkg, npoints}
		bkgDPowFit = SidebandDPowFit(dataSB, mzd, *ws);
		tag_bkg = bkgDPowFit.tag;
		Nbkg = bkgDPowFit.Nbkg;
	}

	else if (bkg_fit == "bernPoly2") {
		// bkgBernPoly2Fit = {tag, poly2_pC_bkg, poly2_p0_bkg, poly2_p1_bkg, poly2_Nbkg, npoints}
		bkgBernPoly2Fit = SidebandBernPoly2Fit(dataSB, mzd, *ws);
		tag_bkg = bkgBernPoly2Fit.tag;
		Nbkg = bkgBernPoly2Fit.Nbkg;
	}

	else if (bkg_fit == "chebPoly2") {
		// bkgChebPoly2Fit = {tag, poly2_pC_bkg, poly2_p0_bkg, poly2_Nbkg, npoints}
		bkgChebPoly2Fit = SidebandChebPoly2Fit(dataSB, mzd, *ws);
		tag_bkg = bkgChebPoly2Fit.tag;
		Nbkg = bkgChebPoly2Fit.Nbkg;
	}

	else if (bkg_fit == "bernPoly3") {
		// bkgBernPoly3Fit = {tag, poly3_pC_bkg, poly3_p0_bkg, poly3_p1_bkg, poly3_p2_bkg, poly3_Nbkg, npoints}
//		bkgBernPoly3Fit = SidebandBernPoly3Fit(&data, mzd, *ws);
		bkgBernPoly3Fit = SidebandBernPoly3Fit(dataSB, mzd, *ws);
		tag_bkg = bkgBernPoly3Fit.tag;
		Nbkg = bkgBernPoly3Fit.Nbkg;
	}

	
	// Calculate Background Normalization
	std::ostringstream ss1, ss2, ss3, ss4;
	ss1 << sideband_min;
	ss2 << sideband_max;
	ss3 << window_min;
	ss4 << window_max;
	std::string s_sideband_min = ss1.str();	
	std::string s_sideband_max = ss2.str();	
	std::string s_window_min = ss3.str();	
	std::string s_window_max = ss4.str();	
	
	std::string sidebandl = "mass>" + s_sideband_min + " && mass<" + s_window_min;
	std::string sidebandr = "mass>" + s_window_max + " && mass<" + s_sideband_max;

	double N_sidebandl = data.sumEntries(sidebandl.c_str());
	double N_sidebandr = data.sumEntries(sidebandr.c_str());
	double N_sideband = dataSB->numEntries();
	double npoints = data.numEntries();

	RooAbsReal* N_signalRegion = ws->pdf(tag_bkg)->createIntegral(mzd, RooFit::NormSet(mzd), RooFit::Range("signal"));
	RooAbsReal* N_sidebandRegion = ws->pdf(tag_bkg)->createIntegral(mzd, RooFit::NormSet(mzd),RooFit::Range("low,high"));
	RooAbsReal* N_sidebandlow = ws->pdf(tag_bkg)->createIntegral(mzd, RooFit::NormSet(mzd),RooFit::Range("low"));
	RooAbsReal* N_sidebandhigh = ws->pdf(tag_bkg)->createIntegral(mzd, RooFit::NormSet(mzd),RooFit::Range("high"));
	double scale_bkg = 1. / (N_sidebandlow->getVal() + N_sidebandhigh->getVal());	
//	double bkgYield = 36.812/0.333 * Nbkg * scale_bkg;
//	double bkgYield = Nbkg * scale_bkg;
	double bkgYield = 36.812/0.333 * Nbkg;
//	double bkgYield = Nbkg;
	
	std::cout << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << "[INFO]:" << std::endl;
	std::cout << "npoints: " << npoints << std::endl;
	std::cout << "Sideband: " << N_sideband << std::endl;
	std::cout << "Left Sideband: " << N_sidebandl << std::endl;
	std::cout << "Right Sideband: " << N_sidebandr << std::endl;
	std::cout << "Fitted npoints (Fitted Nbkg): " << Nbkg << std::endl;
	std::cout << "N_signalRegion: " << N_signalRegion->getVal() << std::endl;
	std::cout << "N_sidebandRegion: " << N_sidebandRegion->getVal() << std::endl;
	std::cout << "N_sidebandlow + N_sidebandhigh: " << N_sidebandlow->getVal() + N_sidebandhigh->getVal() << std::endl;
	std::cout << "Scale: " << scale_bkg << std::endl;
	std::cout << "Background Yield: " << bkgYield << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << std::endl;

	// Generate Toy Data
	RooDataHist* data_toys = ws->pdf(tag_bkg)->generateBinned(mzd, bkgYield, RooFit::ExpectedData());	
//	RooDataHist* data_toys = ws->pdf(tag_bkg)->generateBinned(mzd, Nbkg, RooFit::ExpectedData());	
//	RooDataHist* data_toys = ws->pdf(tag_bkg)->generateBinned(mzd, npoints, RooFit::ExpectedData());	
	data_toys->SetName("data_bin"+binNumber);
	data.SetName("data_bin"+binNumber);

	//-------------------------------------------
	// Signal Fit
	// ------------------------------------------

	// Do Fitting
	bwFit_output sigBWFit;
	TString tag_signal;

	if (sig_fit == "bw") {
		// sigBWFit = {tag, BW_mean_s, BW_width_s, BW_Ns, npoints}
		sigBWFit = BreitWignerFit(mass, lifetime, &dataSignal, mzd, *ws);
		tag_signal = sigBWFit.tag;
	}

	// Calculate Signal Normalization
/*
	// Constants
        double hbar = 6.582119514 * std::pow(10., -16.); // eV s
        double c = 299792458. * 1000.; // mm/s
        double alpha_em = 0.0072973525664; // unitless 
        double mu_mass = 105.658 * std::pow(10., 6.); // eV
        double pi = 3.1415927;

	double N_l = 2.;
	double R_mu = 3.4; // Approx for 20 GeV
	double mass_res = 0.5 * std::pow(10., 9.); // eV; for 20 GeV
	double mass_eV = mass * std::pow(10., 9.); // convert mass to eV

	double BEM = 5463;
	double obs_sig_events = 6910;
	double cut_sig_events = dataSigWindow->numEntries();
	double mass_ratio = mu_mass / mass_eV;
	double mass_ratio2 = std::pow(mass_ratio, 2);
	double eps2 = 1./(lifetime * mass_eV) * (3.*hbar*c)/alpha_em * std::pow((1.+2.*mass_ratio2), -1.) * std::pow((1.-4.*mass_ratio2), -0.5);
	double normalize = BEM * 3.* pi/8. * (mass_eV/mass_res) * (eps2/(alpha_em*(N_l+R_mu)));
	double scale_sig = normalize / obs_sig_events;
//	double signalYield = cut_sig_events * scale_sig; 
*/
	double signalYield, sigYieldNoCut, N_sigNoCut, N_sigCut, eff;

    //=============================================================================================
    // Configure N_sigNoCut
    // ============================================================================================
    // tau0 = 10e-10mm, N_sigNoCut = 22316
    // tau0 = 1mm, N_sigNoCut = 20149
    // tau0 = 10mm, N_sigNoCut = 14606
    // tau0 = 50mm, N_sigNoCut = 9122
    // tau0 = 100mm, N_sigNoCut = 7654 
    // tau0 = 1000mm, N_sigNoCut = 1498 

	if (vtxCut != "0") {
		sigYieldNoCut = 1096.67;
//		sigYieldNoCut = 274.168;
//                sigYieldNoCut = 276.627;
		N_sigNoCut = 1498;
		N_sigCut = dataSignal.numEntries();
		eff = N_sigCut / N_sigNoCut;
		signalYield = eff * sigYieldNoCut;
	}

	else {
		signalYield = std::sqrt(bkgYield); 
	}

	std::cout << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << "[INFO]:" << std::endl;
//	std::cout << "mass ratio: " << mass_ratio << std::endl;
//	std::cout << "eps2: " << eps2 << std::endl;
//	std::cout << "BEM: " << bkgYield << std::endl;
//	std::cout << "Signal Window: " << cut_sig_events << std::endl;
//	std::cout << "Normalize: " << normalize << std::endl;
//	std::cout << "Scale: " << scale_sig << std::endl;
//	std::cout << std::endl;

	if (vtxCut != "0") {
		std::cout << "Full Signal No Cut: " << N_sigNoCut << std::endl;
		std::cout << "Full Signal After Cut: " << N_sigCut << std::endl;
		std::cout << "Cut Efficiency: " << eff << std::endl;
	}

	std::cout << "Signal Yield: " << signalYield << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << std::endl;
	
	//-------------------------------------------
	// Plotting
	// ------------------------------------------
	MakePlot(mzd, *ws, binNumber, vtxCut.c_str(), tag_signal, tag_bkg, bkgYield, &dataSignal, &data, data_toys);

	ws->import(mzd);
	ws->Write("w_sb");

	//-------------------------------------------
	// Preparation to Combine Input
	// ------------------------------------------
	RooWorkspace *combine_ws = new RooWorkspace("combine_ws", "");
	
	//-------------------------------------------
	// Signal line shape
	//-------------------------------------------
	TString combineSignal;

	std::cout << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << "[INFO]:" << std::endl;
	std::cout << "[COMBINE INPUT SIGNAL]:" << std::endl;

	if (sig_fit == "bw") {
		combineSignal = MakeBreitWigner(true, "signal_bin" + binNumber, mass, lifetime, mzd, *combine_ws);
		double BW_mean_s = sigBWFit.mean;
		double BW_width_s = sigBWFit.width;

		combine_ws->var(combineSignal + "_meanBW")->setVal(BW_mean_s);
		combine_ws->var(combineSignal + "_widthBW")->setVal(BW_width_s);

		combine_ws->var(combineSignal + "_meanBW")->setConstant(kTRUE);
		combine_ws->var(combineSignal + "_widthBW")->setConstant(kTRUE);

		std::cout << "BW mean: " << BW_mean_s << std::endl;
		std::cout << "BW width: " << BW_width_s << std::endl;
	}


	RooRealVar *signal_norm = new RooRealVar(combineSignal + "_norm", "", signalYield);
	
	std::cout << "Signal Yield: " << signal_norm->getVal() << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << std::endl;

	combine_ws->import(*signal_norm);

	//-------------------------------------------
	// Background line shape
	//-------------------------------------------
	TString combineBkg;

	std::cout << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << "[INFO]:" << std::endl;
	std::cout << "[COMBINE INPUT BACKGROUND]:" << std::endl;

	if (bkg_fit == "expo") {
		combineBkg = MakeExpo(true, "bkg_bin" + binNumber, mzd, *combine_ws);
		double sE_lambda_bkg = bkgExpoFit.lambda;

		combine_ws->var(combineBkg + "_lambdaExpo")->setVal(sE_lambda_bkg);
		combine_ws->var(combineBkg + "_lambdaExpo")->setConstant(kTRUE);

		std::cout << "EXPO lambda: " << sE_lambda_bkg << std::endl;
	}
	
	else if (bkg_fit == "doubleExpo") {
		combineBkg = MakeDoubleExpo(true, "bkg_bin" + binNumber, mzd, *combine_ws);

		double dE_lambda1_bkg = bkgDExpoFit.lambda1;
		double dE_lambda2_bkg = bkgDExpoFit.lambda2;

		combine_ws->var(combineBkg + "_lambdaExpo1")->setVal(dE_lambda1_bkg);
		combine_ws->var(combineBkg + "_lambdaExpo2")->setVal(dE_lambda2_bkg);

		combine_ws->var(combineBkg + "_lambdaExpo1")->setConstant(kTRUE);
		combine_ws->var(combineBkg + "_lambdaExpo2")->setConstant(kTRUE);

		std::cout << "DEXPO lambda1: " << dE_lambda1_bkg << std::endl;
		std::cout << "DEXPO lambda2: " << dE_lambda2_bkg << std::endl;
	}
		
	else if (bkg_fit == "doubleExpoN1N2") {
		combineBkg = MakeDoubleExpoN1N2("bkg_bin" + binNumber, mzd, *combine_ws);

		double dE_lambda1_bkg = bkgDExpoN1N2Fit.lambda1;
		double dE_lambda2_bkg = bkgDExpoN1N2Fit.lambda2;

		combine_ws->var(combineBkg + "_lambdaExpo1")->setVal(dE_lambda1_bkg);
		combine_ws->var(combineBkg + "_lambdaExpo2")->setVal(dE_lambda2_bkg);

		combine_ws->var(combineBkg + "_lambdaExpo1")->setConstant(kTRUE);
		combine_ws->var(combineBkg + "_lambdaExpo2")->setConstant(kTRUE);

		std::cout << "DEXPON1N2 lambda1: " << dE_lambda1_bkg << std::endl;
		std::cout << "DEXPON1N2 lambda2: " << dE_lambda2_bkg << std::endl;
	}
		
	else if (bkg_fit == "pow") {
		combineBkg = MakeSinglePow("bkg_bin" + binNumber, mzd, *combine_ws);

		double sP_alpha_bkg = bkgPowFit.alpha;

		combine_ws->var(combineBkg + "_alpha")->setVal(sP_alpha_bkg);

		combine_ws->var(combineBkg + "_alpha")->setConstant(kTRUE);

		std::cout << "POW alpha: " << sP_alpha_bkg << std::endl;
	}
		
	else if (bkg_fit == "doublePow") {
		combineBkg = MakeDoublePow("bkg_bin" + binNumber, mzd, *combine_ws);

		double dP_alpha1_bkg = bkgDPowFit.alpha1;
		double dP_alpha2_bkg = bkgDPowFit.alpha2;

		combine_ws->var(combineBkg + "_alpha1")->setVal(dP_alpha1_bkg);
		combine_ws->var(combineBkg + "_alpha2")->setVal(dP_alpha2_bkg);

		combine_ws->var(combineBkg + "_alpha1")->setConstant(kTRUE);
		combine_ws->var(combineBkg + "_alpha2")->setConstant(kTRUE);

		std::cout << "DPOW alpha1: " << dP_alpha1_bkg << std::endl;
		std::cout << "DPOW alpha2: " << dP_alpha2_bkg << std::endl;
	}
		
	else if (bkg_fit == "bernPoly2") {
		combineBkg = MakeBernPoly2(true, "bkg_bin" + binNumber, mzd, *combine_ws);

		double poly2_pC_bkg = bkgBernPoly2Fit.pC;
		double poly2_p0_bkg = bkgBernPoly2Fit.p0;
		double poly2_p1_bkg = bkgBernPoly2Fit.p1;

		combine_ws->var(combineBkg + "_pC")->setVal(poly2_pC_bkg);
		combine_ws->var(combineBkg + "_p0")->setVal(poly2_p0_bkg);
		combine_ws->var(combineBkg + "_p1")->setVal(poly2_p1_bkg);

		combine_ws->var(combineBkg + "_pC")->setConstant(kTRUE);
		combine_ws->var(combineBkg + "_p0")->setConstant(kTRUE);
		combine_ws->var(combineBkg + "_p1")->setConstant(kTRUE);

		std::cout << "BERNPOLY2 p0: " << poly2_p0_bkg << std::endl;
		std::cout << "BERNPOLY2 p1: " << poly2_p1_bkg << std::endl;
		std::cout << "BERNPOLY2 pC: " << poly2_pC_bkg << std::endl;
	}
		
	else if (bkg_fit == "chebPoly2") {
		combineBkg = MakeChebychevPoly2("bkg_bin" + binNumber, mzd, *combine_ws);

		double chebPoly2_pC_bkg = bkgChebPoly2Fit.pC;
		double chebPoly2_p0_bkg = bkgChebPoly2Fit.p0;

		combine_ws->var(combineBkg + "_pC")->setVal(chebPoly2_pC_bkg);
		combine_ws->var(combineBkg + "_p0")->setVal(chebPoly2_p0_bkg);

		combine_ws->var(combineBkg + "_pC")->setConstant(kTRUE);
		combine_ws->var(combineBkg + "_p0")->setConstant(kTRUE);

		std::cout << "CHEBPOLY2 p0: " << chebPoly2_p0_bkg << std::endl;
		std::cout << "CHEBPOLY2 pC: " << chebPoly2_pC_bkg << std::endl;
	}
		
	else if (bkg_fit == "bernPoly3") {
		combineBkg = MakeBernPoly3(true, "bkg_bin" + binNumber, mzd, *combine_ws);

		double poly3_pC_bkg = bkgBernPoly3Fit.pC;
		double poly3_p0_bkg = bkgBernPoly3Fit.p0;
		double poly3_p1_bkg = bkgBernPoly3Fit.p1;
		double poly3_p2_bkg = bkgBernPoly3Fit.p2;

		combine_ws->var(combineBkg + "_pC")->setVal(poly3_pC_bkg);
		combine_ws->var(combineBkg + "_p0")->setVal(poly3_p0_bkg);
		combine_ws->var(combineBkg + "_p1")->setVal(poly3_p1_bkg);
		combine_ws->var(combineBkg + "_p2")->setVal(poly3_p2_bkg);

		combine_ws->var(combineBkg + "_pC")->setConstant(kTRUE);
		combine_ws->var(combineBkg + "_p0")->setConstant(kTRUE);
		combine_ws->var(combineBkg + "_p1")->setConstant(kTRUE);
		combine_ws->var(combineBkg + "_p2")->setConstant(kTRUE);

		std::cout << "BERNPOLY3 p0: " << poly3_p0_bkg << std::endl;
		std::cout << "BERNPOLY3 p1: " << poly3_p1_bkg << std::endl;
		std::cout << "BERNPOLY3 p2: " << poly3_p2_bkg << std::endl;
		std::cout << "BERNPOLY3 pC: " << poly3_pC_bkg << std::endl;
	}
		
	RooRealVar *bkg_norm = new RooRealVar(combineBkg + "_norm", "", bkgYield);
	std::cout << "Background Yield: " << bkg_norm->getVal() << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << std::endl;
	
        combine_ws->import(*bkg_norm);
	
	//-------------------------------------------
	// Import Dataset
	//-------------------------------------------
	//combine_ws->import(*data_toys);
	combine_ws->import(data); //import real data
	
	combine_ws->Write("combineWS");
	file->cd();
	file->Close();

	// Write Datacard
	std::cout << "[INFO]: Creating combine datacard" << std::endl;
	std::ofstream ofs(datacardName, std::ofstream::out);

	ofs << "imax 1\n"; // number of bins
	ofs << "jmax 1\n"; // number of processes
	ofs << "kmax *\n"; // number of nuisance parameters
	ofs << "----------------------------------------------------------------------------------------\n";
	ofs << "shapes bkg\t\tbin"      << binNumber << "\t" << combineRootFileName << " combineWS:" << combineBkg << "\n";
	ofs << "shapes signal\t\tbin"   << binNumber << "\t" << combineRootFileName << " combineWS:" << combineSignal << "\n";
	ofs << "shapes data_obs\t\tbin" << binNumber << "\t" << combineRootFileName << " combineWS:" << "data_bin" << binNumber << "\n";
	ofs << "----------------------------------------------------------------------------------------\n";
	ofs << "bin\t\tbin" << binNumber << "\n";
	ofs << "observation\t-1.0\n";
	ofs << "----------------------------------------------------------------------------------------\n";
	ofs << "bin\t\t\t\t\t\tbin" << binNumber << "\t\tbin" << binNumber << "\n";
	ofs << "process\t\t\t\t\t\tsignal\t\tbkg\n";
	ofs << "process\t\t\t\t\t\t0\t\t1\n";
	ofs << "rate\t\t\t\t\t\t1\t\t1\n";
	ofs << "----------------------------------------------------------------------------------------\n";
//	ofs << "CMS_Lumi\t\t\tlnN\t\t1.026\t\t1.026\t\t-\n";	
	ofs.close();
	return ws;
}
	
