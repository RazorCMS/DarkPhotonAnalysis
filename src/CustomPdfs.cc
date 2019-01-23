#include <iostream>
#include <math.h>
#include <TMath.h>

#include "CustomPdfs.hh"
#include <RooRealVar.h>
#include <RooRealConstant.h>

using namespace RooFit;
using namespace std; 

ClassImp(RooCB);

RooCB::RooCB(){};

RooCB::RooCB(const char *name, const char *title,
	     RooAbsReal& _x,
	     RooAbsReal& _mean,
	     RooAbsReal& _width,
	     RooAbsReal& _alpha,
	     RooAbsReal& _n,
              RooAbsReal& _theta
	     ) :
  RooAbsPdf(name,title),
  x("x","x",this,_x),
  mean("mean","mean",this,_mean),
  width("width","width",this,_width),
  alpha("alpha","alpha",this,_alpha),
  n("n","n",this,_n),
  theta("theta","theta",this,_theta)
{
};

RooCB::RooCB(const RooCB& other, const char* name) :
  RooAbsPdf(other,name),
  x("x",this,other.x),
  mean("mean",this,other.mean),
  width("width",this,other.width),
  alpha("alpha",this,other.alpha),
  n("n",this,other.n),
  theta("theta",this,other.theta)
{
};

double RooCB::evaluate() const
{
  double a = cos(theta)*alpha - sin(theta)*width;
  double w = sin(theta)*alpha + cos(theta)*width;

  double t = (x-mean)/w;
  if(a<0) t = -t;

  double absa = fabs((double)a);

  double A = TMath::Power(n/absa,n)*exp(-0.5*absa*absa);
  double B = n/absa-absa;

  if(t >= -absa){
    return exp(-0.5*t*t);
  }else{
    return A/TMath::Power(B-t,n);
  }
};

//HighMass Diphoton
ClassImp(RooHMDiphoton);

RooHMDiphoton::RooHMDiphoton(){};

RooHMDiphoton::RooHMDiphoton(const char *name, const char *title,
			     RooAbsReal& _x,
			     RooAbsReal& _a,
			     RooAbsReal& _b
			     ) :
  RooAbsPdf(name,title),
  x("x","x",this,_x),
  a("a","a",this,_a),
  b("b","b",this,_b)
  
{
};

RooHMDiphoton::RooHMDiphoton(const RooHMDiphoton& other, const char* name) :
  RooAbsPdf(other,name),
  x("x",this,other.x),
  a("a",this,other.a),
  b("b",this,other.b)
{
};

double RooHMDiphoton::evaluate() const
{
  if ( x < 0 ) return 0.0;
  return TMath::Power( x, a+b*TMath::Log(x) );
  
};

//RooDoubleCB
ClassImp(RooDoubleCB)

RooDoubleCB::RooDoubleCB( ){ };

RooDoubleCB::RooDoubleCB(const char *name, const char *title, 
			 RooAbsReal& _x,
			 RooAbsReal& _mean,
			 RooAbsReal& _width,
			 RooAbsReal& _alpha1,
			 RooAbsReal& _n1,
			 RooAbsReal& _alpha2,
			 RooAbsReal& _n2
			 ) :
  RooAbsPdf(name,title), 
  x("x","x",this,_x),
  mean("mean","mean",this,_mean),
  width("width","width",this,_width),
  alpha1("alpha1","alpha1",this,_alpha1),
  n1("n1","n1",this,_n1),
  alpha2("alpha2","alpha2",this,_alpha2),
  n2("n2","n2",this,_n2)
{ 
};


RooDoubleCB::RooDoubleCB(const RooDoubleCB& other, const char* name) :  
  RooAbsPdf(other,name), 
  x("x",this,other.x),
  mean("mean",this,other.mean),
  width("width",this,other.width),
  alpha1("alpha1",this,other.alpha1),
  n1("n1",this,other.n1),
  alpha2("alpha2",this,other.alpha2),
  n2("n2",this,other.n2)

{ 
};

double RooDoubleCB::evaluate() const 
{ 
  double t = (x-mean)/width;
  if( t >= -alpha1 && t <= alpha2 )
    {
      return exp(-0.5*t*t);
    }
  else if ( t < -alpha1 )
    {
      double A1 = pow(n1/fabs(alpha1),n1)*exp(-alpha1*alpha1/2);
      double B1 = n1/fabs(alpha1)-fabs(alpha1);
      return A1*pow(B1-t,-n1);
    }
  else if ( t > alpha2 )
    {
      double A2 = pow(n2/fabs(alpha2),n2)*exp(-alpha2*alpha2/2);
      double B2 = n2/fabs(alpha2)-fabs(alpha2);
      return A2*pow(B2+t,-n2);
    }
  else
    {
      cout << "ERROR evaluating range... t = " << t << endl;
      return 99;
    }
   
};

Int_t RooDoubleCB::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* range) const 
{
  if (matchArgs(allVars,analVars,x)) return 1;
  return 0;
};

Double_t RooDoubleCB::analyticalIntegral(Int_t code, const char* rangeName) const 
{
  assert(code==1) ;
 
  double central=0;
  double left=0;
  double right=0;
 
  static const Double_t root2 = sqrt(2) ;
  static const Double_t rootPiBy2 = sqrt(atan2(0.0,-1.0)/2.0);
  Double_t xscale = root2*width;
 
  //compute gaussian contribution
  double central_low =max(x.min(rangeName),mean - alpha1*width );
  double central_high=min(x.max(rangeName),mean + alpha2*width );
  if(central_low < central_high) // is the gaussian part in range?
    central = rootPiBy2*width*(TMath::Erf((central_high-mean)/xscale)-TMath::Erf((central_low-mean)/xscale));
 
  //compute left tail;
  double A1 = pow(n1/fabs(alpha1),n1)*exp(-alpha1*alpha1/2);
  double B1 = n1/fabs(alpha1)-fabs(alpha1);
 
  double left_low=x.min(rangeName);
  double left_high=min(x.max(rangeName),mean - alpha1*width);
  if(left_low < left_high){ //is the left tail in range?
    if(fabs(n1-1.0)>1.e-5)
      left = A1/(-n1+1.0)*width*(pow(B1-(left_low-mean)/width,-n1+1.)-pow(B1-(left_high-mean)/width,-n1+1.));
    else
      left = A1*width*(log(B1-(left_low-mean)/width) - log(B1-(left_high-mean)/width) );
  }
 
  //compute right tail;
  double A2 = pow(n2/fabs(alpha2),n2)*exp(-alpha2*alpha2/2);
  double B2 = n2/fabs(alpha2)-fabs(alpha2);
 
  double right_low=max(x.min(rangeName),mean + alpha2*width);
  double right_high=x.max(rangeName);
  if(right_low < right_high){ //is the right tail in range?
    if(fabs(n2-1.0)>1.e-5)
      right = A2/(-n2+1.0)*width*(pow(B2+(right_high-mean)/width,-n2+1.)-pow(B2+(right_low-mean)/width,-n2+1.));
    else
      right = A2*width*(log(B2+(right_high-mean)/width) - log(B2+(right_low-mean)/width) );
  }
     
  return left+central+right;
 
};

//RooDoubleCBInterpolate
ClassImp(RooDoubleCBInterpolate)

RooDoubleCBInterpolate::RooDoubleCBInterpolate( ){ };

RooDoubleCBInterpolate::RooDoubleCBInterpolate(const char *name, const char *title, 
					       RooAbsReal& _x,
					       RooAbsReal& _mass
					       ) :
  RooAbsPdf(name,title), 
  x("x","x",this,_x),
  mass("mass","mass",this, _mass)
{
};


RooDoubleCBInterpolate::RooDoubleCBInterpolate(const RooDoubleCBInterpolate& other, const char* name) :  
  RooAbsPdf(other,name), 
  x("x",this,other.x),
  mass("mass",this, other.mass)
{ 
};

Double_t RooDoubleCBInterpolate::getMean( Double_t m ) const
{
  if ( m  < 500 )
    {
      return 0.998226*m;
    }
  else if ( m >= 500. && m < 740. )
    {
      return ((737.55-499.113)/(740.-500.))*(m-500.) + 499.113;
    }
  else if ( m >= 740. && m < 745. )
    {
      return ((742.696-737.55)/(745.-740.))*(m-740.) + 737.55;
    }
  else if (  m >= 745. && m < 750. )
    {
      return ((747.501-742.696)/(750.-745.))*(m-745.) + 742.696;
    }
  else if (  m >= 750. && m < 755. )
    {
      return ((752.634-747.501)/(755.-750.))*(m-750.) + 747.501;
    }
  else if (  m >= 755. && m < 760. )
    {
      return ((757.577-752.634)/(760.-755.))*(m-755.) + 752.634;
    }
  else if (  m >= 760. && m < 1000. )
    {
      return ((995.441-757.577)/(1000.-760.))*(m-760.) + 757.577;
    }
  else if (  m >= 1000. && m < 1250. )
    {
      return  ((1242.48-995.441)/(1250.-1000.))*(m-1000.) + 995.441;
    }
  else if (  m >= 1250. && m < 1500. )
    {
      return ((1489.63-1242.48)/(1500.-1250.))*(m-1250.) + 1242.48;
    }
  else if (  m >= 1500. && m < 1750. )
    {
      return ((1736.32-1489.63)/(1750.-1500.))*(m-1500.) + 1489.63;
    }
  else if (  m >= 1750. && m < 2000. )
    {
      return ((1983.38-1736.32)/(2000.-1750.))*(m-1750.) + 1736.32;
    }
  else if (  m >= 2000. && m < 2250. )
    {
      return ((2230.57-1983.38)/(2250.-2000.))*(m-2000.) + 1983.38;
    }
  else if (  m >= 2250. && m < 2500. )
    {
      return ((2477.21-2230.57)/(2500.-2250.))*(m-2250.) + 2230.57;
    }
  else if (  m >= 2500. && m < 2750. )
    {
      return ((2723.35-2477.21)/(2750.-2500.))*(m-2500.) + 2477.21;
    }
  else if (  m >= 2750. && m < 3000. )
    {
      return ((2968.51-2723.35)/(3000.-2750.))*(m-2750.) + 2723.35;
    }
  else if (  m >= 3000. && m < 3250. )
    {
      return ((3214.51-2968.51)/(3250.-3000.))*(m-3000.) + 2968.51;
    }
  else if (  m >= 3250. && m < 3500. )
    {
      return ((3460.37-3214.51)/(3500.-3250.))*(m-3250.) + 3214.51;
    }
  else if (  m >= 3500. && m < 3750. )
    {
      return ((3706.29-3460.37)/(3750.-3500.))*(m-3500.) + 3460.37;
    }
  else if (  m >= 3750. && m < 4000. )
    {
      return ((3957.41-3706.29)/(4000.-3750.))*(m-3750.) + 3706.29;
    }
 return 0.9893525*m;
};

