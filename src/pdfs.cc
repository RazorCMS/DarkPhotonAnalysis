// ROOT INCLUDES
#include <TString.h>

// ROOFIT INCLUDES 
#include <RooWorkspace.h>
#include <RooExponential.h>
#include <RooBreitWigner.h>
#include <RooGenericPdf.h>
#include <RooBernstein.h>
#include <RooChebychev.h>
#include <RooAddPdf.h>
#include <RooFormulaVar.h>
#include <RooRealVar.h>
#include <RooArgList.h>

// LOCAL INCLUDES
#include "pdfs.h"

TString MakeBreitWigner(TString tag, double mass, double lifetime, RooRealVar &mzd, RooWorkspace &w) {
        double decay_width = 1./lifetime;
        RooRealVar *mean = new RooRealVar(tag+"_meanBW", "mean", mass, "");
        RooRealVar *width = new RooRealVar(tag+"_widthBW", "width", 1., "");

        RooRealVar *Ns = new RooRealVar(tag+"_Ns", "N_{s}", 1, "events");

        RooBreitWigner *bw = new RooBreitWigner(tag+"_NE", "bw", mzd, *mean, *width);
        RooAddPdf *ext_bw = new RooAddPdf(tag, "ext_bw", RooArgList(*bw), RooArgList(*Ns));

        Ns->setConstant(kFALSE);
        mean->setConstant(kFALSE);
        width->setConstant(kFALSE);

        w.import(*ext_bw);

        return tag;
};

TString MakeExpo(TString tag, RooRealVar &mzd, RooWorkspace &w) {
        RooRealVar *lambda = new RooRealVar(tag+"_lambdaExpo", "#lambda", -0.1, "");

        RooRealVar *Nbkg = new RooRealVar(tag+"_Nbkg", "N_{bkg}", 1, "events");

        RooExponential *expo = new RooExponential(tag+"_NE", "expo", mzd, *lambda);
        RooAddPdf *ext_expo = new RooAddPdf(tag, "ext_expo", RooArgList(*expo), RooArgList(*Nbkg));

        Nbkg->setConstant(kFALSE);
        lambda->setConstant(kFALSE);

        w.import(*ext_expo);

        return tag;
};

TString MakeDoubleExpo(TString tag, RooRealVar &mzd, RooWorkspace &w) {
	RooRealVar* lambda1 = new RooRealVar(tag + "_lambdaExpo1", "#lambda_{1}", 0.15, -0.5, 0.5,  "");
	RooRealVar* lambda2 = new RooRealVar(tag + "_lambdaExpo2", "#lambda_{2}", 0.1, "");


	 //--------------------------------------------
	 // Square variables to avoid rising exponential
	 //--------------------------------------------
  	RooFormulaVar* lambda_sq1 = new RooFormulaVar(tag + "_lambda1Sq", "#lambda^{2}_{1}", "-1*@0*@0", *lambda1);
	RooFormulaVar* lambda_sq2 = new RooFormulaVar(tag + "_lambda2Sq", "#lambda^{2}_{2}", "-1*@0*@0", *lambda2);

	RooRealVar *frac    = new RooRealVar(tag+"_frac", "frac", 0.2, 0., 1.0, "");
	RooRealVar *Nbkg = new RooRealVar(tag+"_Nbkg", "N_{bkg}", 1, "events");

	RooExponential *expo1    = new RooExponential(tag+"_expo1", "", mzd, *lambda_sq1);
	RooExponential *expo2    = new RooExponential(tag+"_expo2", "", mzd, *lambda_sq2);
//	RooExponential *expo1    = new RooExponential(tag+"_expo1", "", mzd, *lambda1);
//	RooExponential *expo2    = new RooExponential(tag+"_expo2", "", mzd, *lambda2);
	RooAddPdf* doubleExpo = new RooAddPdf(tag + "_NE", "", RooArgList(*expo1, *expo2), *frac);
	RooAddPdf* ext_doubleExpo = new RooAddPdf(tag, "ext_dexpo", RooArgList(*doubleExpo), RooArgList(*Nbkg));

	Nbkg->setConstant(kFALSE);
	frac->setConstant(kFALSE);
	lambda1->setConstant(kFALSE);
	lambda2->setConstant(kFALSE);

	w.import(*ext_doubleExpo);

	return tag;
}

