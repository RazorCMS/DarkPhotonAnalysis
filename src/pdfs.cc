// ROOT INCLUDES
#include <TString.h>

// ROOFIT INCLUDES
#include <RooWorkspace.h>
#include <RooExponential.h>
#include <RooBreitWigner.h>
#include <RooCBShape.h>
#include <RooGenericPdf.h>
#include <RooBernstein.h>
#include <RooChebychev.h>
#include <RooAddPdf.h>
#include <RooFormulaVar.h>
#include <RooRealVar.h>
#include <RooArgList.h>

// LOCAL INCLUDES
#include "pdfs.h"

/*
 * Wrappers for the PDFs.
 *
 * Args
 *      extended: boolean to specify whether the PDFs are extended
 *      tag: tag identify PDF in the RooWorkspace
 *      mzd: RooRealVar of the mass
 *      w: the RooWorkspace where the PDFs are saved
 *
 * Returns
 *      the tag of the PDF.
 */

/*
 * The functions ending with "_NE" are redundant. They are the non-extended
 * versions of the respective PDFs. I kept them here just in case.
 */


TString MakeCB(bool extended, TString tag, double mass, RooRealVar &mzd, RooWorkspace &w) {
    RooRealVar *mu = new RooRealVar(tag+"_CB_mu", "#mu_{CB}", mass, "");
    RooRealVar *sigma = new RooRealVar(tag+"_CB_sigma", "#sigma_{CB}", 0.01, "");
    RooRealVar *alpha = new RooRealVar(tag+"_CB_alpha", "#alpha", 1.5, "");
    RooRealVar *n = new RooRealVar(tag+"_CB_n", "#n", 1.0, "");

    RooRealVar *Ns = new RooRealVar(tag+"_Ns", "N_{s}", 1, "events");

    RooCBShape *cb = new RooCBShape(tag+"_NE", "CB", mzd, *mu, *sigma, *alpha, *n);

    RooAddPdf *ext_cb = new RooAddPdf(tag, "ext_cb", RooArgList(*cb), RooArgList(*Ns));

    Ns->setConstant(kFALSE);
    mu->setConstant(kFALSE);
    sigma->setConstant(kFALSE);
    alpha->setConstant(kFALSE);
    n->setConstant(kFALSE);
    n->setRange(0, 1000);

    w.import(*ext_cb);

    return tag;
}

TString MakeDoubleCB(bool extended, TString tag, double mass, RooRealVar &mzd, RooWorkspace &w) {
    RooRealVar *mu1 = new RooRealVar(tag+"_CB_mu1", "#mu_{1}", mass, "");
    RooRealVar *alpha1 = new RooRealVar(tag+"_CB_alpha1", "#alpha_{1}", 1.5, "");
    RooRealVar *sigma1 = new RooRealVar(tag+"_CB_sigma1", "#sigma_{1}", 1.1, "");
    RooRealVar *n1 = new RooRealVar(tag+"_CB_n1", "#n_{1}", 1.0, "");

    RooRealVar *mu2 = new RooRealVar(tag+"_CB_mu2", "#mu_{2}", mass, "");
    RooRealVar *alpha2 = new RooRealVar(tag+"_CB_alpha2", "#alpha_{2}", 1.5, "");
    RooRealVar *sigma2 = new RooRealVar(tag+"_CB_sigma2", "#sigma_{2}", 1.1, "");
    RooRealVar *n2 = new RooRealVar(tag+"_CB_n2", "#n_{2}", 1.0, "");

    RooRealVar *frac = new RooRealVar(tag+"_frac", "frac", 0.3, 0., 1., "");

    RooCBShape *cb1 = new RooCBShape(tag+"_cb1", "cb1", mzd, *mu1, *sigma1, *alpha1, *n1);
    RooCBShape *cb2 = new RooCBShape(tag+"_cb2", "cb2", mzd, *mu2, *sigma2, *alpha2, *n2);

    mu1->setConstant(kFALSE);
    alpha1->setConstant(kFALSE);
    sigma1->setConstant(kFALSE);
    mu2->setConstant(kFALSE);
    alpha2->setConstant(kFALSE);
    sigma2->setConstant(kFALSE);
    n1->setConstant(kFALSE);
    n1->setRange(0, 1000);
    n2->setConstant(kFALSE);
    n2->setRange(0, 1000);

    if (extended) {
        RooRealVar *Ns = new RooRealVar(tag+"_Ns", "N_{s}", 1, "events");
        Ns->setConstant(kFALSE);
        RooAddPdf *dcb = new RooAddPdf(tag+"_NE", "dcb", RooArgList(*cb1, *cb2), *frac);
        RooAddPdf *ext_dcb = new RooAddPdf(tag, "ext_dcb", RooArgList(*dcb), RooArgList(*Ns));
        w.import(*ext_dcb);
    }

    else {
        RooAddPdf *dcb = new RooAddPdf(tag, "dcb", RooArgList(*cb1, *cb2), *frac);
        w.import(*dcb);
    }

    return tag;
}