Double_t RooDoubleCBInterpolate::getSigma( Double_t m ) const
{
  if ( m  < 500 )
    {
      return 4.71304;
    }
  else if ( m >= 500. && m < 740. )
    {
      return ((6.95368-4.71304)/(740.-500.))*(m-500.) + 4.71304;
    }
  else if ( m >= 740. && m < 745. )
    {
      return ((7.03181-6.95368)/(745.-740.))*(m-740.) + 6.95368;
    }
  else if (  m >= 745. && m < 750. )
    {
      return ((7.03829-7.03181)/(750.-745.))*(m-745.) + 7.03181;
    }
  else if (  m >= 750. && m < 755. )
    {
      return ((7.24748-7.03829)/(755.-750.))*(m-750.) + 7.03829;
    }
  else if (  m >= 755. && m < 760. )
    {
      return ((6.93232-7.24748)/(760.-755.))*(m-755.) + 7.24748;
    }
  else if (  m >= 760. && m < 1000. )
    {
      return ((9.50926-6.93232)/(1000.-760.))*(m-760.) + 6.93232;
    }
  else if (  m >= 1000. && m < 1250. )
    {
      return  ((11.9767-9.50926)/(1250.-1000.))*(m-1000.) + 9.50926;
    }
  else if (  m >= 1250. && m < 1500. )
    {
      return ((14.5047-11.9767)/(1500.-1250.))*(m-1250.) + 11.9767;
    }
  else if (  m >= 1500. && m < 1750. )
    {
      return ((16.8543-14.5047)/(1750.-1500.))*(m-1500.) + 14.5047;
    }
  else if (  m >= 1750. && m < 2000. )
    {
      return ((19.6088-16.8543)/(2000.-1750.))*(m-1750.) + 16.8543;
    }
  else if (  m >= 2000. && m < 2250. )
    {
      return ((22.1248-19.6088)/(2250.-2000.))*(m-2000.) + 19.6088;
    }
  else if (  m >= 2250. && m < 2500. )
    {
      return ((25.0484-22.1248)/(2500.-2250.))*(m-2250.) + 22.1248;
    }
  else if (  m >= 2500. && m < 2750. )
    {
      return ((27.5967-25.0484)/(2750.-2500.))*(m-2500.) + 25.0484;
    }
  else if (  m >= 2750. && m < 3000. )
    {
      return ((30.546-27.5967)/(3000.-2750.))*(m-2750.) + 27.5967;
    }
  else if (  m >= 3000. && m < 3250. )
    {
      return ((33.7079-30.546)/(3250.-3000.))*(m-3000.) + 30.546;
    }
  else if (  m >= 3250. && m < 3500. )
    {
      return ((35.8802-33.7079)/(3500.-3250.))*(m-3250.) + 33.7079;
    }
  else if (  m >= 3500. && m < 3750. )
    {
      return ((39.1346-35.8802)/(3750.-3500.))*(m-3500.) + 35.8802;
    }
  else if (  m >= 3750. && m < 4000. )
    {
      return ((41.744-39.1346)/(4000.-3750.))*(m-3750.) + 39.1346;
    }
  
  return 0.01044*m;
};

Double_t RooDoubleCBInterpolate::getAlpha1( Double_t m ) const
{
  if ( m  < 500 )
    {
      return 1.24366;
    }
  else if ( m >= 500. && m < 740. )
    {
      return ((1.32445-1.24366)/(740.-500.))*(m-500.) + 1.24366;
    }
  else if ( m >= 740. && m < 745. )
    {
      return ((1.31705-1.32445)/(745.-740.))*(m-740.) + 1.32445;
    }
  else if (  m >= 745. && m < 750. )
    {
      return ((1.29296-1.31705)/(750.-745.))*(m-745.) + 1.31705;
    }
  else if (  m >= 750. && m < 755. )
    {
      return ((1.33272-1.29296)/(755.-750.))*(m-750.) + 1.29296;
    }
  else if (  m >= 755. && m < 760. )
    {
      return ((1.23588-1.33272)/(760.-755.))*(m-755.) + 1.33272;
    }
  else if (  m >= 760. && m < 1000. )
    {
      return ((1.2881-1.23588)/(1000.-760.))*(m-760.) + 1.23588;
    }
  else if (  m >= 1000. && m < 1250. )
    {
      return  ((1.29209-1.2881)/(1250.-1000.))*(m-1000.) + 1.2881;
    }
  else if (  m >= 1250. && m < 1500. )
    {
      return ((1.31867-1.29209)/(1500.-1250.))*(m-1250.) + 1.29209;
    }
  else if (  m >= 1500. && m < 1750. )
    {
      return ((1.27682-1.31867)/(1750.-1500.))*(m-1500.) + 1.31867;
    }
  else if (  m >= 1750. && m < 2000. )
    {
      return ((1.28911-1.27682)/(2000.-1750.))*(m-1750.) + 1.27682;
    }
  else if (  m >= 2000. && m < 2250. )
    {
      return ((1.22497-1.28911)/(2250.-2000.))*(m-2000.) + 1.28911;
    }
  else if (  m >= 2250. && m < 2500. )
    {
      return ((1.19581-1.22497)/(2500.-2250.))*(m-2250.) + 1.22497;
    }
  else if (  m >= 2500. && m < 2750. )
    {
      return ((1.12843-1.19581)/(2750.-2500.))*(m-2500.) + 1.19581;
    }
  else if (  m >= 2750. && m < 3000. )
    {
      return ((1.25584-1.12843)/(3000.-2750.))*(m-2750.) + 1.12843;
    }
  else if (  m >= 3000. && m < 3250. )
    {
      return ((1.22101-1.25584)/(3250.-3000.))*(m-3000.) + 1.25584;
    }
  else if (  m >= 3250. && m < 3500. )
    {
      return ((1.13156-1.22101)/(3500.-3250.))*(m-3250.) + 1.22101;
    }
  else if (  m >= 3500. && m < 3750. )
    {
      return ((1.08709-1.13156)/(3750.-3500.))*(m-3500.) + 1.13156;
    }
  else if (  m >= 3750. && m < 4000. )
    {
      return ((0.495629-1.08709)/(4000.-3750.))*(m-3750.) + 1.08709;
    }
  return 0.495629;
};

Double_t RooDoubleCBInterpolate::getN1( Double_t m ) const
{
  if ( m  < 500 )
    {
      return 2.81157;
    }
  else if ( m >= 500. && m < 740. )
    {
      return ((2.59159-2.81157)/(740.-500.))*(m-500.) + 2.81157;
    }
  else if ( m >= 740. && m < 745. )
    {
      return ((2.68365-2.59159)/(745.-740.))*(m-740.) + 2.59159;
    }
  else if (  m >= 745. && m < 750. )
    {
      return ((2.74203-2.68365)/(750.-745.))*(m-745.) + 2.68365;
    }
  else if (  m >= 750. && m < 755. )
    {
      return ((2.60803-2.74203)/(755.-750.))*(m-750.) + 2.74203;
    }
  else if (  m >= 755. && m < 760. )
    {
      return ((2.87792-2.60803)/(760.-755.))*(m-755.) + 2.60803;
    }
  else if (  m >= 760. && m < 1000. )
    {
      return ((2.78344-2.87792)/(1000.-760.))*(m-760.) + 2.87792;
    }
  else if (  m >= 1000. && m < 1250. )
    {
      return  ((2.83202-2.78344)/(1250.-1000.))*(m-1000.) + 2.78344;
    }
  else if (  m >= 1250. && m < 1500. )
    {
      return ((2.76646-2.83202)/(1500.-1250.))*(m-1250.) + 2.83202;
    }
  else if (  m >= 1500. && m < 1750. )
    {
      return ((2.97399-2.76646)/(1750.-1500.))*(m-1500.) + 2.76646;
    }
  else if (  m >= 1750. && m < 2000. )
    {
      return ((2.93861-2.97399)/(2000.-1750.))*(m-1750.) + 2.97399;
    }
  else if (  m >= 2000. && m < 2250. )
    {
      return ((3.24526-2.93861)/(2250.-2000.))*(m-2000.) + 2.93861;
    }
  else if (  m >= 2250. && m < 2500. )
    {
      return ((3.33493-3.24526)/(2500.-2250.))*(m-2250.) + 3.24526;
    }
  else if (  m >= 2500. && m < 2750. )
    {
      return ((3.70123-3.33493)/(2750.-2500.))*(m-2500.) + 3.33493;
    }
  else if (  m >= 2750. && m < 3000. )
    {
      return ((2.78014-3.70123)/(3000.-2750.))*(m-2750.) + 3.70123;
    }
  else if (  m >= 3000. && m < 3250. )
    {
      return ((2.9969-2.78014)/(3250.-3000.))*(m-3000.) + 2.78014;
    }
  else if (  m >= 3250. && m < 3500. )
    {
      return ((3.17137-2.9969)/(3500.-3250.))*(m-3250.) + 2.9969;
    }
  else if (  m >= 3500. && m < 3750. )
    {
      return ((3.30329-3.17137)/(3750.-3500.))*(m-3500.) + 3.17137;
    }
  else if (  m >= 3750. && m < 4000. )
    {
      return ((3.71696-3.30329)/(4000.-3750.))*(m-3750.) + 3.30329;
    }  
  return 3.71696;
};

Double_t RooDoubleCBInterpolate::getAlpha2( Double_t m ) const
{
  if ( m  < 500 )
    {
      return 1.71965;
    }
  else if ( m >= 500. && m < 740. )
    {
      return ((1.74497-1.71965)/(740.-500.))*(m-500.) + 1.71965;
    }
  else if ( m >= 740. && m < 745. )
    {
      return ((1.8299-1.74497)/(745.-740.))*(m-740.) + 1.74497;
    }
  else if (  m >= 745. && m < 750. )
    {
      return ((1.80373-1.8299)/(750.-745.))*(m-745.) + 1.8299;
    }
  else if (  m >= 750. && m < 755. )
    {
      return ((1.83062-1.80373)/(755.-750.))*(m-750.) + 1.80373;
    }
  else if (  m >= 755. && m < 760. )
    {
      return ((1.72937-1.83062)/(760.-755.))*(m-755.) + 1.83062;
    }
  else if (  m >= 760. && m < 1000. )
    {
      return ((1.88932-1.72937)/(1000.-760.))*(m-760.) + 1.72937;
    }
  else if (  m >= 1000. && m < 1250. )
    {
      return  ((1.83134-1.88932)/(1250.-1000.))*(m-1000.) + 1.88932;
    }
  else if (  m >= 1250. && m < 1500. )
    {
      return ((2.02476-1.83134)/(1500.-1250.))*(m-1250.) + 1.83134;
    }
  else if (  m >= 1500. && m < 1750. )
    {
      return ((1.53062-2.02476)/(1750.-1500.))*(m-1500.) + 2.02476;
    }
  else if (  m >= 1750. && m < 2000. )
    {
      return ((2.09688-1.53062)/(2000.-1750.))*(m-1750.) + 1.53062;
    }
  else if (  m >= 2000. && m < 2250. )
    {
      return ((2.06856-2.09688)/(2250.-2000.))*(m-2000.) + 2.09688;
    }
  else if (  m >= 2250. && m < 2500. )
    {
      return ((2.155-2.06856)/(2500.-2250.))*(m-2250.) + 2.06856;
    }
  else if (  m >= 2500. && m < 2750. )
    {
      return ((2.24434-2.155)/(2750.-2500.))*(m-2500.) + 2.155;
    }
  else if (  m >= 2750. && m < 3000. )
    {
      return ((2.21766-2.24434)/(3000.-2750.))*(m-2750.) + 2.24434;
    }
  else if (  m >= 3000. && m < 3250. )
    {
      return ((2.17426-2.21766)/(3250.-3000.))*(m-3000.) + 2.21766;
    }
  else if (  m >= 3250. && m < 3500. )
    {
      return ((1.94443-2.17426)/(3500.-3250.))*(m-3250.) + 2.17426;
    }
  else if (  m >= 3500. && m < 3750. )
    {
      return ((2.09163-1.94443)/(3750.-3500.))*(m-3500.) + 1.94443;
    }
  else if (  m >= 3750. && m < 4000. )
    {
      return ((0.678367-2.09163)/(4000.-3750.))*(m-3750.) + 2.09163;
    } 
  return 0.678367;
};

