#ifndef pdfs_v3_H
#define pdfs_v3_H

//ROOT INCLUDES
#include <TString.h>

//ROOTFIT INCLUDES
#include <RooWorkspace.h>
#include <RooRealVar.h>

TString MakeBreitWigner(TString tag, double mass, double lifetime, RooRealVar &mzd, RooWorkspace &w);
TString MakeExpo(TString tag, RooRealVar &mzd, RooWorkspace &w);
TString MakeDoubleExpo(TString tag, RooRealVar &mzd, RooWorkspace &w);
TString MakeDoubleExpoN1N2(TString tag, RooRealVar &mzd, RooWorkspace &w);
TString MakeSinglePow(TString tag, RooRealVar &mzd, RooWorkspace &w);
TString MakeDoublePow(TString tag, RooRealVar &mzd,RooWorkspace &w);
TString MakeBernPoly2(TString tag, RooRealVar& mzd, RooWorkspace &w);
TString MakeChebychevPoly2(TString tag, RooRealVar& mzd, RooWorkspace &w);
TString MakeBernPoly3(TString tag, RooRealVar &mzd, RooWorkspace &w);

#endif
