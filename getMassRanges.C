void getMassRanges(const char* inputFile="subTree_-1.root", const char* tag="allSkimmed", float massMin=0, float massMax=10) {
    TFile *infile = new TFile(inputFile);
    TFile *outfile = new TFile(Form("%s_min%g_max%g.root", tag, massMin, massMax), "recreate");
    TTree *fullTree = (TTree*) infile->Get("tree");
    TString cut = Form("mass>%g&&mass<%g", massMin, massMax);

    cout << "Min mass: " << massMin << endl;
    cout << "Max mass: " << massMax << endl;
    cout << "Cut: " << cut << endl;

    TTree *cutTree = fullTree->CopyTree(cut);
    TCanvas *c = new TCanvas("c", "c", 800, 700);
    c->cd();
    cutTree->Draw("mass");
    c->SetLogy();
    c->SaveAs(Form("%s_min%g_max%g.png", tag, massMin, massMax));

    outfile->cd();
    cutTree->Write();
    outfile->Close();
    
    delete c;
    delete fullTree;
    delete infile;
    delete outfile;
}