Double_t RooDoubleCBInterpolate::getN2( Double_t m ) const
{
  if ( m  < 500 )
    {
      return 4.52615;
    }
  else if ( m >= 500. && m < 740. )
    {
      return ((4.9256-4.52615)/(740.-500.))*(m-500.) + 4.52615;
    }
  else if ( m >= 740. && m < 745. )
    {
      return ((4.25446-4.9256)/(745.-740.))*(m-740.) + 4.9256;
    }
  else if (  m >= 745. && m < 750. )
    {
      return ((4.29415-4.25446)/(750.-745.))*(m-745.) + 4.25446;
    }
  else if (  m >= 750. && m < 755. )
    {
      return ((4.98554-4.29415)/(755.-750.))*(m-750.) + 4.29415;
    }
  else if (  m >= 755. && m < 760. )
    {
      return ((5.09606-4.98554)/(760.-755.))*(m-755.) + 4.98554;
    }
  else if (  m >= 760. && m < 1000. )
    {
      return ((4.39101-5.09606)/(1000.-760.))*(m-760.) + 5.09606;
    }
  else if (  m >= 1000. && m < 1250. )
    {
      return  ((5.15418-4.39101)/(1250.-1000.))*(m-1000.) + 4.39101;
    }
  else if (  m >= 1250. && m < 1500. )
    {
      return ((4.14705-5.15418)/(1500.-1250.))*(m-1250.) + 5.15418;
    }
  else if (  m >= 1500. && m < 1750. )
    {
      return ((108.872-4.14705)/(1750.-1500.))*(m-1500.) + 4.14705;
    }
  else if (  m >= 1750. && m < 2000. )
    {
      return ((3.94784-108.872)/(2000.-1750.))*(m-1750.) + 108.872;
    }
  else if (  m >= 2000. && m < 2250. )
    {
      return ((5.10298-3.94784)/(2250.-2000.))*(m-2000.) + 3.94784;
    }
  else if (  m >= 2250. && m < 2500. )
    {
      return ((4.87344-5.10298)/(2500.-2250.))*(m-2250.) + 5.10298;
    }
  else if (  m >= 2500. && m < 2750. )
    {
      return ((4.64861-4.87344)/(2750.-2500.))*(m-2500.) + 4.87344;
    }
  else if (  m >= 2750. && m < 3000. )
    {
      return ((4.8015-4.64861)/(3000.-2750.))*(m-2750.) + 4.64861;
    }
  else if (  m >= 3000. && m < 3250. )
    {
      return ((5.93429-4.8015)/(3250.-3000.))*(m-3000.) + 4.8015;
    }
  else if (  m >= 3250. && m < 3500. )
    {
      return ((10.7216-5.93429)/(3500.-3250.))*(m-3250.) + 5.93429;
    }
  else if (  m >= 3500. && m < 3750. )
    {
      return ((7.2023-10.7216)/(3750.-3500.))*(m-3500.) + 10.7216;
    }
  else if (  m >= 3750. && m < 4000. )
    {
      return ((133.548-7.2023)/(4000.-3750.))*(m-3750.) + 7.2023;
    } 
  return 133.548;
};

double RooDoubleCBInterpolate::evaluate() const 
{
  double mean   = getMean( mass );
  double width  = getSigma( mass );
  double n1     = getN1( mass );
  double alpha1 = getAlpha1( mass );
  double n2     = getN2( mass );
  double alpha2 = getAlpha2( mass );
  
  double t = (x-mean)/width;
  if( t >= -alpha1 && t <= alpha2 )
    {
      return exp(-0.5*t*t);
    }
  else if ( t < -alpha1 )
    {
      double A1 = pow(n1/fabs(alpha1),n1)*exp(-alpha1*alpha1/2);
      double B1 = n1/fabs(alpha1)-fabs(alpha1);
      return A1*pow(B1-t,-n1);
    }
  else if ( t > alpha2 )
    {
      double A2 = pow(n2/fabs(alpha2),n2)*exp(-alpha2*alpha2/2);
      double B2 = n2/fabs(alpha2)-fabs(alpha2);
      return A2*pow(B2+t,-n2);
    }
  else
    {
      cout << "ERROR evaluating range... t = " << t << endl;
      return 99;
    }
   
};

Int_t RooDoubleCBInterpolate::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* range) const 
{
  if (matchArgs(allVars,analVars,x)) return 1;
  return 0;
};

Double_t RooDoubleCBInterpolate::analyticalIntegral(Int_t code, const char* rangeName) const 
{
  assert(code==1) ;
  //
  double mean   = getMean( mass );
  double width  = getSigma( mass );
  double n1     = getN1( mass );
  double alpha1 = getAlpha1( mass );
  double n2     = getN2( mass );
  double alpha2 = getAlpha2( mass );
  //
  
  double central=0;
  double left=0;
  double right=0;
 
  static const Double_t root2 = sqrt(2) ;
  static const Double_t rootPiBy2 = sqrt(atan2(0.0,-1.0)/2.0);
  Double_t xscale = root2*width;
 
  //compute gaussian contribution
  double central_low =max(x.min(rangeName),mean - alpha1*width );
  double central_high=min(x.max(rangeName),mean + alpha2*width );
  if(central_low < central_high) // is the gaussian part in range?
    central = rootPiBy2*width*(TMath::Erf((central_high-mean)/xscale)-TMath::Erf((central_low-mean)/xscale));
 
  //compute left tail;
  double A1 = pow(n1/fabs(alpha1),n1)*exp(-alpha1*alpha1/2);
  double B1 = n1/fabs(alpha1)-fabs(alpha1);
 
  double left_low=x.min(rangeName);
  double left_high=min(x.max(rangeName),mean - alpha1*width);
  if(left_low < left_high){ //is the left tail in range?
    if(fabs(n1-1.0)>1.e-5)
      left = A1/(-n1+1.0)*width*(pow(B1-(left_low-mean)/width,-n1+1.)-pow(B1-(left_high-mean)/width,-n1+1.));
    else
      left = A1*width*(log(B1-(left_low-mean)/width) - log(B1-(left_high-mean)/width) );
  }
 
  //compute right tail;
  double A2 = pow(n2/fabs(alpha2),n2)*exp(-alpha2*alpha2/2);
  double B2 = n2/fabs(alpha2)-fabs(alpha2);
 
  double right_low=max(x.min(rangeName),mean + alpha2*width);
  double right_high=x.max(rangeName);
  if(right_low < right_high){ //is the right tail in range?
    if(fabs(n2-1.0)>1.e-5)
      right = A2/(-n2+1.0)*width*(pow(B2+(right_high-mean)/width,-n2+1.)-pow(B2+(right_low-mean)/width,-n2+1.));
    else
      right = A2*width*(log(B2+(right_high-mean)/width) - log(B2+(right_low-mean)/width) );
  }
     
  return left+central+right;
 
};


//-----------------------
//2015 interpolation EBEB
//-----------------------

ClassImp(RooIntepolateDSCB_W0p014_Spin0_EBEB)

RooIntepolateDSCB_W0p014_Spin0_EBEB::RooIntepolateDSCB_W0p014_Spin0_EBEB( ){ };

RooIntepolateDSCB_W0p014_Spin0_EBEB::RooIntepolateDSCB_W0p014_Spin0_EBEB(const char *name, const char *title, 
					       RooAbsReal& _x,
					       RooAbsReal& _mass
					       ) :
  RooAbsPdf(name,title), 
  x("x","x",this,_x),
  mass("mass","mass",this, _mass)
{
};


RooIntepolateDSCB_W0p014_Spin0_EBEB::RooIntepolateDSCB_W0p014_Spin0_EBEB(const RooIntepolateDSCB_W0p014_Spin0_EBEB& other, const char* name) :  
  RooAbsPdf(other,name), 
  x("x",this,other.x),
  mass("mass",this, other.mass)
{ 
};

double RooIntepolateDSCB_W0p014_Spin0_EBEB::evaluate() const 
{
  double mean   = getMean( mass );
  double width  = getSigma( mass );
  double n1     = getN1( mass );
  double alpha1 = getAlpha1( mass );
  double n2     = getN2( mass );
  double alpha2 = getAlpha2( mass );
  
  double t = (x-mean)/width;
  if( t >= -alpha1 && t <= alpha2 )
    {
      return exp(-0.5*t*t);
    }
  else if ( t < -alpha1 )
    {
      double A1 = pow(n1/fabs(alpha1),n1)*exp(-alpha1*alpha1/2);
      double B1 = n1/fabs(alpha1)-fabs(alpha1);
      return A1*pow(B1-t,-n1);
    }
  else if ( t > alpha2 )
    {
      double A2 = pow(n2/fabs(alpha2),n2)*exp(-alpha2*alpha2/2);
      double B2 = n2/fabs(alpha2)-fabs(alpha2);
      return A2*pow(B2+t,-n2);
    }
  else
    {
      cout << "ERROR evaluating range... t = " << t << endl;
      return 99;
    }
   
};

Int_t RooIntepolateDSCB_W0p014_Spin0_EBEB::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* range) const 
{
  if (matchArgs(allVars,analVars,x)) return 1;
  return 0;
};

Double_t RooIntepolateDSCB_W0p014_Spin0_EBEB::analyticalIntegral(Int_t code, const char* rangeName) const 
{
  assert(code==1) ;
  //
  double mean   = getMean( mass );
  double width  = getSigma( mass );
  double n1     = getN1( mass );
  double alpha1 = getAlpha1( mass );
  double n2     = getN2( mass );
  double alpha2 = getAlpha2( mass );
  //
  
  double central=0;
  double left=0;
  double right=0;
 
  static const Double_t root2 = sqrt(2) ;
  static const Double_t rootPiBy2 = sqrt(atan2(0.0,-1.0)/2.0);
  Double_t xscale = root2*width;
 
  //compute gaussian contribution
  double central_low =max(x.min(rangeName),mean - alpha1*width );
  double central_high=min(x.max(rangeName),mean + alpha2*width );
  if(central_low < central_high) // is the gaussian part in range?
    central = rootPiBy2*width*(TMath::Erf((central_high-mean)/xscale)-TMath::Erf((central_low-mean)/xscale));
 
  //compute left tail;
  double A1 = pow(n1/fabs(alpha1),n1)*exp(-alpha1*alpha1/2);
  double B1 = n1/fabs(alpha1)-fabs(alpha1);
 
  double left_low=x.min(rangeName);
  double left_high=min(x.max(rangeName),mean - alpha1*width);
  if(left_low < left_high){ //is the left tail in range?
    if(fabs(n1-1.0)>1.e-5)
      left = A1/(-n1+1.0)*width*(pow(B1-(left_low-mean)/width,-n1+1.)-pow(B1-(left_high-mean)/width,-n1+1.));
    else
      left = A1*width*(log(B1-(left_low-mean)/width) - log(B1-(left_high-mean)/width) );
  }
 
  //compute right tail;
  double A2 = pow(n2/fabs(alpha2),n2)*exp(-alpha2*alpha2/2);
  double B2 = n2/fabs(alpha2)-fabs(alpha2);
 
  double right_low=max(x.min(rangeName),mean + alpha2*width);
  double right_high=x.max(rangeName);
  if(right_low < right_high){ //is the right tail in range?
    if(fabs(n2-1.0)>1.e-5)
      right = A2/(-n2+1.0)*width*(pow(B2+(right_high-mean)/width,-n2+1.)-pow(B2+(right_low-mean)/width,-n2+1.));
    else
      right = A2*width*(log(B2+(right_high-mean)/width) - log(B2+(right_low-mean)/width) );
  }
     
  return left+central+right;
 
};

double RooIntepolateDSCB_W0p014_Spin0_EBEB::getMean( double m ) const
{
	if( m >=  500 && m < 740 ) return 0.994216*(m-500) + 499.14;
	if( m >=  740 && m < 745 ) return 0.9984*(m-740) + 737.752;
	if( m >=  745 && m < 750 ) return 0.957734*(m-745) + 742.744;
	if( m >=  750 && m < 755 ) return 0.999629*(m-750) + 747.532;
	if( m >=  755 && m < 760 ) return 1.00813*(m-755) + 752.53;
	if( m >=  760 && m < 1000 ) return 0.991179*(m-760) + 757.571;
	if( m >=  1000 && m < 1250 ) return 0.988158*(m-1000) + 995.454;
	if( m >=  1250 && m < 1500 ) return 0.989073*(m-1250) + 1242.49;
	if( m >=  1500 && m < 1750 ) return 0.988192*(m-1500) + 1489.76;
	if( m >=  1750 && m < 2000 ) return 0.987584*(m-1750) + 1736.81;
	if( m >=  2000 && m < 2250 ) return 0.987793*(m-2000) + 1983.71;
	if( m >=  2250 && m < 2500 ) return 0.984663*(m-2250) + 2230.65;
	if( m >=  2500 && m < 2750 ) return 0.985442*(m-2500) + 2476.82;
	if( m >=  2750 && m < 3000 ) return 0.984061*(m-2750) + 2723.18;
	if( m >=  3000 && m < 3250 ) return 0.983365*(m-3000) + 2969.2;
	if( m >=  3250 && m < 3500 ) return 0.981574*(m-3250) + 3215.04;
	if( m >=  3500 && m < 3750 ) return 0.986228*(m-3500) + 3460.43;
	return 0;
};

