#include <iostream>
//ROOT
#include <TFile.h>
#include <TTree.h>
//LOCAL
#include <tree.h>

int main( int argc, char** argv)
{
  /*
  TChain* chain = new TChain("tree");
  do the loop()
  tree* mytree = new tree( chain );
  */

  tree* mytree = new tree( );
  mytree->Loop();
  return 0;
}
