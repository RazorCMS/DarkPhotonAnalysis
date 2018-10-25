#ifndef fitDarkphoton_v3_H
#define fitDarkphoton_v3_H 1

//C++ INCLUDES

//ROOT INCLUDES
//#include <TSYSTEM.h>
#include <TTree.h>
#include <TString.h>
#include <TROOT.h>

//ROOFIT INCLUDES
#include <RooWorkspace.h>
#include <RooRealVar.h>

// LOCAL INCLUDES
#include "pdfs.h"

struct bwFit_output {
        TString tag;
        double mean;
        double width;
        double Ns;
        int npoints;
};

struct expoFit_output {
        TString tag;
        double lambda;
        double Nbkg;
        int npoints;
};

struct doubleExpoFit_output {
        TString tag;
        double lambda1;
        double lambda2;
        double Nbkg;
        int npoints;
};

struct doubleExpoN1N2Fit_output {
        TString tag;
        double lambda1;
        double lambda2;
        double Nbkg1;
        double Nbkg2;
        int npoints;
};

struct powFit_output {
        TString tag;
        double alpha;
        double Nbkg;
        int npoints;
};

struct doublePowFit_output {
        TString tag;
        double alpha1;
        double alpha2;
        double Nbkg;
        int npoints;
};

struct bernPoly2Fit_output {
        TString tag;
        double pC;
        double p0;
        double p1;
	double Nbkg;
        int npoints;
};

struct chebPoly2Fit_output {
        TString tag;
        double pC;
        double p0;
	double Nbkg;
        int npoints;
};

struct bernPoly3Fit_output {
        TString tag;
        double pC;
        double p0;
        double p1;
        double p2;
	double Nbkg;
        int npoints;
};

void SplusB_fit(TTree* tree, std::string sig_fit, std::string bkg_fit);

bwFit_output BreitWignerFit(double mass, double lifetime, RooDataSet *data, RooRealVar &mzd, RooWorkspace &w);

expoFit_output SidebandExpoFit(RooDataSet *data, RooRealVar &mzd, RooWorkspace &w);

doubleExpoFit_output SidebandDExpoFit(RooDataSet *data, RooRealVar &mzd, RooWorkspace &w);

doubleExpoN1N2Fit_output SidebandDExpoN1N2Fit(RooDataSet *data, RooRealVar &mzd, RooWorkspace &w);

powFit_output SidebandPowFit(RooDataSet *data, RooRealVar &mzd, RooWorkspace &w);

doublePowFit_output SidebandDPowFit(RooDataSet *data, RooRealVar &mzd, RooWorkspace &w);

bernPoly2Fit_output SidebandBernPoly2Fit(RooDataSet *data, RooRealVar &mzd, RooWorkspace &w);

chebPoly2Fit_output SidebandChebPoly2Fit(RooDataSet *data, RooRealVar &mzd, RooWorkspace &w);

bernPoly3Fit_output SidebandBernPoly3Fit(RooDataSet *data, RooRealVar &mzd, RooWorkspace &w);

void MakePlotTree(TTree *tree, TString tag, TString vtxCut, double sideband_min, double sideband_max);

void MakePlot(RooRealVar &mzd, RooWorkspace &w, TString binNumber, TString vtxCut, TString tag_sig, TString tag_bkg, double bkgYield, RooDataSet *data_sig, RooDataSet *data_bkg, RooDataSet *data_toy);

RooWorkspace *MakeDataCard(std::string sig_fit, std::string bkg_fit, TTree* treeSignal, TTree* treeData, std::string vtxCut, double mass, double lifetime, TString binNumber, TString combineRootFileName, TString datacardName);

#endif