double RooIntepolateDSCB_W0p014_Spin0_EBEB::getSigma( double m ) const
{
	if( m >=  500 && m < 740 ) return 0.00905269*(m-500) + 4.57404;
	if( m >=  740 && m < 745 ) return 0.0316008*(m-740) + 6.74669;
	if( m >=  745 && m < 750 ) return 0.0012557*(m-745) + 6.9047;
	if( m >=  750 && m < 755 ) return 0.036069*(m-750) + 6.91097;
	if( m >=  755 && m < 760 ) return -0.000875217*(m-755) + 7.09132;
	if( m >=  760 && m < 1000 ) return 0.00998901*(m-760) + 7.08694;
	if( m >=  1000 && m < 1250 ) return 0.00949227*(m-1000) + 9.48431;
	if( m >=  1250 && m < 1500 ) return 0.010308*(m-1250) + 11.8574;
	if( m >=  1500 && m < 1750 ) return 0.00858579*(m-1500) + 14.4344;
	if( m >=  1750 && m < 2000 ) return 0.0105061*(m-1750) + 16.5808;
	if( m >=  2000 && m < 2250 ) return 0.0109698*(m-2000) + 19.2073;
	if( m >=  2250 && m < 2500 ) return 0.0118582*(m-2250) + 21.9498;
	if( m >=  2500 && m < 2750 ) return 0.0112551*(m-2500) + 24.9144;
	if( m >=  2750 && m < 3000 ) return 0.0107435*(m-2750) + 27.7281;
	if( m >=  3000 && m < 3250 ) return 0.00927897*(m-3000) + 30.414;
	if( m >=  3250 && m < 3500 ) return 0.0148497*(m-3250) + 32.7337;
	if( m >=  3500 && m < 3750 ) return 0.00814216*(m-3500) + 36.4462;
	return 0;
};

double RooIntepolateDSCB_W0p014_Spin0_EBEB::getN1( double m ) const
{
	if( m >=  500 && m < 740 ) return -0.000396992*(m-500) + 2.7163;
	if( m >=  740 && m < 745 ) return 0.00962813*(m-740) + 2.62103;
	if( m >=  745 && m < 750 ) return 0.000376827*(m-745) + 2.66917;
	if( m >=  750 && m < 755 ) return -0.0373941*(m-750) + 2.67105;
	if( m >=  755 && m < 760 ) return 0.0365451*(m-755) + 2.48408;
	if( m >=  760 && m < 1000 ) return 0.000108846*(m-760) + 2.66681;
	if( m >=  1000 && m < 1250 ) return 0.000392984*(m-1000) + 2.69293;
	if( m >=  1250 && m < 1500 ) return -0.000112652*(m-1250) + 2.79117;
	if( m >=  1500 && m < 1750 ) return 0.00114139*(m-1500) + 2.76301;
	if( m >=  1750 && m < 2000 ) return 0.000137985*(m-1750) + 3.04836;
	if( m >=  2000 && m < 2250 ) return 0.000378671*(m-2000) + 3.08286;
	if( m >=  2250 && m < 2500 ) return 4.10528e-05*(m-2250) + 3.17752;
	if( m >=  2500 && m < 2750 ) return 0.00110905*(m-2500) + 3.18779;
	if( m >=  2750 && m < 3000 ) return -0.000722474*(m-2750) + 3.46505;
	if( m >=  3000 && m < 3250 ) return 0.00093738*(m-3000) + 3.28443;
	if( m >=  3250 && m < 3500 ) return -0.000318732*(m-3250) + 3.51878;
	if( m >=  3500 && m < 3750 ) return 0.000374807*(m-3500) + 3.43909;
	return 0;
};

double RooIntepolateDSCB_W0p014_Spin0_EBEB::getN2( double m ) const
{
	if( m >=  500 && m < 740 ) return 0.00330295*(m-500) + 4.31818;
	if( m >=  740 && m < 745 ) return -0.104413*(m-740) + 5.11089;
	if( m >=  745 && m < 750 ) return -0.135634*(m-745) + 4.58883;
	if( m >=  750 && m < 755 ) return 0.0299102*(m-750) + 3.91066;
	if( m >=  755 && m < 760 ) return 0.0254858*(m-755) + 4.06021;
	if( m >=  760 && m < 1000 ) return 3.41912e-05*(m-760) + 4.18764;
	if( m >=  1000 && m < 1250 ) return 0.00157148*(m-1000) + 4.19585;
	if( m >=  1250 && m < 1500 ) return -0.00405596*(m-1250) + 4.58872;
	if( m >=  1500 && m < 1750 ) return 0.00420385*(m-1500) + 3.57473;
	if( m >=  1750 && m < 2000 ) return -0.00327187*(m-1750) + 4.62569;
	if( m >=  2000 && m < 2250 ) return 0.000327924*(m-2000) + 3.80772;
	if( m >=  2250 && m < 2500 ) return -0.000160426*(m-2250) + 3.8897;
	if( m >=  2500 && m < 2750 ) return 0.000919424*(m-2500) + 3.84959;
	if( m >=  2750 && m < 3000 ) return 0.00138577*(m-2750) + 4.07945;
	if( m >=  3000 && m < 3250 ) return 0.00847361*(m-3000) + 4.42589;
	if( m >=  3250 && m < 3500 ) return 0.00705364*(m-3250) + 6.54429;
	if( m >=  3500 && m < 3750 ) return -0.010717*(m-3500) + 8.3077;
	return 0;
};

double RooIntepolateDSCB_W0p014_Spin0_EBEB::getAlpha1( double m ) const
{
	if( m >=  500 && m < 740 ) return 0.000169805*(m-500) + 1.26057;
	if( m >=  740 && m < 745 ) return 0.00496956*(m-740) + 1.30133;
	if( m >=  745 && m < 750 ) return -0.00265817*(m-745) + 1.32617;
	if( m >=  750 && m < 755 ) return 0.0108886*(m-750) + 1.31288;
	if( m >=  755 && m < 760 ) return -0.0062598*(m-755) + 1.36733;
	if( m >=  760 && m < 1000 ) return -7.45965e-05*(m-760) + 1.33603;
	if( m >=  1000 && m < 1250 ) return -4.82067e-05*(m-1000) + 1.31812;
	if( m >=  1250 && m < 1500 ) return 7.51587e-05*(m-1250) + 1.30607;
	if( m >=  1500 && m < 1750 ) return -0.000301287*(m-1500) + 1.32486;
	if( m >=  1750 && m < 2000 ) return -4.0742e-06*(m-1750) + 1.24954;
	if( m >=  2000 && m < 2250 ) return -9.45065e-05*(m-2000) + 1.24852;
	if( m >=  2250 && m < 2500 ) return 1.43727e-05*(m-2250) + 1.2249;
	if( m >=  2500 && m < 2750 ) return -0.000247518*(m-2500) + 1.22849;
	if( m >=  2750 && m < 3000 ) return 3.70953e-05*(m-2750) + 1.16661;
	if( m >=  3000 && m < 3250 ) return -0.000188293*(m-3000) + 1.17588;
	if( m >=  3250 && m < 3500 ) return -7.36893e-05*(m-3250) + 1.12881;
	if( m >=  3500 && m < 3750 ) return -0.000275004*(m-3500) + 1.11039;
	return 0;
};

double RooIntepolateDSCB_W0p014_Spin0_EBEB::getAlpha2( double m ) const
{
	if( m >=  500 && m < 740 ) return 4.60073e-06*(m-500) + 1.79471;
	if( m >=  740 && m < 745 ) return 0.0167923*(m-740) + 1.79582;
	if( m >=  745 && m < 750 ) return 0.0147963*(m-745) + 1.87978;
	if( m >=  750 && m < 755 ) return 0.000476421*(m-750) + 1.95376;
	if( m >=  755 && m < 760 ) return 0.00351346*(m-755) + 1.95614;
	if( m >=  760 && m < 1000 ) return 0.00022489*(m-760) + 1.97371;
	if( m >=  1000 && m < 1250 ) return -0.000183764*(m-1000) + 2.02768;
	if( m >=  1250 && m < 1500 ) return 0.000937109*(m-1250) + 1.98174;
	if( m >=  1500 && m < 1750 ) return -0.000672106*(m-1500) + 2.21602;
	if( m >=  1750 && m < 2000 ) return 0.000495366*(m-1750) + 2.04799;
	if( m >=  2000 && m < 2250 ) return 0.000678331*(m-2000) + 2.17184;
	if( m >=  2250 && m < 2500 ) return -0.000298686*(m-2250) + 2.34142;
	if( m >=  2500 && m < 2750 ) return 0.00036343*(m-2500) + 2.26675;
	if( m >=  2750 && m < 3000 ) return -0.00024248*(m-2750) + 2.3576;
	if( m >=  3000 && m < 3250 ) return -0.000651556*(m-3000) + 2.29698;
	if( m >=  3250 && m < 3500 ) return -0.000152253*(m-3250) + 2.1341;
	if( m >=  3500 && m < 3750 ) return 0.000224401*(m-3500) + 2.09603;
	return 0;
};

//-----------------------
//2016 interpolation EBEB
//-----------------------

ClassImp(RooIntepolateDSCB_W0p014_Spin0_EBEB_2016)

RooIntepolateDSCB_W0p014_Spin0_EBEB_2016::RooIntepolateDSCB_W0p014_Spin0_EBEB_2016( ){ };

RooIntepolateDSCB_W0p014_Spin0_EBEB_2016::RooIntepolateDSCB_W0p014_Spin0_EBEB_2016(const char *name, const char *title, 
					       RooAbsReal& _x,
					       RooAbsReal& _mass
					       ) :
  RooAbsPdf(name,title), 
  x("x","x",this,_x),
  mass("mass","mass",this, _mass)
{
};


RooIntepolateDSCB_W0p014_Spin0_EBEB_2016::RooIntepolateDSCB_W0p014_Spin0_EBEB_2016(const RooIntepolateDSCB_W0p014_Spin0_EBEB_2016& other, const char* name) :  
  RooAbsPdf(other,name), 
  x("x",this,other.x),
  mass("mass",this, other.mass)
{ 
};

double RooIntepolateDSCB_W0p014_Spin0_EBEB_2016::evaluate() const 
{
  double mean   = getMean( mass );
  double width  = getSigma( mass );
  double n1     = getN1( mass );
  double alpha1 = getAlpha1( mass );
  double n2     = getN2( mass );
  double alpha2 = getAlpha2( mass );
  
  double t = (x-mean)/width;
  if( t >= -alpha1 && t <= alpha2 )
    {
      return exp(-0.5*t*t);
    }
  else if ( t < -alpha1 )
    {
      double A1 = pow(n1/fabs(alpha1),n1)*exp(-alpha1*alpha1/2);
      double B1 = n1/fabs(alpha1)-fabs(alpha1);
      return A1*pow(B1-t,-n1);
    }
  else if ( t > alpha2 )
    {
      double A2 = pow(n2/fabs(alpha2),n2)*exp(-alpha2*alpha2/2);
      double B2 = n2/fabs(alpha2)-fabs(alpha2);
      return A2*pow(B2+t,-n2);
    }
  else
    {
      cout << "ERROR evaluating range... t = " << t << endl;
      return 99;
    }
   
};

Int_t RooIntepolateDSCB_W0p014_Spin0_EBEB_2016::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* range) const 
{
  if (matchArgs(allVars,analVars,x)) return 1;
  return 0;
};