TString MakeDoubleCB_NE(TString tag, double mass, RooRealVar &mzd, RooWorkspace &w) {
    RooRealVar *mu1 = new RooRealVar(tag+"_CB_mu1", "#mu_{1}", mass, "");
    RooRealVar *alpha1 = new RooRealVar(tag+"_CB_alpha1", "#alpha_{1}", 1.5, "");
    RooRealVar *sigma1 = new RooRealVar(tag+"_CB_sigma1", "#sigma_{1}", 1.1, "");
    RooRealVar *n1 = new RooRealVar(tag+"_CB_n1", "#n_{1}", 1.0, "");

    RooRealVar *mu2 = new RooRealVar(tag+"_CB_mu2", "#mu_{2}", mass, "");
    RooRealVar *alpha2 = new RooRealVar(tag+"_CB_alpha2", "#alpha_{2}", 1.5, "");
    RooRealVar *sigma2 = new RooRealVar(tag+"_CB_sigma2", "#sigma_{2}", 1.1, "");
    RooRealVar *n2 = new RooRealVar(tag+"_CB_n2", "#n_{2}", 1.0, "");

    RooRealVar *frac = new RooRealVar(tag+"_frac", "frac", 0.3, 0., 1., "");

    RooCBShape *cb1 = new RooCBShape(tag+"_cb1", "cb1", mzd, *mu1, *sigma1, *alpha1, *n1);
    RooCBShape *cb2 = new RooCBShape(tag+"_cb2", "cb2", mzd, *mu2, *sigma2, *alpha2, *n2);
    RooAddPdf *dcb = new RooAddPdf(tag, "dcb", RooArgList(*cb1, *cb2), *frac);

    mu1->setConstant(kFALSE);
    alpha1->setConstant(kFALSE);
    sigma1->setConstant(kFALSE);
    mu2->setConstant(kFALSE);
    alpha2->setConstant(kFALSE);
    sigma2->setConstant(kFALSE);
    n1->setConstant(kFALSE);
    n1->setRange(0, 1000);
    n2->setConstant(kFALSE);
    n2->setRange(0, 1000);

    w.import(*dcb);

    return tag;
}

TString MakeBreitWigner(bool extended, TString tag, double mass, double lifetime, RooRealVar &mzd, RooWorkspace &w) {
        double decay_width = 1./lifetime;
        RooRealVar *mean = new RooRealVar(tag+"_meanBW", "mean", mass, "");
        RooRealVar *width = new RooRealVar(tag+"_widthBW", "width", 0.001, 0., 1.0, "");

        mean->setConstant(kFALSE);
        width->setConstant(kFALSE);

        if (extended) {
            RooRealVar *Ns = new RooRealVar(tag+"_Ns", "N_{s}", 1, "events");
            Ns->setConstant(kFALSE);
            RooBreitWigner *bw = new RooBreitWigner(tag+"_NE", "bw", mzd, *mean, *width);
            RooAddPdf *ext_bw = new RooAddPdf(tag, "ext_bw", RooArgList(*bw), RooArgList(*Ns));
            w.import(*ext_bw);
        }

        else {
            RooBreitWigner *bw = new RooBreitWigner(tag, "bw", mzd, *mean, *width);
            w.import(*bw);
        }

        return tag;
};