TString MakeDoubleExpoN1N2(TString tag, RooRealVar &mzd, RooWorkspace &w) {
	RooRealVar* lambda1 = new RooRealVar(tag + "_lambdaExpo1", "#lambda_{1}", 0.25, "");
	RooRealVar* lambda2 = new RooRealVar(tag + "_lambdaExpo2", "#lambda_{2}", 0.25, "");

	RooRealVar *Nbkg1 = new RooRealVar(tag+"_Nbkg1", "N_{bkg, 1}", std::sqrt(0.5), "events");
	RooRealVar *Nbkg2 = new RooRealVar(tag+"_Nbkg2", "N_{bkg, 2}", std::sqrt(0.5), "events");

	 //--------------------------------------------
	 // Square variables to avoid rising exponential
	 //--------------------------------------------
  	RooFormulaVar* lambda1sq = new RooFormulaVar(tag + "_lambda1Sq", "#lambda^{2}_{1}", "-1*@0*@0", *lambda1);
	RooFormulaVar* lambda2sq = new RooFormulaVar(tag + "_lambda2Sq", "#lambda^{2}_{2}", "-1*@0*@0", *lambda2);

  	RooFormulaVar* Nbkg1sq = new RooFormulaVar(tag + "_Nbkg1Sq", "N^{2}_{bkg, 1}", "@0*@0", *Nbkg1);
	RooFormulaVar* Nbkg2sq = new RooFormulaVar(tag + "_Nbkg2Sq", "#N^{2}_{bkg, 2}", "@0*@0", *Nbkg2);

	RooExponential *expo1    = new RooExponential(tag+"_expo1", "", mzd, *lambda1sq);
	RooExponential *expo2    = new RooExponential(tag+"_expo2", "", mzd, *lambda2sq);
	RooAddPdf* ext_doubleExpoN1N2 = new RooAddPdf(tag, "ext_dexpoN1N2", RooArgList(*expo1, *expo2), RooArgList(*Nbkg1sq, *Nbkg2sq));

	Nbkg1->setConstant(kFALSE);
	Nbkg2->setConstant(kFALSE);
	lambda1->setConstant(kFALSE);
	lambda2->setConstant(kFALSE);

	w.import(*ext_doubleExpoN1N2);

	return tag;
}

TString MakeSinglePow(TString tag, RooRealVar &mzd, RooWorkspace &w) {
	RooRealVar *alpha  = new RooRealVar(tag+"_alpha","#alpha", -1);

	RooRealVar *Nbkg = new RooRealVar(tag+"_Nbkg", "N_{bkg}", 1, "events");

	RooGenericPdf *pow = new RooGenericPdf(tag+"_NE", "", "@0^@1", RooArgList(mzd,*alpha));
	RooAddPdf *ext_pow = new RooAddPdf(tag, "ext_pow", RooArgList(*pow), RooArgList(*Nbkg));

	Nbkg->setConstant(kFALSE);
	alpha->setConstant(kFALSE);

	w.import(*ext_pow);

	return tag;
};

TString MakeDoublePow(TString tag, RooRealVar &mzd, RooWorkspace &w) {
	RooRealVar *alpha1  = new RooRealVar(tag+"_alpha1", "#alpha_{1}", -1.0,"");
	RooRealVar *alpha2  = new RooRealVar(tag+"_alpha2", "#alpha_{2}", -2.0,"");
	RooRealVar *f       = new RooRealVar(tag+"_f", "f", 0.5, 0.0, 1.0);
	RooRealVar *Nbkg    = new RooRealVar(tag+"_Nbkg","N_{bkg}", 1, "events");

	RooGenericPdf *pow1 = new RooGenericPdf(tag+"_pow1", "", "@0^@1", RooArgList(mzd, *alpha1));
	RooGenericPdf *pow2 = new RooGenericPdf(tag+"_pow2", "", "@0^@1", RooArgList(mzd, *alpha2));
	RooAddPdf *doublePow = new RooAddPdf(tag+"_NE", "", *pow1, *pow2, *f);  
	RooAddPdf *ext_doublePow = new RooAddPdf(tag, "ext_dpow", RooArgList(*doublePow), RooArgList(*Nbkg));

	Nbkg->setConstant(kFALSE);
	alpha1->setConstant(kFALSE);
	alpha2->setConstant(kFALSE);
	alpha1->setMax(0.0);
	alpha2->setMax(0.0);

	w.import(*ext_doublePow);

	return tag;
};