Double_t RooIntepolateDSCB_W0p014_Spin0_EBEB_2016::analyticalIntegral(Int_t code, const char* rangeName) const 
{
  assert(code==1) ;
  //
  double mean   = getMean( mass );
  double width  = getSigma( mass );
  double n1     = getN1( mass );
  double alpha1 = getAlpha1( mass );
  double n2     = getN2( mass );
  double alpha2 = getAlpha2( mass );
  //
  
  double central=0;
  double left=0;
  double right=0;
 
  static const Double_t root2 = sqrt(2) ;
  static const Double_t rootPiBy2 = sqrt(atan2(0.0,-1.0)/2.0);
  Double_t xscale = root2*width;
 
  //compute gaussian contribution
  double central_low =max(x.min(rangeName),mean - alpha1*width );
  double central_high=min(x.max(rangeName),mean + alpha2*width );
  if(central_low < central_high) // is the gaussian part in range?
    central = rootPiBy2*width*(TMath::Erf((central_high-mean)/xscale)-TMath::Erf((central_low-mean)/xscale));
 
  //compute left tail;
  double A1 = pow(n1/fabs(alpha1),n1)*exp(-alpha1*alpha1/2);
  double B1 = n1/fabs(alpha1)-fabs(alpha1);
 
  double left_low=x.min(rangeName);
  double left_high=min(x.max(rangeName),mean - alpha1*width);
  if(left_low < left_high){ //is the left tail in range?
    if(fabs(n1-1.0)>1.e-5)
      left = A1/(-n1+1.0)*width*(pow(B1-(left_low-mean)/width,-n1+1.)-pow(B1-(left_high-mean)/width,-n1+1.));
    else
      left = A1*width*(log(B1-(left_low-mean)/width) - log(B1-(left_high-mean)/width) );
  }
 
  //compute right tail;
  double A2 = pow(n2/fabs(alpha2),n2)*exp(-alpha2*alpha2/2);
  double B2 = n2/fabs(alpha2)-fabs(alpha2);
 
  double right_low=max(x.min(rangeName),mean + alpha2*width);
  double right_high=x.max(rangeName);
  if(right_low < right_high){ //is the right tail in range?
    if(fabs(n2-1.0)>1.e-5)
      right = A2/(-n2+1.0)*width*(pow(B2+(right_high-mean)/width,-n2+1.)-pow(B2+(right_low-mean)/width,-n2+1.));
    else
      right = A2*width*(log(B2+(right_high-mean)/width) - log(B2+(right_low-mean)/width) );
  }
     
  return left+central+right;
 
};

double RooIntepolateDSCB_W0p014_Spin0_EBEB_2016::getMean( double m ) const
{
  if( m >=  500 && m < 740 ) return 0.993877*(m-500) + 499.094;
  if( m >=  740 && m < 745 ) return 1.00041*(m-740) + 737.624;
  if( m >=  745 && m < 750 ) return 0.970229*(m-745) + 742.626;
  if( m >=  750 && m < 755 ) return 1.01314*(m-750) + 747.478;
  if( m >=  755 && m < 760 ) return 1.00734*(m-755) + 752.543;
  if( m >=  760 && m < 1000 ) return 0.990714*(m-760) + 757.58;
  if( m >=  1000 && m < 1250 ) return 0.988488*(m-1000) + 995.351;
  if( m >=  1250 && m < 1500 ) return 0.988805*(m-1250) + 1242.47;
  if( m >=  1500 && m < 1750 ) return 0.986991*(m-1500) + 1489.67;
  if( m >=  1750 && m < 2000 ) return 0.987303*(m-1750) + 1736.42;
  if( m >=  2000 && m < 2250 ) return 0.987939*(m-2000) + 1983.25;
  if( m >=  2250 && m < 2500 ) return 0.985724*(m-2250) + 2230.23;
  if( m >=  2500 && m < 2750 ) return 0.985432*(m-2500) + 2476.66;
  if( m >=  2750 && m < 3000 ) return 0.984363*(m-2750) + 2723.02;
  if( m >=  3000 && m < 3250 ) return 0.980199*(m-3000) + 2969.11;
  if( m >=  3250 && m < 3500 ) return 0.981931*(m-3250) + 3214.16;
  if( m >=  3500 && m < 3750 ) return 0.982365*(m-3500) + 3459.65;
  return 0;
};

double RooIntepolateDSCB_W0p014_Spin0_EBEB_2016::getSigma( double m ) const
{
  if( m >=  500 && m < 740 ) return 0.00921867*(m-500) + 4.51071;
  if( m >=  740 && m < 745 ) return -0.0226585*(m-740) + 6.72319;
  if( m >=  745 && m < 750 ) return 0.0430507*(m-745) + 6.6099;
  if( m >=  750 && m < 755 ) return 0.023515*(m-750) + 6.82515;
  if( m >=  755 && m < 760 ) return -0.00850728*(m-755) + 6.94273;
  if( m >=  760 && m < 1000 ) return 0.00917394*(m-760) + 6.90019;
  if( m >=  1000 && m < 1250 ) return 0.00982108*(m-1000) + 9.10194;
  if( m >=  1250 && m < 1500 ) return 0.00908806*(m-1250) + 11.5572;
  if( m >=  1500 && m < 1750 ) return 0.00941604*(m-1500) + 13.8292;
  if( m >=  1750 && m < 2000 ) return 0.00972389*(m-1750) + 16.1832;
  if( m >=  2000 && m < 2250 ) return 0.00834053*(m-2000) + 18.6142;
  if( m >=  2250 && m < 2500 ) return 0.0115764*(m-2250) + 20.6993;
  if( m >=  2500 && m < 2750 ) return 0.0102892*(m-2500) + 23.5934;
  if( m >=  2750 && m < 3000 ) return 0.00900137*(m-2750) + 26.1657;
  if( m >=  3000 && m < 3250 ) return 0.0145019*(m-3000) + 28.4161;
  if( m >=  3250 && m < 3500 ) return 0.0086967*(m-3250) + 32.0415;
  if( m >=  3500 && m < 3750 ) return 0.0136304*(m-3500) + 34.2157;
  return 0;
};

double RooIntepolateDSCB_W0p014_Spin0_EBEB_2016::getN1( double m ) const
{
  if( m >=  500 && m < 740 ) return -0.000937557*(m-500) + 2.79148;
  if( m >=  740 && m < 745 ) return 0.0371158*(m-740) + 2.56647;
  if( m >=  745 && m < 750 ) return -0.00646286*(m-745) + 2.75205;
  if( m >=  750 && m < 755 ) return -0.0547692*(m-750) + 2.71973;
  if( m >=  755 && m < 760 ) return 0.0613787*(m-755) + 2.44589;
  if( m >=  760 && m < 1000 ) return 0.00024581*(m-760) + 2.75278;
  if( m >=  1000 && m < 1250 ) return 0.000421671*(m-1000) + 2.81177;
  if( m >=  1250 && m < 1500 ) return 0.000192798*(m-1250) + 2.91719;
  if( m >=  1500 && m < 1750 ) return 0.000446433*(m-1500) + 2.96539;
  if( m >=  1750 && m < 2000 ) return -8.56581e-05*(m-1750) + 3.077;
  if( m >=  2000 && m < 2250 ) return 0.00114165*(m-2000) + 3.05559;
  if( m >=  2250 && m < 2500 ) return 0.000463084*(m-2250) + 3.341;
  if( m >=  2500 && m < 2750 ) return 0.000888811*(m-2500) + 3.45677;
  if( m >=  2750 && m < 3000 ) return -0.000397485*(m-2750) + 3.67897;
  if( m >=  3000 && m < 3250 ) return -5.5071e-06*(m-3000) + 3.5796;
  if( m >=  3250 && m < 3500 ) return -0.000334312*(m-3250) + 3.57822;
  if( m >=  3500 && m < 3750 ) return -0.000198104*(m-3500) + 3.49465;
  return 0;
};

double RooIntepolateDSCB_W0p014_Spin0_EBEB_2016::getN2( double m ) const
{
  if( m >=  500 && m < 740 ) return -0.00135576*(m-500) + 4.17425;
  if( m >=  740 && m < 745 ) return -0.00850094*(m-740) + 3.84887;
  if( m >=  745 && m < 750 ) return -0.113298*(m-745) + 3.80636;
  if( m >=  750 && m < 755 ) return 0.190869*(m-750) + 3.23987;
  if( m >=  755 && m < 760 ) return -0.0544422*(m-755) + 4.19422;
  if( m >=  760 && m < 1000 ) return -0.000131072*(m-760) + 3.92201;
  if( m >=  1000 && m < 1250 ) return -0.000915535*(m-1000) + 3.89055;
  if( m >=  1250 && m < 1500 ) return -0.000983521*(m-1250) + 3.66167;
  if( m >=  1500 && m < 1750 ) return 0.00144712*(m-1500) + 3.41579;
  if( m >=  1750 && m < 2000 ) return -0.000912781*(m-1750) + 3.77757;
  if( m >=  2000 && m < 2250 ) return 0.00326423*(m-2000) + 3.54937;
  if( m >=  2250 && m < 2500 ) return -0.00316202*(m-2250) + 4.36543;
  if( m >=  2500 && m < 2750 ) return -0.000674945*(m-2500) + 3.57493;
  if( m >=  2750 && m < 3000 ) return 0.00548049*(m-2750) + 3.40619;
  if( m >=  3000 && m < 3250 ) return -0.00151318*(m-3000) + 4.77631;
  if( m >=  3250 && m < 3500 ) return -0.00238002*(m-3250) + 4.39802;
  if( m >=  3500 && m < 3750 ) return 0.00176116*(m-3500) + 3.80301;
  return 0;
};

double RooIntepolateDSCB_W0p014_Spin0_EBEB_2016::getAlpha1( double m ) const
{
  if( m >=  500 && m < 740 ) return 0.000414442*(m-500) + 1.32088;
  if( m >=  740 && m < 745 ) return -0.0154094*(m-740) + 1.42035;
  if( m >=  745 && m < 750 ) return 0.00836468*(m-745) + 1.3433;
  if( m >=  750 && m < 755 ) return 0.0113587*(m-750) + 1.38513;
  if( m >=  755 && m < 760 ) return -0.0156081*(m-755) + 1.44192;
  if( m >=  760 && m < 1000 ) return -9.2941e-05*(m-760) + 1.36388;
  if( m >=  1000 && m < 1250 ) return -2.01925e-05*(m-1000) + 1.34157;
  if( m >=  1250 && m < 1500 ) return -0.000134425*(m-1250) + 1.33653;
  if( m >=  1500 && m < 1750 ) return -8.81417e-05*(m-1500) + 1.30292;
  if( m >=  1750 && m < 2000 ) return 5.42138e-05*(m-1750) + 1.28088;
  if( m >=  2000 && m < 2250 ) return -0.00035673*(m-2000) + 1.29444;
  if( m >=  2250 && m < 2500 ) return -5.95231e-05*(m-2250) + 1.20526;
  if( m >=  2500 && m < 2750 ) return -0.000253634*(m-2500) + 1.19037;
  if( m >=  2750 && m < 3000 ) return -4.29077e-05*(m-2750) + 1.12697;
  if( m >=  3000 && m < 3250 ) return 0.000136791*(m-3000) + 1.11624;
  if( m >=  3250 && m < 3500 ) return -0.000160626*(m-3250) + 1.15044;
  if( m >=  3500 && m < 3750 ) return -3.94266e-05*(m-3500) + 1.11028;
  return 0;
};

double RooIntepolateDSCB_W0p014_Spin0_EBEB_2016::getAlpha2( double m ) const
{
  if( m >=  500 && m < 740 ) return 0.000483547*(m-500) + 1.76561;
  if( m >=  740 && m < 745 ) return -0.00739828*(m-740) + 1.88166;
  if( m >=  745 && m < 750 ) return 0.0343372*(m-745) + 1.84467;
  if( m >=  750 && m < 755 ) return -0.0252737*(m-750) + 2.01636;
  if( m >=  755 && m < 760 ) return 0.00793363*(m-755) + 1.88999;
  if( m >=  760 && m < 1000 ) return 0.000286565*(m-760) + 1.92966;
  if( m >=  1000 && m < 1250 ) return 0.000245411*(m-1000) + 1.99843;
  if( m >=  1250 && m < 1500 ) return 0.000513356*(m-1250) + 2.05979;
  if( m >=  1500 && m < 1750 ) return -0.000114779*(m-1500) + 2.18813;
  if( m >=  1750 && m < 2000 ) return 0.000216371*(m-1750) + 2.15943;
  if( m >=  2000 && m < 2250 ) return -0.000289605*(m-2000) + 2.21352;
  if( m >=  2250 && m < 2500 ) return 0.000584429*(m-2250) + 2.14112;
  if( m >=  2500 && m < 2750 ) return 0.000441657*(m-2500) + 2.28723;
  if( m >=  2750 && m < 3000 ) return -0.00057106*(m-2750) + 2.39764;
  if( m >=  3000 && m < 3250 ) return 0.000648816*(m-3000) + 2.25488;
  if( m >=  3250 && m < 3500 ) return -4.7339e-05*(m-3250) + 2.41708;
  if( m >=  3500 && m < 3750 ) return -0.000124014*(m-3500) + 2.40525;
  return 0;
};

