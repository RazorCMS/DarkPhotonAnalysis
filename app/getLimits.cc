// C++ INCLUDES
#include <string>
#include <iostream>

// ROOT INCLUDES
#include <TTree.h>
#include <TFile.h>

// ROOFIT INCLUDES
#include <RooWorkspace.h>

// LOCAL INCLUDES
#include "fitDarkphoton.h"
#include "CommandLineInput.h"

int main(int argc, char* argv[]) {
	gROOT->Reset();

	// Parse command line inputs 
	std::string inputFile_sig = ParseCommandLine( argc, argv, "-inputFileSig=" );
	if (  inputFile_sig == "" ) {
		std::cerr << "[ERROR]: please provide an input file using --inputFileSig=<path_to_file>" << std::endl;
		return -1;
	}

	std::string inputFile_bkg = ParseCommandLine( argc, argv, "-inputFileBkg=" );
	if (  inputFile_bkg == "" ) {
	std::cerr << "[INFO]: using a single root file, provide an second input file using --inputFileBkg=<path_to_file>" << std::endl;
	}

	std::string outfilename = ParseCommandLine( argc, argv, "-outputFile=" );
	if ( outfilename == "") {
		std::cerr << "[ERROR]: please provide an output file using --outputFile=<path_to_file>" << std::endl;
                return -1;
        }

	std::string datacardname = ParseCommandLine( argc, argv, "-datacard=" );
	if ( datacardname == "") {
		std::cerr << "[ERROR]: please provide a datacard name using --datacard=<name>" << std::endl;
                return -1;
        }

	std::string treeName = ParseCommandLine( argc, argv, "-treeName=" );
	if (  treeName == "" ) {
		std::cout << "[WARNING]: user did not provide a tree name, if you wish to provide one use --treeName=<yourTreeName>" << std::endl;
		treeName = "tree";
	}

	std::string _mass = ParseCommandLine( argc, argv, "-mzd=" );
	double mass = -1.0;
	if ( _mass != "" ) {
		mass = atof( _mass.c_str() );
	}

	else {
		std::cerr << "[INFO] mass has not been set, please use --mzd=" << std::endl;
	}

	std::string _lifetime = ParseCommandLine( argc, argv, "-tau0=" );
	double lifetime = -1.0;
	if ( _lifetime != "" ) {
		lifetime = atof( _lifetime.c_str() );
	}

	else {
		std::cerr << "[INFO] lifetime has not been set, please use --tau0=" << std::endl;
	}

	std::string vtxCut = ParseCommandLine( argc, argv, "-vtxCut=" );
	if (vtxCut == "") {
		std::cerr << "[INFO]: vtxCut has not been set, using default vtxCut = 0 cm" << std::endl;
		vtxCut = "0";
	}

	std::string sigFit = ParseCommandLine( argc, argv, "-sigFit=" );
	if (sigFit == "") {
		std::cerr << "[INFO]: sigFit has not been set, using default sigFit = bw" << std::endl;
		sigFit = "bw";
	}

	std::string bkgFit = ParseCommandLine( argc, argv, "-bkgFit=" );
	if (bkgFit == "") {
		std::cerr << "[INFO]: bkgFit has not been set, using default bkgFit = expo" << std::endl;
		sigFit = "expo";
	}

	std::string _binNumber = ParseCommandLine( argc, argv, "-binNumber=" );
	TString binNumber = "-666";
	if (_binNumber == "") {
		std::cerr << "[WARNING]: please provide a binNumber, --binNumber=<binNumber>" << std::endl;
	}

	else {
		binNumber = _binNumber;
	}

	std::cout << std::endl;
	std::cout << "[INFO]: treeName: " << treeName << std::endl;
	std::cout << "[INFO]: outputFile: " << outfilename << std::endl;
	std::cout << "[INFO]: datacard: " << datacardname << std::endl;
	std::cout << "[INFO]: mass: " << mass << " GeV" << std::endl;
	std::cout << "[INFO]: lifetime: " << lifetime << " mm" <<  std::endl;
	std::cout << "[INFO]: binNumber: " << binNumber << std::endl;
	std::cout << "[INFO]: vtxCut: " << vtxCut << " cm" << std::endl;
	std::cout << "[INFO]: sigFit: " << sigFit << std::endl;
	std::cout << "[INFO]: bkgFit: " << bkgFit << std::endl;
	std::cout << std::endl;

	// Get files and trees
	TFile *file_sig = new TFile(inputFile_sig.c_str(), "read");
	TFile *file_bkg = new TFile(inputFile_bkg.c_str(), "read");
//	TFile *outfile = new TFile(outfilename, "recreate");
	
	TTree *tree_sig = (TTree*) file_sig->Get(treeName.c_str());
	TTree *tree_bkg = (TTree*) file_bkg->Get(treeName.c_str());

	// Vertex Cuts
	TString distx2 = "(vtxX-primaryVtxX)*(vtxX-primaryVtxX)";
	TString disty2 = "(vtxY-primaryVtxY)*(vtxY-primaryVtxY)";
	TString distz2 = "(vtxZ-primaryVtxZ)*(vtxZ-primaryVtxZ)";
	TString doVtxCut = "sqrt(" + distx2 + "+" + disty2 + "+" + distz2 + ") > " + vtxCut;

	// Total Cuts
	TString cut = doVtxCut;

	// Sideband info
	double sideband_min = 16.5;
	double sideband_max = 23.5;

	// Make datacard
	RooWorkspace *w_sb;
	std::cout << "calling MakeDataCard" << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << "[INFO]: cut for DATA--> "  << cut << std::endl;
	std::cout << "================================================================================" << std::endl;
	std::cout << std::endl;

	w_sb = MakeDataCard(sigFit, bkgFit, tree_sig->CopyTree(cut), tree_bkg->CopyTree(cut), vtxCut, mass, lifetime, binNumber, outfilename.c_str(), datacardname.c_str());

//	MakePlotTree(tree_bkg->CopyTree(cut), "bkg", vtxCut.c_str(), sideband_min, sideband_max);	
//	MakePlotTree(tree_sig->CopyTree(cut), "sig", vtxCut.c_str(), sideband_min, sideband_max);
	
	std::cout << "finish MakeDataCard" << std::endl;
}