TString MakeBernPoly2(TString tag, RooRealVar& mzd, RooWorkspace &w) {
	RooRealVar *pC = new RooRealVar(tag+"_pC","C", 0.6, -10.,10,""); 
	RooRealVar *p0 = new RooRealVar(tag+"_p0","p_0", -0.2, -10.,10.,"");
	RooRealVar *p1 = new RooRealVar(tag+"_p1","p_1", 0.3, -10.,10.,"");

	RooRealVar *Nbkg   = new RooRealVar(tag+"_Nbkg","N_{bkg}",1,"events");

	RooFormulaVar *pCmod = new RooFormulaVar(tag+"_pCmod","@0*@0",*pC);
	RooFormulaVar *p0mod = new RooFormulaVar(tag+"_p0mod","@0*@0",*p0);
	RooFormulaVar *p1mod = new RooFormulaVar(tag+"_p1mod","@0*@0",*p1);

	RooBernstein *bern = new RooBernstein(tag+"_poly2", "", mzd, RooArgList(*pCmod, *p0mod, *p1mod));
//	RooBernstein *bern = new RooBernstein(tag+"_poly2", "", mzd, RooArgList(*pC, *p0, *p1));
	RooAddPdf *ext_bern = new RooAddPdf(tag, "ext_poly2", RooArgList(*bern), RooArgList(*Nbkg));

	Nbkg->setConstant(kFALSE);
	pC->setConstant(kFALSE);
	p0->setConstant(kFALSE);
	p1->setConstant(kFALSE);

	w.import(*ext_bern);

	return tag;
};

TString MakeChebychevPoly2(TString tag, RooRealVar& mzd, RooWorkspace &w) {
	RooRealVar *pC = new RooRealVar(tag+"_pC","C",0.5,""); 
	RooRealVar *p0 = new RooRealVar(tag+"_p0","p_0",0.2,"");

	RooRealVar *Nbkg   = new RooRealVar(tag+"_Nbkg","N_{bkg}",1,"events");
	RooChebychev *cheb = new RooChebychev(tag+"_chebpoly2", "", mzd, RooArgList(*pC, *p0));
	RooAddPdf *ext_cheb = new RooAddPdf(tag, "ext_chebpoly2", RooArgList(*cheb), RooArgList(*Nbkg));

	Nbkg->setConstant(kFALSE);
	pC->setConstant(kFALSE);
	p0->setConstant(kFALSE);

	w.import(*ext_cheb);

	return tag;
};

TString MakeBernPoly3(TString tag, RooRealVar &mzd, RooWorkspace &w) {
	RooRealVar *pC = new RooRealVar(tag+"_pC","C",1.2,"");
	RooRealVar *p0 = new RooRealVar(tag+"_p0","p_0",0.8,"");
	RooRealVar *p1 = new RooRealVar(tag+"_p1","p_1",0.9, "");
	RooRealVar *p2 = new RooRealVar(tag+"_p2","p_2",0.8, "");

	RooRealVar *Nbkg   = new RooRealVar(tag+"_Nbkg","N_{bkg}",1,"events");

	RooFormulaVar *pCmod = new RooFormulaVar(tag+"_pCmod","@0*@0",*pC);
	RooFormulaVar *p0mod = new RooFormulaVar(tag+"_p0mod","@0*@0",*p0);
	RooFormulaVar *p1mod = new RooFormulaVar(tag+"_p1mod","@0*@0",*p1);
	RooFormulaVar *p2mod = new RooFormulaVar(tag+"_p2mod","@0*@0",*p2);

	RooBernstein *bern = new RooBernstein(tag+"_poly3","", mzd, RooArgList(*pCmod,*p0mod,*p1mod,*p2mod));
	RooAddPdf *ext_bern = new RooAddPdf(tag,"ext_poly3", RooArgList(*bern), RooArgList(*Nbkg));

	Nbkg->setConstant(kFALSE);
	pC->setConstant(kFALSE);
	p0->setConstant(kFALSE);
	p1->setConstant(kFALSE);
	p2->setConstant(kFALSE);

	w.import(*ext_bern);

	return tag;
};