//----------------------------
//2015 interpolation EBEE
//----------------------------

ClassImp(RooIntepolateDSCB_W0p014_Spin0_EBEE)

RooIntepolateDSCB_W0p014_Spin0_EBEE::RooIntepolateDSCB_W0p014_Spin0_EBEE( ){ };

RooIntepolateDSCB_W0p014_Spin0_EBEE::RooIntepolateDSCB_W0p014_Spin0_EBEE(const char *name, const char *title, 
					       RooAbsReal& _x,
					       RooAbsReal& _mass
					       ) :
  RooAbsPdf(name,title), 
  x("x","x",this,_x),
  mass("mass","mass",this, _mass)
{
};


RooIntepolateDSCB_W0p014_Spin0_EBEE::RooIntepolateDSCB_W0p014_Spin0_EBEE(const RooIntepolateDSCB_W0p014_Spin0_EBEE& other, const char* name) :  
  RooAbsPdf(other,name), 
  x("x",this,other.x),
  mass("mass",this, other.mass)
{ 
};

double RooIntepolateDSCB_W0p014_Spin0_EBEE::evaluate() const 
{
  double mean   = getMean( mass );
  double width  = getSigma( mass );
  double n1     = getN1( mass );
  double alpha1 = getAlpha1( mass );
  double n2     = getN2( mass );
  double alpha2 = getAlpha2( mass );
  
  double t = (x-mean)/width;
  if( t >= -alpha1 && t <= alpha2 )
    {
      return exp(-0.5*t*t);
    }
  else if ( t < -alpha1 )
    {
      double A1 = pow(n1/fabs(alpha1),n1)*exp(-alpha1*alpha1/2);
      double B1 = n1/fabs(alpha1)-fabs(alpha1);
      return A1*pow(B1-t,-n1);
    }
  else if ( t > alpha2 )
    {
      double A2 = pow(n2/fabs(alpha2),n2)*exp(-alpha2*alpha2/2);
      double B2 = n2/fabs(alpha2)-fabs(alpha2);
      return A2*pow(B2+t,-n2);
    }
  else
    {
      cout << "ERROR evaluating range... t = " << t << endl;
      return 99;
    }
   
};

Int_t RooIntepolateDSCB_W0p014_Spin0_EBEE::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* range) const 
{
  if (matchArgs(allVars,analVars,x)) return 1;
  return 0;
};

Double_t RooIntepolateDSCB_W0p014_Spin0_EBEE::analyticalIntegral(Int_t code, const char* rangeName) const 
{
  assert(code==1) ;
  //
  double mean   = getMean( mass );
  double width  = getSigma( mass );
  double n1     = getN1( mass );
  double alpha1 = getAlpha1( mass );
  double n2     = getN2( mass );
  double alpha2 = getAlpha2( mass );
  //
  
  double central=0;
  double left=0;
  double right=0;
 
  static const Double_t root2 = sqrt(2) ;
  static const Double_t rootPiBy2 = sqrt(atan2(0.0,-1.0)/2.0);
  Double_t xscale = root2*width;
 
  //compute gaussian contribution
  double central_low =max(x.min(rangeName),mean - alpha1*width );
  double central_high=min(x.max(rangeName),mean + alpha2*width );
  if(central_low < central_high) // is the gaussian part in range?
    central = rootPiBy2*width*(TMath::Erf((central_high-mean)/xscale)-TMath::Erf((central_low-mean)/xscale));
 
  //compute left tail;
  double A1 = pow(n1/fabs(alpha1),n1)*exp(-alpha1*alpha1/2);
  double B1 = n1/fabs(alpha1)-fabs(alpha1);
 
  double left_low=x.min(rangeName);
  double left_high=min(x.max(rangeName),mean - alpha1*width);
  if(left_low < left_high){ //is the left tail in range?
    if(fabs(n1-1.0)>1.e-5)
      left = A1/(-n1+1.0)*width*(pow(B1-(left_low-mean)/width,-n1+1.)-pow(B1-(left_high-mean)/width,-n1+1.));
    else
      left = A1*width*(log(B1-(left_low-mean)/width) - log(B1-(left_high-mean)/width) );
  }
 
  //compute right tail;
  double A2 = pow(n2/fabs(alpha2),n2)*exp(-alpha2*alpha2/2);
  double B2 = n2/fabs(alpha2)-fabs(alpha2);
 
  double right_low=max(x.min(rangeName),mean + alpha2*width);
  double right_high=x.max(rangeName);
  if(right_low < right_high){ //is the right tail in range?
    if(fabs(n2-1.0)>1.e-5)
      right = A2/(-n2+1.0)*width*(pow(B2+(right_high-mean)/width,-n2+1.)-pow(B2+(right_low-mean)/width,-n2+1.));
    else
      right = A2*width*(log(B2+(right_high-mean)/width) - log(B2+(right_low-mean)/width) );
  }
     
  return left+central+right;
 
};

double RooIntepolateDSCB_W0p014_Spin0_EBEE::getMean( double m ) const
{
	if( m >=  500 && m < 740 ) return 0.995322*(m-500) + 499.129;
	if( m >=  740 && m < 745 ) return 0.940496*(m-740) + 738.007;
	if( m >=  745 && m < 750 ) return 1.05776*(m-745) + 742.709;
	if( m >=  750 && m < 755 ) return 0.950917*(m-750) + 747.998;
	if( m >=  755 && m < 760 ) return 1.02621*(m-755) + 752.752;
	if( m >=  760 && m < 1000 ) return 0.993343*(m-760) + 757.884;
	if( m >=  1000 && m < 1250 ) return 0.992417*(m-1000) + 996.286;
	if( m >=  1250 && m < 1500 ) return 0.989511*(m-1250) + 1244.39;
	if( m >=  1500 && m < 1750 ) return 0.986369*(m-1500) + 1491.77;
	if( m >=  1750 && m < 2000 ) return 0.992526*(m-1750) + 1738.36;
	if( m >=  2000 && m < 2250 ) return 0.985941*(m-2000) + 1986.49;
	if( m >=  2250 && m < 2500 ) return 0.993692*(m-2250) + 2232.98;
	if( m >=  2500 && m < 2750 ) return 0.983566*(m-2500) + 2481.4;
	if( m >=  2750 && m < 3000 ) return 0.995891*(m-2750) + 2727.29;
	if( m >=  3000 && m < 3250 ) return 0.98611*(m-3000) + 2976.26;
	if( m >=  3250 && m < 3500 ) return 0.982006*(m-3250) + 3222.79;
	if( m >=  3500 && m < 3750 ) return 0.978914*(m-3500) + 3468.29;
	return 0;
};

double RooIntepolateDSCB_W0p014_Spin0_EBEE::getSigma( double m ) const
{
	if( m >=  500 && m < 740 ) return 0.0149796*(m-500) + 7.98483;
	if( m >=  740 && m < 745 ) return 0.051507*(m-740) + 11.5799;
	if( m >=  745 && m < 750 ) return -0.0703522*(m-745) + 11.8375;
	if( m >=  750 && m < 755 ) return 0.103891*(m-750) + 11.4857;
	if( m >=  755 && m < 760 ) return -0.0351852*(m-755) + 12.0052;
	if( m >=  760 && m < 1000 ) return 0.0169437*(m-760) + 11.8292;
	if( m >=  1000 && m < 1250 ) return 0.0156625*(m-1000) + 15.8957;
	if( m >=  1250 && m < 1500 ) return 0.0152305*(m-1250) + 19.8114;
	if( m >=  1500 && m < 1750 ) return 0.0182183*(m-1500) + 23.619;
	if( m >=  1750 && m < 2000 ) return 0.0144939*(m-1750) + 28.1736;
	if( m >=  2000 && m < 2250 ) return 0.021097*(m-2000) + 31.797;
	if( m >=  2250 && m < 2500 ) return 0.0136223*(m-2250) + 37.0713;
	if( m >=  2500 && m < 2750 ) return 0.0237272*(m-2500) + 40.4769;
	if( m >=  2750 && m < 3000 ) return 0.0117985*(m-2750) + 46.4087;
	if( m >=  3000 && m < 3250 ) return 0.0128546*(m-3000) + 49.3583;
	if( m >=  3250 && m < 3500 ) return 0.0277385*(m-3250) + 52.572;
	if( m >=  3500 && m < 3750 ) return 0.0140387*(m-3500) + 59.5066;
	return 0;
};

double RooIntepolateDSCB_W0p014_Spin0_EBEE::getN1( double m ) const
{
	if( m >=  500 && m < 740 ) return -0.00117175*(m-500) + 3.47673;
	if( m >=  740 && m < 745 ) return -0.077044*(m-740) + 3.19551;
	if( m >=  745 && m < 750 ) return 0.0098794*(m-745) + 2.81029;
	if( m >=  750 && m < 755 ) return -0.0483327*(m-750) + 2.85969;
	if( m >=  755 && m < 760 ) return 0.0918962*(m-755) + 2.61803;
	if( m >=  760 && m < 1000 ) return -0.00227564*(m-760) + 3.07751;
	if( m >=  1000 && m < 1250 ) return -0.000341539*(m-1000) + 2.53136;
	if( m >=  1250 && m < 1500 ) return -0.00081655*(m-1250) + 2.44597;
	if( m >=  1500 && m < 1750 ) return -0.00109114*(m-1500) + 2.24183;
	if( m >=  1750 && m < 2000 ) return 0.000477881*(m-1750) + 1.96905;
	if( m >=  2000 && m < 2250 ) return -0.000988361*(m-2000) + 2.08852;
	if( m >=  2250 && m < 2500 ) return -0.00120279*(m-2250) + 1.84143;
	if( m >=  2500 && m < 2750 ) return -0.00144645*(m-2500) + 1.54073;
	if( m >=  2750 && m < 3000 ) return 0.00106027*(m-2750) + 1.17912;
	if( m >=  3000 && m < 3250 ) return -0.0015138*(m-3000) + 1.44418;
	if( m >=  3250 && m < 3500 ) return -0.000633532*(m-3250) + 1.06573;
	if( m >=  3500 && m < 3750 ) return 0.000304564*(m-3500) + 0.907351;
	return 0;
};

double RooIntepolateDSCB_W0p014_Spin0_EBEE::getN2( double m ) const
{
	if( m >=  500 && m < 740 ) return 1.13113e-05*(m-500) + 2.88855;
	if( m >=  740 && m < 745 ) return 0.469553*(m-740) + 2.89126;
	if( m >=  745 && m < 750 ) return -0.00410109*(m-745) + 5.23903;
	if( m >=  750 && m < 755 ) return -0.481446*(m-750) + 5.21852;
	if( m >=  755 && m < 760 ) return 0.495757*(m-755) + 2.81129;
	if( m >=  760 && m < 1000 ) return -0.01125*(m-760) + 5.29008;
	if( m >=  1000 && m < 1250 ) return 0.00895198*(m-1000) + 2.59008;
	if( m >=  1250 && m < 1500 ) return -0.00880095*(m-1250) + 4.82807;
	if( m >=  1500 && m < 1750 ) return 0.0121763*(m-1500) + 2.62783;
	if( m >=  1750 && m < 2000 ) return 0.011549*(m-1750) + 5.6719;
	if( m >=  2000 && m < 2250 ) return 0.0103098*(m-2000) + 8.55915;
	if( m >=  2250 && m < 2500 ) return -0.0273186*(m-2250) + 11.1366;
	if( m >=  2500 && m < 2750 ) return 0.0165945*(m-2500) + 4.30695;
	if( m >=  2750 && m < 3000 ) return -0.0241867*(m-2750) + 8.45556;
	if( m >=  3000 && m < 3250 ) return 0.00204365*(m-3000) + 2.40888;
	if( m >=  3250 && m < 3500 ) return 0.00721212*(m-3250) + 2.9198;
	if( m >=  3500 && m < 3750 ) return -0.00582837*(m-3500) + 4.72283;
	return 0;
};