TString MakeBreitWigner_NE(TString tag, double mass, double lifetime, RooRealVar &mzd, RooWorkspace &w) {
        double decay_width = 1./lifetime;
        RooRealVar *mean = new RooRealVar(tag+"_meanBW", "mean", mass, "");
        RooRealVar *width = new RooRealVar(tag+"_widthBW", "width", 0.001, 0., 1.0, "");


        RooBreitWigner *bw = new RooBreitWigner(tag, "bw", mzd, *mean, *width);

        mean->setConstant(kFALSE);
        width->setConstant(kFALSE);

        w.import(*bw);

        return tag;
};

TString MakeExpo(bool extended, TString tag, RooRealVar &mzd, RooWorkspace &w) {
  RooRealVar *lambda = new RooRealVar(tag+"_lambdaExpo", "#lambda", -0.1, "");
  lambda->setConstant(kFALSE);
  if (extended)
  {
    RooRealVar *Nbkg = new RooRealVar(tag+"_Nbkg", "N_{bkg}", 1, "events");
    Nbkg->setConstant(kFALSE);
    RooExponential *expo = new RooExponential(tag+"_NE", "expo", mzd, *lambda);
    RooAddPdf *ext_expo = new RooAddPdf(tag, "ext_expo", RooArgList(*expo), RooArgList(*Nbkg));
    w.import(*ext_expo);
  }
  else
  {
    RooExponential *expo = new RooExponential(tag, "expo", mzd, *lambda);
    w.import(*expo);
  }
  return tag;
};

TString MakeExpo_NE(TString tag, RooRealVar &mzd, RooWorkspace &w) {
        RooRealVar *lambda = new RooRealVar(tag+"_lambdaExpo", "#lambda", -0.1, "");


        RooExponential *expo = new RooExponential(tag, "expo", mzd, *lambda);

        lambda->setConstant(kFALSE);

        w.import(*expo);

        return tag;
};

TString MakeDoubleExpo(bool extended, TString tag, RooRealVar &mzd, RooWorkspace &w) {
	RooRealVar* lambda1 = new RooRealVar(tag + "_lambdaExpo1", "#lambda_{1}", 0.15, "");
	RooRealVar* lambda2 = new RooRealVar(tag + "_lambdaExpo2", "#lambda_{2}", 0.1, "");


	 //--------------------------------------------
	 // Square variables to avoid rising exponential
	 //--------------------------------------------
  RooFormulaVar* lambda_sq1 = new RooFormulaVar(tag + "_lambda1Sq", "#lambda^{2}_{1}", "-1*@0*@0", *lambda1);
	RooFormulaVar* lambda_sq2 = new RooFormulaVar(tag + "_lambda2Sq", "#lambda^{2}_{2}", "-1*@0*@0", *lambda2);

	RooRealVar *frac    = new RooRealVar(tag+"_frac", "frac", 0.2, 0., 1.0, "");

	frac->setConstant(kFALSE);
	lambda1->setConstant(kFALSE);
	lambda2->setConstant(kFALSE);

        if (extended) {
	    RooRealVar *Nbkg = new RooRealVar(tag+"_Nbkg", "N_{bkg}", 1, "events");
	    Nbkg->setConstant(kFALSE);
            RooExponential *expo1    = new RooExponential(tag+"_expo1", "", mzd, *lambda_sq1);
            RooExponential *expo2    = new RooExponential(tag+"_expo2", "", mzd, *lambda_sq2);
    //	RooExponential *expo1    = new RooExponential(tag+"_expo1", "", mzd, *lambda1);
    //	RooExponential *expo2    = new RooExponential(tag+"_expo2", "", mzd, *lambda2);
            RooAddPdf *doubleExpo = new RooAddPdf(tag + "_NE", "", RooArgList(*expo1, *expo2), *frac);
            RooAddPdf *ext_doubleExpo = new RooAddPdf(tag, "ext_dexpo", RooArgList(*doubleExpo), RooArgList(*Nbkg));
	    w.import(*ext_doubleExpo);
        }

        else {
            RooExponential *expo1    = new RooExponential(tag+"_expo1", "", mzd, *lambda_sq1);
            RooExponential *expo2    = new RooExponential(tag+"_expo2", "", mzd, *lambda_sq2);
    //	RooExponential *expo1    = new RooExponential(tag+"_expo1", "", mzd, *lambda1);
    //	RooExponential *expo2    = new RooExponential(tag+"_expo2", "", mzd, *lambda2);
            RooAddPdf *doubleExpo = new RooAddPdf(tag, "", RooArgList(*expo1, *expo2), *frac);
            w.import(*doubleExpo);
        }

	return tag;
}

