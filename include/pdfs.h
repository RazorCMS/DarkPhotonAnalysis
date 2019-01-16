#ifndef pdfs_H
#define pdfs_H

//ROOT INCLUDES
#include <TString.h>

//ROOTFIT INCLUDES
#include <RooWorkspace.h>
#include <RooRealVar.h>

TString MakeDoubleCB(bool extended, TString tag, double mass, RooRealVar &mzd, RooWorkspace &w);
TString MakeDoubleCB_NE(TString tag, double mass, RooRealVar &mzd, RooWorkspace &w);

TString MakeCB(bool extended, TString tag, double mass, RooRealVar &mzd, RooWorkspace &w);

TString MakeBreitWigner(bool extended, TString tag, double mass, double lifetime, RooRealVar &mzd, RooWorkspace &w);
TString MakeBreitWigner_NE(TString tag, double mass, double lifetime, RooRealVar &mzd, RooWorkspace &w);

TString MakeExpo(bool extended, TString tag, RooRealVar &mzd, RooWorkspace &w);
TString MakeExpo_NE(TString tag, RooRealVar &mzd, RooWorkspace &w);

TString MakeDoubleExpo(bool extended, TString tag, RooRealVar &mzd, RooWorkspace &w);
TString MakeDoubleExpo_NE(TString tag, RooRealVar &mzd, RooWorkspace &w);
TString MakeDoubleExpoN1N2(TString tag, RooRealVar &mzd, RooWorkspace &w);

TString MakeSinglePow(TString tag, RooRealVar &mzd, RooWorkspace &w, bool ext = true);

TString MakeDoublePow(TString tag, RooRealVar &mzd,RooWorkspace &w, bool ext = true);

TString MakeBernPoly2(bool extended, TString tag, RooRealVar& mzd, RooWorkspace &w);
TString MakeBernPoly2_NE(TString tag, RooRealVar& mzd, RooWorkspace &w);

TString MakeChebychevPoly2(TString tag, RooRealVar& mzd, RooWorkspace &w);

TString MakeBernPoly3(bool extended, TString tag, RooRealVar &mzd, RooWorkspace &w);
TString MakeBernPoly3_NE(TString tag, RooRealVar &mzd, RooWorkspace &w);

TString MakeBernPoly4(bool extended, TString tag, RooRealVar &mzd, RooWorkspace &w);
TString MakeBernPoly5(bool extended, TString tag, RooRealVar &mzd, RooWorkspace &w);
#endif