double RooIntepolateDSCB_W0p014_Spin0_EBEE::getAlpha1( double m ) const
{
	if( m >=  500 && m < 740 ) return 0.000128657*(m-500) + 1.54255;
	if( m >=  740 && m < 745 ) return 0.0118325*(m-740) + 1.57343;
	if( m >=  745 && m < 750 ) return -0.00776204*(m-745) + 1.63259;
	if( m >=  750 && m < 755 ) return 0.0247986*(m-750) + 1.59378;
	if( m >=  755 && m < 760 ) return -0.0236144*(m-755) + 1.71777;
	if( m >=  760 && m < 1000 ) return 0.0005031*(m-760) + 1.5997;
	if( m >=  1000 && m < 1250 ) return -2.87555e-05*(m-1000) + 1.72045;
	if( m >=  1250 && m < 1500 ) return 0.000354975*(m-1250) + 1.71326;
	if( m >=  1500 && m < 1750 ) return 0.000482002*(m-1500) + 1.802;
	if( m >=  1750 && m < 2000 ) return -0.000507001*(m-1750) + 1.9225;
	if( m >=  2000 && m < 2250 ) return 0.000759668*(m-2000) + 1.79575;
	if( m >=  2250 && m < 2500 ) return 5.72213e-05*(m-2250) + 1.98567;
	if( m >=  2500 && m < 2750 ) return 0.000904143*(m-2500) + 1.99997;
	if( m >=  2750 && m < 3000 ) return -0.00110344*(m-2750) + 2.22601;
	if( m >=  3000 && m < 3250 ) return 0.00079785*(m-3000) + 1.95015;
	if( m >=  3250 && m < 3500 ) return 0.000633135*(m-3250) + 2.14961;
	if( m >=  3500 && m < 3750 ) return -0.000525699*(m-3500) + 2.3079;
	return 0;
};

double RooIntepolateDSCB_W0p014_Spin0_EBEE::getAlpha2( double m ) const
{
	if( m >=  500 && m < 740 ) return 0.000592368*(m-500) + 2.35797;
	if( m >=  740 && m < 745 ) return -0.0530706*(m-740) + 2.50014;
	if( m >=  745 && m < 750 ) return 0.0058549*(m-745) + 2.23478;
	if( m >=  750 && m < 755 ) return 0.0720748*(m-750) + 2.26406;
	if( m >=  755 && m < 760 ) return -0.0587264*(m-755) + 2.62443;
	if( m >=  760 && m < 1000 ) return 0.00152393*(m-760) + 2.3308;
	if( m >=  1000 && m < 1250 ) return -0.00136587*(m-1000) + 2.69654;
	if( m >=  1250 && m < 1500 ) return 0.00296536*(m-1250) + 2.35508;
	if( m >=  1500 && m < 1750 ) return -0.000743122*(m-1500) + 3.09642;
	if( m >=  1750 && m < 2000 ) return -0.00232187*(m-1750) + 2.91063;
	if( m >=  2000 && m < 2250 ) return 0.0118391*(m-2000) + 2.33017;
	if( m >=  2250 && m < 2500 ) return -0.00918172*(m-2250) + 5.28994;
	if( m >=  2500 && m < 2750 ) return 0.00902744*(m-2500) + 2.99451;
	if( m >=  2750 && m < 3000 ) return -0.00938143*(m-2750) + 5.25137;
	if( m >=  3000 && m < 3250 ) return 0.000612331*(m-3000) + 2.90601;
	if( m >=  3250 && m < 3500 ) return 0.000960696*(m-3250) + 3.05909;
	if( m >=  3500 && m < 3750 ) return 7.79992e-05*(m-3500) + 3.29927;
	return 0;
};



//----------------------------
//2016 interpolation EBEE
//----------------------------

ClassImp(RooIntepolateDSCB_W0p014_Spin0_EBEE_2016)

RooIntepolateDSCB_W0p014_Spin0_EBEE_2016::RooIntepolateDSCB_W0p014_Spin0_EBEE_2016( ){ };

RooIntepolateDSCB_W0p014_Spin0_EBEE_2016::RooIntepolateDSCB_W0p014_Spin0_EBEE_2016(const char *name, const char *title, 
					       RooAbsReal& _x,
					       RooAbsReal& _mass
					       ) :
  RooAbsPdf(name,title), 
  x("x","x",this,_x),
  mass("mass","mass",this, _mass)
{
};


RooIntepolateDSCB_W0p014_Spin0_EBEE_2016::RooIntepolateDSCB_W0p014_Spin0_EBEE_2016(const RooIntepolateDSCB_W0p014_Spin0_EBEE_2016& other, const char* name) :  
  RooAbsPdf(other,name), 
  x("x",this,other.x),
  mass("mass",this, other.mass)
{ 
};

double RooIntepolateDSCB_W0p014_Spin0_EBEE_2016::evaluate() const 
{
  double mean   = getMean( mass );
  double width  = getSigma( mass );
  double n1     = getN1( mass );
  double alpha1 = getAlpha1( mass );
  double n2     = getN2( mass );
  double alpha2 = getAlpha2( mass );
  
  double t = (x-mean)/width;
  if( t >= -alpha1 && t <= alpha2 )
    {
      return exp(-0.5*t*t);
    }
  else if ( t < -alpha1 )
    {
      double A1 = pow(n1/fabs(alpha1),n1)*exp(-alpha1*alpha1/2);
      double B1 = n1/fabs(alpha1)-fabs(alpha1);
      return A1*pow(B1-t,-n1);
    }
  else if ( t > alpha2 )
    {
      double A2 = pow(n2/fabs(alpha2),n2)*exp(-alpha2*alpha2/2);
      double B2 = n2/fabs(alpha2)-fabs(alpha2);
      return A2*pow(B2+t,-n2);
    }
  else
    {
      cout << "ERROR evaluating range... t = " << t << endl;
      return 99;
    }
   
};

Int_t RooIntepolateDSCB_W0p014_Spin0_EBEE_2016::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* range) const 
{
  if (matchArgs(allVars,analVars,x)) return 1;
  return 0;
};

Double_t RooIntepolateDSCB_W0p014_Spin0_EBEE_2016::analyticalIntegral(Int_t code, const char* rangeName) const 
{
  assert(code==1) ;
  //
  double mean   = getMean( mass );
  double width  = getSigma( mass );
  double n1     = getN1( mass );
  double alpha1 = getAlpha1( mass );
  double n2     = getN2( mass );
  double alpha2 = getAlpha2( mass );
  //
  
  double central=0;
  double left=0;
  double right=0;
 
  static const Double_t root2 = sqrt(2) ;
  static const Double_t rootPiBy2 = sqrt(atan2(0.0,-1.0)/2.0);
  Double_t xscale = root2*width;
 
  //compute gaussian contribution
  double central_low =max(x.min(rangeName),mean - alpha1*width );
  double central_high=min(x.max(rangeName),mean + alpha2*width );
  if(central_low < central_high) // is the gaussian part in range?
    central = rootPiBy2*width*(TMath::Erf((central_high-mean)/xscale)-TMath::Erf((central_low-mean)/xscale));
 
  //compute left tail;
  double A1 = pow(n1/fabs(alpha1),n1)*exp(-alpha1*alpha1/2);
  double B1 = n1/fabs(alpha1)-fabs(alpha1);
 
  double left_low=x.min(rangeName);
  double left_high=min(x.max(rangeName),mean - alpha1*width);
  if(left_low < left_high){ //is the left tail in range?
    if(fabs(n1-1.0)>1.e-5)
      left = A1/(-n1+1.0)*width*(pow(B1-(left_low-mean)/width,-n1+1.)-pow(B1-(left_high-mean)/width,-n1+1.));
    else
      left = A1*width*(log(B1-(left_low-mean)/width) - log(B1-(left_high-mean)/width) );
  }
 
  //compute right tail;
  double A2 = pow(n2/fabs(alpha2),n2)*exp(-alpha2*alpha2/2);
  double B2 = n2/fabs(alpha2)-fabs(alpha2);
 
  double right_low=max(x.min(rangeName),mean + alpha2*width);
  double right_high=x.max(rangeName);
  if(right_low < right_high){ //is the right tail in range?
    if(fabs(n2-1.0)>1.e-5)
      right = A2/(-n2+1.0)*width*(pow(B2+(right_high-mean)/width,-n2+1.)-pow(B2+(right_low-mean)/width,-n2+1.));
    else
      right = A2*width*(log(B2+(right_high-mean)/width) - log(B2+(right_low-mean)/width) );
  }
     
  return left+central+right;
 
};

double RooIntepolateDSCB_W0p014_Spin0_EBEE_2016::getMean( double m ) const
{
  if( m >=  500 && m < 740 ) return 0.993877*(m-500) + 499.094;
  if( m >=  740 && m < 745 ) return 1.00041*(m-740) + 737.624;
  if( m >=  745 && m < 750 ) return 0.970229*(m-745) + 742.626;
  if( m >=  750 && m < 755 ) return 1.01314*(m-750) + 747.478;
  if( m >=  755 && m < 760 ) return 1.00734*(m-755) + 752.543;
  if( m >=  760 && m < 1000 ) return 0.990714*(m-760) + 757.58;
  if( m >=  1000 && m < 1250 ) return 0.988488*(m-1000) + 995.351;
  if( m >=  1250 && m < 1500 ) return 0.988805*(m-1250) + 1242.47;
  if( m >=  1500 && m < 1750 ) return 0.986991*(m-1500) + 1489.67;
  if( m >=  1750 && m < 2000 ) return 0.987303*(m-1750) + 1736.42;
  if( m >=  2000 && m < 2250 ) return 0.987939*(m-2000) + 1983.25;
  if( m >=  2250 && m < 2500 ) return 0.985724*(m-2250) + 2230.23;
  if( m >=  2500 && m < 2750 ) return 0.985432*(m-2500) + 2476.66;
  if( m >=  2750 && m < 3000 ) return 0.984363*(m-2750) + 2723.02;
  if( m >=  3000 && m < 3250 ) return 0.980199*(m-3000) + 2969.11;
  if( m >=  3250 && m < 3500 ) return 0.981931*(m-3250) + 3214.16;
  if( m >=  3500 && m < 3750 ) return 0.982365*(m-3500) + 3459.65;
  return 0;
};

double RooIntepolateDSCB_W0p014_Spin0_EBEE_2016::getSigma( double m ) const
{
  if( m >=  500 && m < 740 ) return 0.00921867*(m-500) + 4.51071;
  if( m >=  740 && m < 745 ) return -0.0226585*(m-740) + 6.72319;
  if( m >=  745 && m < 750 ) return 0.0430507*(m-745) + 6.6099;
  if( m >=  750 && m < 755 ) return 0.023515*(m-750) + 6.82515;
  if( m >=  755 && m < 760 ) return -0.00850728*(m-755) + 6.94273;
  if( m >=  760 && m < 1000 ) return 0.00917394*(m-760) + 6.90019;
  if( m >=  1000 && m < 1250 ) return 0.00982108*(m-1000) + 9.10194;
  if( m >=  1250 && m < 1500 ) return 0.00908806*(m-1250) + 11.5572;
  if( m >=  1500 && m < 1750 ) return 0.00941604*(m-1500) + 13.8292;
  if( m >=  1750 && m < 2000 ) return 0.00972389*(m-1750) + 16.1832;
  if( m >=  2000 && m < 2250 ) return 0.00834053*(m-2000) + 18.6142;
  if( m >=  2250 && m < 2500 ) return 0.0115764*(m-2250) + 20.6993;
  if( m >=  2500 && m < 2750 ) return 0.0102892*(m-2500) + 23.5934;
  if( m >=  2750 && m < 3000 ) return 0.00900137*(m-2750) + 26.1657;
  if( m >=  3000 && m < 3250 ) return 0.0145019*(m-3000) + 28.4161;
  if( m >=  3250 && m < 3500 ) return 0.0086967*(m-3250) + 32.0415;
  if( m >=  3500 && m < 3750 ) return 0.0136304*(m-3500) + 34.2157;
  return 0;
};

