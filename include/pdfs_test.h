#ifndef pdfs_H
#define pdfs_H

//ROOT INCLUDES
#include <TString.h>

//ROOTFIT INCLUDES
#include <RooWorkspace.h>
#include <RooRealVar.h>

TString MakeDoubleCB(TString tag, double mass, RooRealVar &mzd, RooWorkspace &w);
TString MakeDoubleCB_NE(TString tag, double mass, RooRealVar &mzd, RooWorkspace &w);

TString MakeCB(TString tag, double mass, RooRealVar &mzd, RooWorkspace &w);

TString MakeBreitWigner(TString tag, double mass, double lifetime, RooRealVar &mzd, RooWorkspace &w);
TString MakeBreitWigner_NE(TString tag, double mass, double lifetime, RooRealVar &mzd, RooWorkspace &w);

TString MakeExpo(TString tag, RooRealVar &mzd, RooWorkspace &w);
TString MakeExpo_NE(TString tag, RooRealVar &mzd, RooWorkspace &w);

TString MakeDoubleExpo(TString tag, RooRealVar &mzd, RooWorkspace &w);
TString MakeDoubleExpo_NE(TString tag, RooRealVar &mzd, RooWorkspace &w);
TString MakeDoubleExpoN1N2(TString tag, RooRealVar &mzd, RooWorkspace &w);

TString MakeSinglePow(TString tag, RooRealVar &mzd, RooWorkspace &w);

TString MakeDoublePow(TString tag, RooRealVar &mzd,RooWorkspace &w);

TString MakeBernPoly2(TString tag, RooRealVar& mzd, RooWorkspace &w);
TString MakeBernPoly2_NE(TString tag, RooRealVar& mzd, RooWorkspace &w);

TString MakeChebychevPoly2(TString tag, RooRealVar& mzd, RooWorkspace &w);

TString MakeBernPoly3(TString tag, RooRealVar &mzd, RooWorkspace &w);
TString MakeBernPoly3_NE(TString tag, RooRealVar &mzd, RooWorkspace &w);

TString MakeBernPoly4(TString tag, RooRealVar &mzd, RooWorkspace &w);
TString MakeBernPoly5(TString tag, RooRealVar &mzd, RooWorkspace &w);
#endif