TString MakeDoubleExpo_NE(TString tag, RooRealVar &mzd, RooWorkspace &w) {
	RooRealVar* lambda1 = new RooRealVar(tag + "_lambdaExpo1", "#lambda_{1}", 0.15, "");
	RooRealVar* lambda2 = new RooRealVar(tag + "_lambdaExpo2", "#lambda_{2}", 0.1, "");


	 //--------------------------------------------
	 // Square variables to avoid rising exponential
	 //--------------------------------------------
  	RooFormulaVar* lambda_sq1 = new RooFormulaVar(tag + "_lambda1Sq", "#lambda^{2}_{1}", "-1*@0*@0", *lambda1);
	RooFormulaVar* lambda_sq2 = new RooFormulaVar(tag + "_lambda2Sq", "#lambda^{2}_{2}", "-1*@0*@0", *lambda2);

	RooRealVar *frac    = new RooRealVar(tag+"_frac", "frac", 0.2, 0., 1.0, "");

	RooExponential *expo1    = new RooExponential(tag+"_expo1", "", mzd, *lambda_sq1);
	RooExponential *expo2    = new RooExponential(tag+"_expo2", "", mzd, *lambda_sq2);
//	RooExponential *expo1    = new RooExponential(tag+"_expo1", "", mzd, *lambda1);
//	RooExponential *expo2    = new RooExponential(tag+"_expo2", "", mzd, *lambda2);
	RooAddPdf* doubleExpo = new RooAddPdf(tag, "", RooArgList(*expo1, *expo2), *frac);

	frac->setConstant(kFALSE);
	lambda1->setConstant(kFALSE);
	lambda2->setConstant(kFALSE);

	w.import(*doubleExpo);

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

TString MakeSinglePow(TString tag, RooRealVar &mzd, RooWorkspace &w, bool ext) {
	RooRealVar *alpha  = new RooRealVar(tag+"_alpha","#alpha", -1.1);
  alpha->setConstant(kFALSE);

  if ( ext )
  {
    RooRealVar *Nbkg = new RooRealVar(tag+"_Nbkg", "N_{bkg}", 1, "events");
    RooGenericPdf *pow = new RooGenericPdf(tag+"_NE", "", "@0^@1", RooArgList(mzd,*alpha));
    RooAddPdf *ext_pow = new RooAddPdf(tag, "ext_pow", RooArgList(*pow), RooArgList(*Nbkg));
    Nbkg->setConstant(kFALSE);
    w.import(*ext_pow);
  }
  else
  {
    RooGenericPdf *pow = new RooGenericPdf(tag, "", "@0^@1", RooArgList(mzd,*alpha));
    w.import(*pow);
  }
	alpha->setRange(-10,0);
  return tag;
};

TString MakeDoublePow(TString tag, RooRealVar &mzd, RooWorkspace &w, bool ext) {
	RooRealVar *alpha1  = new RooRealVar(tag+"_alpha1", "#alpha_{1}", -1.0,"");
	RooRealVar *alpha2  = new RooRealVar(tag+"_alpha2", "#alpha_{2}", -0.5,"");
	RooRealVar *f       = new RooRealVar(tag+"_f", "f", 0.5, 0.0, 1.0);
    alpha1->setConstant(kFALSE);
	alpha2->setConstant(kFALSE);

	RooGenericPdf *pow1 = new RooGenericPdf(tag+"_pow1", "", "@0^@1", RooArgList(mzd, *alpha1));
	RooGenericPdf *pow2 = new RooGenericPdf(tag+"_pow2", "", "@0^@1", RooArgList(mzd, *alpha2));
    
    if ( ext )
    {
        RooRealVar *Nbkg    = new RooRealVar(tag+"_Nbkg","N_{bkg}", 1, "events");
        RooAddPdf *doublePow = new RooAddPdf(tag+"_NE", "", *pow1, *pow2, *f);
        RooAddPdf *ext_doublePow = new RooAddPdf(tag, "ext_dpow", RooArgList(*doublePow), RooArgList(*Nbkg));
        Nbkg->setConstant(kFALSE);
        w.import(*ext_doublePow);
    }

    else
    {
        RooAddPdf *doublePow = new RooAddPdf(tag, "", *pow1, *pow2, *f);
        w.import(*doublePow);
    }

    alpha1->setRange(-100,0);
    alpha2->setRange(-100,0);

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

TString MakeBernPoly2(bool extended, TString tag, RooRealVar& mzd, RooWorkspace &w) {
	RooRealVar *pC = new RooRealVar(tag+"_pC","C", 0.6, -10.,10,"");
	RooRealVar *p0 = new RooRealVar(tag+"_p0","p_0", -0.2, -10.,10.,"");
	RooRealVar *p1 = new RooRealVar(tag+"_p1","p_1", 0.3, -10.,10.,"");

	RooFormulaVar *pCmod = new RooFormulaVar(tag+"_pCmod","@0*@0",*pC);
	RooFormulaVar *p0mod = new RooFormulaVar(tag+"_p0mod","@0*@0",*p0);
	RooFormulaVar *p1mod = new RooFormulaVar(tag+"_p1mod","@0*@0",*p1);

	pC->setConstant(kFALSE);
	p0->setConstant(kFALSE);
	p1->setConstant(kFALSE);

    if (extended) {
    RooRealVar *Nbkg   = new RooRealVar(tag+"_Nbkg","N_{bkg}",1,"events");
    Nbkg->setConstant(kFALSE);
    //	RooBernstein *bern = new RooBernstein(tag+"_poly2", "", mzd, RooArgList(*pCmod, *p0mod, *p1mod));
        RooBernstein *bern = new RooBernstein(tag+"_NE", "", mzd, RooArgList(*pC, *p0, *p1));
        RooAddPdf *ext_bern = new RooAddPdf(tag,"ext_poly2", RooArgList(*bern), RooArgList(*Nbkg));
    w.import(*ext_bern);
    }

    else {
    //	RooBernstein *bern = new RooBernstein(tag+"_poly2", "", mzd, RooArgList(*pCmod, *p0mod, *p1mod));
        RooBernstein *bern = new RooBernstein(tag+"_NE", "", mzd, RooArgList(*pC, *p0, *p1));
        w.import(*bern);
    }

	return tag;
};

TString MakeBernPoly2_NE(TString tag, RooRealVar& mzd, RooWorkspace &w) {
	RooRealVar *pC = new RooRealVar(tag+"_pC","C", 0.6, -10.,10,"");
	RooRealVar *p0 = new RooRealVar(tag+"_p0","p_0", -0.2, -10.,10.,"");
	RooRealVar *p1 = new RooRealVar(tag+"_p1","p_1", 0.3, -10.,10.,"");

	RooFormulaVar *pCmod = new RooFormulaVar(tag+"_pCmod","@0*@0",*pC);
	RooFormulaVar *p0mod = new RooFormulaVar(tag+"_p0mod","@0*@0",*p0);
	RooFormulaVar *p1mod = new RooFormulaVar(tag+"_p1mod","@0*@0",*p1);

//	RooBernstein *bern = new RooBernstein(tag+"_poly2", "", mzd, RooArgList(*pCmod, *p0mod, *p1mod));
	RooBernstein *bern = new RooBernstein(tag, "", mzd, RooArgList(*pC, *p0, *p1));

	pC->setConstant(kFALSE);
	p0->setConstant(kFALSE);
	p1->setConstant(kFALSE);

	w.import(*bern);

	return tag;
};

TString MakeBernPoly3(bool extended, TString tag, RooRealVar &mzd, RooWorkspace &w) {
	RooRealVar *pC = new RooRealVar(tag+"_pC","C",1.2,"");
	RooRealVar *p0 = new RooRealVar(tag+"_p0","p_0",0.8,"");
	RooRealVar *p1 = new RooRealVar(tag+"_p1","p_1",0.9, "");
	RooRealVar *p2 = new RooRealVar(tag+"_p2","p_2",0.8, "");

	RooFormulaVar *pCmod = new RooFormulaVar(tag+"_pCmod","@0*@0",*pC);
	RooFormulaVar *p0mod = new RooFormulaVar(tag+"_p0mod","@0*@0",*p0);
	RooFormulaVar *p1mod = new RooFormulaVar(tag+"_p1mod","@0*@0",*p1);
	RooFormulaVar *p2mod = new RooFormulaVar(tag+"_p2mod","@0*@0",*p2);

	pC->setConstant(kFALSE);
	p0->setConstant(kFALSE);
	p1->setConstant(kFALSE);
	p2->setConstant(kFALSE);

    if (extended) {
    RooRealVar *Nbkg   = new RooRealVar(tag+"_Nbkg","N_{bkg}",1,"events");
        Nbkg->setConstant(kFALSE);
    //	RooBernstein *bern = new RooBernstein(tag+"_poly3","", mzd, RooArgList(*pCmod,*p0mod,*p1mod,*p2mod));
        RooBernstein *bern = new RooBernstein(tag+"_poly3","", mzd, RooArgList(*pC,*p0,*p1,*p2));
        RooAddPdf *ext_bern = new RooAddPdf(tag,"ext_poly3", RooArgList(*bern), RooArgList(*Nbkg));
    w.import(*ext_bern);
    }

    else {
    //	RooBernstein *bern = new RooBernstein(tag,"", mzd, RooArgList(*pCmod,*p0mod,*p1mod,*p2mod));
        RooBernstein *bern = new RooBernstein(tag,"", mzd, RooArgList(*pC,*p0,*p1,*p2));
        w.import(*bern);
    }

	return tag;
};

TString MakeBernPoly3_NE(TString tag, RooRealVar &mzd, RooWorkspace &w) {
	RooRealVar *pC = new RooRealVar(tag+"_pC","C",1.2,"");
	RooRealVar *p0 = new RooRealVar(tag+"_p0","p_0",0.8,"");
	RooRealVar *p1 = new RooRealVar(tag+"_p1","p_1",0.9, "");
	RooRealVar *p2 = new RooRealVar(tag+"_p2","p_2",0.8, "");


	RooFormulaVar *pCmod = new RooFormulaVar(tag+"_pCmod","@0*@0",*pC);
	RooFormulaVar *p0mod = new RooFormulaVar(tag+"_p0mod","@0*@0",*p0);
	RooFormulaVar *p1mod = new RooFormulaVar(tag+"_p1mod","@0*@0",*p1);
	RooFormulaVar *p2mod = new RooFormulaVar(tag+"_p2mod","@0*@0",*p2);

//	RooBernstein *bern = new RooBernstein(tag+"_poly3","", mzd, RooArgList(*pCmod,*p0mod,*p1mod,*p2mod));
	RooBernstein *bern = new RooBernstein(tag, "", mzd, RooArgList(*pC,*p0,*p1,*p2));

	pC->setConstant(kFALSE);
	p0->setConstant(kFALSE);
	p1->setConstant(kFALSE);
	p2->setConstant(kFALSE);

	w.import(*bern);

	return tag;
};

TString MakeBernPoly4(bool extended, TString tag, RooRealVar &mzd, RooWorkspace &w) {
	RooRealVar *pC = new RooRealVar(tag+"_pC","C",0.1,"");
	RooRealVar *p0 = new RooRealVar(tag+"_p0","p_0",0.1,"");
	RooRealVar *p1 = new RooRealVar(tag+"_p1","p_1",0.3, "");
	RooRealVar *p2 = new RooRealVar(tag+"_p2","p_2",0.2, "");
	RooRealVar *p3 = new RooRealVar(tag+"_p3","p_3",0.1, "");

	RooFormulaVar *pCmod = new RooFormulaVar(tag+"_pCmod","@0*@0",*pC);
	RooFormulaVar *p0mod = new RooFormulaVar(tag+"_p0mod","@0*@0",*p0);
	RooFormulaVar *p1mod = new RooFormulaVar(tag+"_p1mod","@0*@0",*p1);
	RooFormulaVar *p2mod = new RooFormulaVar(tag+"_p2mod","@0*@0",*p2);
	RooFormulaVar *p3mod = new RooFormulaVar(tag+"_p3mod","@0*@0",*p3);

	pC->setConstant(kFALSE);
    pC->setRange(-1,1);
	p0->setConstant(kFALSE);
    p0->setRange(-1,1);
	p1->setConstant(kFALSE);
    p1->setRange(-1,1);
	p2->setConstant(kFALSE);
    p2->setRange(-1,1);
	p3->setConstant(kFALSE);
    p3->setRange(-1,1);

    if (extended) {
        RooRealVar *Nbkg   = new RooRealVar(tag+"_Nbkg","N_{bkg}",1,"events");
        Nbkg->setConstant(kFALSE);
        //	RooBernstein *bern = new RooBernstein(tag+"_poly4","", mzd, RooArgList(*pCmod,*p0mod,*p1mod,*p2mod, *p3mod));
        RooBernstein *bern = new RooBernstein(tag+"_poly4","", mzd, RooArgList(*pC,*p0,*p1,*p2,*p3));
        RooAddPdf *ext_bern = new RooAddPdf(tag,"ext_poly4", RooArgList(*bern), RooArgList(*Nbkg));
        w.import(*ext_bern);
    }

    else {
        //	RooBernstein *bern = new RooBernstein(tag,"", mzd, RooArgList(*pCmod,*p0mod,*p1mod,*p2mod, *p3mod));
        RooBernstein *bern = new RooBernstein(tag,"", mzd, RooArgList(*pC,*p0,*p1,*p2,*p3));
        w.import(*bern);
    }

	return tag;
};

TString MakeBernPoly5(bool extended, TString tag, RooRealVar &mzd, RooWorkspace &w) {
	RooRealVar *pC = new RooRealVar(tag+"_pC","C",0,"");
	RooRealVar *p0 = new RooRealVar(tag+"_p0","p_0",0,"");
	RooRealVar *p1 = new RooRealVar(tag+"_p1","p_1",100, "");
	RooRealVar *p2 = new RooRealVar(tag+"_p2","p_2",0, "");
	RooRealVar *p3 = new RooRealVar(tag+"_p3","p_3",0, "");
	RooRealVar *p4 = new RooRealVar(tag+"_p4","p_4",1000, "");

	RooFormulaVar *pCmod = new RooFormulaVar(tag+"_pCmod","@0*@0",*pC);
	RooFormulaVar *p0mod = new RooFormulaVar(tag+"_p0mod","@0*@0",*p0);
	RooFormulaVar *p1mod = new RooFormulaVar(tag+"_p1mod","@0*@0",*p1);
	RooFormulaVar *p2mod = new RooFormulaVar(tag+"_p2mod","@0*@0",*p2);
	RooFormulaVar *p3mod = new RooFormulaVar(tag+"_p3mod","@0*@0",*p3);
	RooFormulaVar *p4mod = new RooFormulaVar(tag+"_p4mod","@0*@0",*p4);

	pC->setConstant(kFALSE);
	p0->setConstant(kFALSE);
	p1->setConstant(kFALSE);
	p2->setConstant(kFALSE);
	p3->setConstant(kFALSE);
	p4->setConstant(kFALSE);

    if (extended) {
        RooRealVar *Nbkg   = new RooRealVar(tag+"_Nbkg","N_{bkg}",1,"events");
        Nbkg->setConstant(kFALSE);
        //	RooBernstein *bern = new RooBernstein(tag+"_poly5","", mzd, RooArgList(*pCmod,*p0mod,*p1mod,*p2mod, *p3mod, *p4mod));
        RooBernstein *bern = new RooBernstein(tag+"_poly5","", mzd, RooArgList(*pC,*p0,*p1,*p2,*p3,*p4));
        RooAddPdf *ext_bern = new RooAddPdf(tag,"ext_poly5", RooArgList(*bern), RooArgList(*Nbkg));
    w.import(*ext_bern);
    }

    else {
        //	RooBernstein *bern = new RooBernstein(tag,"", mzd, RooArgList(*pCmod,*p0mod,*p1mod,*p2mod, *p3mod, *p4mod));
        RooBernstein *bern = new RooBernstein(tag,"", mzd, RooArgList(*pC,*p0,*p1,*p2,*p3,*p4));
        w.import(*bern);
    }

	return tag;
};