double RooIntepolateDSCB_W0p014_Spin0_EBEE_2016::getN1( double m ) const
{
  if( m >=  500 && m < 740 ) return -0.000937557*(m-500) + 2.79148;
  if( m >=  740 && m < 745 ) return 0.0371158*(m-740) + 2.56647;
  if( m >=  745 && m < 750 ) return -0.00646286*(m-745) + 2.75205;
  if( m >=  750 && m < 755 ) return -0.0547692*(m-750) + 2.71973;
  if( m >=  755 && m < 760 ) return 0.0613787*(m-755) + 2.44589;
  if( m >=  760 && m < 1000 ) return 0.00024581*(m-760) + 2.75278;
  if( m >=  1000 && m < 1250 ) return 0.000421671*(m-1000) + 2.81177;
  if( m >=  1250 && m < 1500 ) return 0.000192798*(m-1250) + 2.91719;
  if( m >=  1500 && m < 1750 ) return 0.000446433*(m-1500) + 2.96539;
  if( m >=  1750 && m < 2000 ) return -8.56581e-05*(m-1750) + 3.077;
  if( m >=  2000 && m < 2250 ) return 0.00114165*(m-2000) + 3.05559;
  if( m >=  2250 && m < 2500 ) return 0.000463084*(m-2250) + 3.341;
  if( m >=  2500 && m < 2750 ) return 0.000888811*(m-2500) + 3.45677;
  if( m >=  2750 && m < 3000 ) return -0.000397485*(m-2750) + 3.67897;
  if( m >=  3000 && m < 3250 ) return -5.5071e-06*(m-3000) + 3.5796;
  if( m >=  3250 && m < 3500 ) return -0.000334312*(m-3250) + 3.57822;
  if( m >=  3500 && m < 3750 ) return -0.000198104*(m-3500) + 3.49465;
  return 0;
};

double RooIntepolateDSCB_W0p014_Spin0_EBEE_2016::getN2( double m ) const
{
  if( m >=  500 && m < 740 ) return -0.00135576*(m-500) + 4.17425;
  if( m >=  740 && m < 745 ) return -0.00850094*(m-740) + 3.84887;
  if( m >=  745 && m < 750 ) return -0.113298*(m-745) + 3.80636;
  if( m >=  750 && m < 755 ) return 0.190869*(m-750) + 3.23987;
  if( m >=  755 && m < 760 ) return -0.0544422*(m-755) + 4.19422;
  if( m >=  760 && m < 1000 ) return -0.000131072*(m-760) + 3.92201;
  if( m >=  1000 && m < 1250 ) return -0.000915535*(m-1000) + 3.89055;
  if( m >=  1250 && m < 1500 ) return -0.000983521*(m-1250) + 3.66167;
  if( m >=  1500 && m < 1750 ) return 0.00144712*(m-1500) + 3.41579;
  if( m >=  1750 && m < 2000 ) return -0.000912781*(m-1750) + 3.77757;
  if( m >=  2000 && m < 2250 ) return 0.00326423*(m-2000) + 3.54937;
  if( m >=  2250 && m < 2500 ) return -0.00316202*(m-2250) + 4.36543;
  if( m >=  2500 && m < 2750 ) return -0.000674945*(m-2500) + 3.57493;
  if( m >=  2750 && m < 3000 ) return 0.00548049*(m-2750) + 3.40619;
  if( m >=  3000 && m < 3250 ) return -0.00151318*(m-3000) + 4.77631;
  if( m >=  3250 && m < 3500 ) return -0.00238002*(m-3250) + 4.39802;
  if( m >=  3500 && m < 3750 ) return 0.00176116*(m-3500) + 3.80301;
  return 0;
};

double RooIntepolateDSCB_W0p014_Spin0_EBEE_2016::getAlpha1( double m ) const
{
  if( m >=  500 && m < 740 ) return 0.000414442*(m-500) + 1.32088;
  if( m >=  740 && m < 745 ) return -0.0154094*(m-740) + 1.42035;
  if( m >=  745 && m < 750 ) return 0.00836468*(m-745) + 1.3433;
  if( m >=  750 && m < 755 ) return 0.0113587*(m-750) + 1.38513;
  if( m >=  755 && m < 760 ) return -0.0156081*(m-755) + 1.44192;
  if( m >=  760 && m < 1000 ) return -9.2941e-05*(m-760) + 1.36388;
  if( m >=  1000 && m < 1250 ) return -2.01925e-05*(m-1000) + 1.34157;
  if( m >=  1250 && m < 1500 ) return -0.000134425*(m-1250) + 1.33653;
  if( m >=  1500 && m < 1750 ) return -8.81417e-05*(m-1500) + 1.30292;
  if( m >=  1750 && m < 2000 ) return 5.42138e-05*(m-1750) + 1.28088;
  if( m >=  2000 && m < 2250 ) return -0.00035673*(m-2000) + 1.29444;
  if( m >=  2250 && m < 2500 ) return -5.95231e-05*(m-2250) + 1.20526;
  if( m >=  2500 && m < 2750 ) return -0.000253634*(m-2500) + 1.19037;
  if( m >=  2750 && m < 3000 ) return -4.29077e-05*(m-2750) + 1.12697;
  if( m >=  3000 && m < 3250 ) return 0.000136791*(m-3000) + 1.11624;
  if( m >=  3250 && m < 3500 ) return -0.000160626*(m-3250) + 1.15044;
  if( m >=  3500 && m < 3750 ) return -3.94266e-05*(m-3500) + 1.11028;
  return 0;
};

double RooIntepolateDSCB_W0p014_Spin0_EBEE_2016::getAlpha2( double m ) const
{
  if( m >=  500 && m < 740 ) return 0.000483547*(m-500) + 1.76561;
  if( m >=  740 && m < 745 ) return -0.00739828*(m-740) + 1.88166;
  if( m >=  745 && m < 750 ) return 0.0343372*(m-745) + 1.84467;
  if( m >=  750 && m < 755 ) return -0.0252737*(m-750) + 2.01636;
  if( m >=  755 && m < 760 ) return 0.00793363*(m-755) + 1.88999;
  if( m >=  760 && m < 1000 ) return 0.000286565*(m-760) + 1.92966;
  if( m >=  1000 && m < 1250 ) return 0.000245411*(m-1000) + 1.99843;
  if( m >=  1250 && m < 1500 ) return 0.000513356*(m-1250) + 2.05979;
  if( m >=  1500 && m < 1750 ) return -0.000114779*(m-1500) + 2.18813;
  if( m >=  1750 && m < 2000 ) return 0.000216371*(m-1750) + 2.15943;
  if( m >=  2000 && m < 2250 ) return -0.000289605*(m-2000) + 2.21352;
  if( m >=  2250 && m < 2500 ) return 0.000584429*(m-2250) + 2.14112;
  if( m >=  2500 && m < 2750 ) return 0.000441657*(m-2500) + 2.28723;
  if( m >=  2750 && m < 3000 ) return -0.00057106*(m-2750) + 2.39764;
  if( m >=  3000 && m < 3250 ) return 0.000648816*(m-3000) + 2.25488;
  if( m >=  3250 && m < 3500 ) return -4.7339e-05*(m-3250) + 2.41708;
  if( m >=  3500 && m < 3750 ) return -0.000124014*(m-3500) + 2.40525;
  return 0;
};

//---------------------

ClassImp(RooFermi) 

RooFermi::RooFermi(){};

RooFermi::RooFermi(const char *name, const char *title, 
		   RooAbsReal& _x,
		   RooAbsReal& _cutOff,
		       RooAbsReal& _beta
		   ) :
  RooAbsPdf(name,title), 
  x("x","x",this,_x),
  cutOff("cutOff","cutOff",this,_cutOff),
  beta("beta","beta",this,_beta)
{ 
};


RooFermi::RooFermi(const RooFermi& other, const char* name) :  
  RooAbsPdf(other,name), 
  x("x",this,other.x),
  cutOff("cutOff",this,other.cutOff),
  beta("beta",this,other.beta)

{ 
};



double RooFermi::evaluate() const 
{ 
  return 1.0/(exp((cutOff-x)/beta)+1);
};

ClassImp(RooRelBW)

RooRelBW::RooRelBW(){};

RooRelBW::RooRelBW(const char *name, const char *title, 
		   RooAbsReal& _x,
		   RooAbsReal& _mean,
		   RooAbsReal& _width,
		       RooAbsReal& _n
		   ) :
  RooAbsPdf(name,title), 
  x("x","x",this,_x),
  mean("mean","mean",this,_mean),
  width("width","width",this,_width),
  n("n","n",this,_n)
{ 
};


RooRelBW::RooRelBW(const RooRelBW& other, const char* name) :  
  RooAbsPdf(other,name), 
  x("x",this,other.x),
  mean("mean",this,other.mean),
  width("width",this,other.width),
  n("n",this,other.n)

{ 
}; 



double RooRelBW::evaluate() const 
{ 
  return pow(x*x,n)/((x*x-mean*mean)*(x*x-mean*mean)+pow(x*x/(mean*mean),2*n)*mean*mean*width*width);
};


ClassImp(Triangle)

Triangle::Triangle(){};

Triangle::Triangle(const char *name, const char *title,                
		   RooAbsReal& _m,
		   RooAbsReal& _start,
		   RooAbsReal& _turn,
		      RooAbsReal& _stop
		   ):
  RooAbsPdf(name, title),
  m("m", "Dependent", this, _m),
  start("start","start",this,_start),
  turn("turn","turn",this,_turn),
  stop("stop","stop",this,_stop)
{
};

Triangle::Triangle(const Triangle& other, const char* name) :
  RooAbsPdf(other, name), m("m", this, other.m),start("start", this, other.start), turn("turn", this, other.turn), stop("stop", this, other.stop)
{
};

Double_t Triangle::evaluate() const 
{
  //std::cout << m << " "<<1.+(start-m)/turn << " " << 1+(turn-m)/stop << std::endl;
  if(m<turn  && m > turn+start)
    return 1.+(turn-m)/start;
  if(m>=turn && m < turn+stop)
    return 1.+(turn-m)/stop;
  
  return 0;
};


Int_t Triangle::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* range) const 
{
  if (matchArgs(allVars,analVars,m)) return 1;
  return 0;
};

Double_t Triangle::analyticalIntegral(Int_t code, const char* rangeName) const 
{

  // WARNING, ASSSUMES TURN TO BE IN INTERVAL
  assert(code==1) ;
  //whole triangle
  Double_t sumleft = sqrt(1+ (turn+start)*(turn+start) ) ;
  Double_t sumright= sqrt(1+ (turn+stop)*(turn+stop) );


  if(m.min() < turn+start)// correct for left missing bit
    sumleft -= sumleft*(m.min()-(turn+start))/fabs(start);


  if(m.max() > turn+stop)// correct for right missing bit
    sumright -= sumright*(turn+stop -m.max())/fabs(stop);

  

  return sumleft+sumright;    
};




ClassImp(RooLevelledExp)

RooLevelledExp::RooLevelledExp(){};

RooLevelledExp::RooLevelledExp(const char *name, const char *title,
			       RooAbsReal& _x,
			       RooAbsReal& _sigma, 
			       RooAbsReal& _alpha,
			       RooAbsReal& _m,
			       RooAbsReal& _theta):
  RooAbsPdf(name,title),
  x("x","x",this,_x),
  sigma("sigma","sigma",this,_sigma),
  alpha("alpha","alpha",this,_alpha),
  m("m","m",this,_m),
  //  k("k","k",this,_k),
  theta("theta","theta",this,_theta)
{
};

RooLevelledExp::RooLevelledExp(const RooLevelledExp& other, const char* name) :
  RooAbsPdf(other,name),
  x("x",this,other.x),
  sigma("sigma",this,other.sigma),
  alpha("alpha",this,other.alpha),
  m("m",this,other.m),
  theta("theta",this,other.theta)
{
};

double RooLevelledExp::evaluate() const
{
  double res=0.0;
  double s = cos(theta)*sigma - sin(theta)*alpha;
  double a = sin(theta)*sigma + cos(theta)*alpha;
    
  //original
  double t = fabs(x-m);
  double den = (s + a*t);
  res=exp(-1.0*t/den);
  

  return res;
};
