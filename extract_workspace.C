int extract_workspace(const char* rooworkspace_file, TString tag, bool saveFig) {
    TFile *file = new TFile(rooworkspace_file);

    // Retriev workspace from file
    RooWorkspace *w = (RooWorkspace*) file->Get("LowMassFits");

    // Retrieve pdf, data from workspace
    RooRealVar *m_mumu = (RooRealVar*) w->var("mass");
    RooAddPdf *sb_model = (RooAddPdf*) w->pdf("sb_model");
    TString s_model_name = sb_model->pdfList()[0].GetName();
    TString b_model_name = sb_model->pdfList()[1].GetName();
    
    RooAddPdf *b_only_model = (RooAddPdf*) w->pdf("b_only_model");
    TString b_only_model_name = b_only_model->pdfList()[0].GetName();

    RooDataHist *binned_tree_mass = (RooDataHist*) w->data("binned_mass");

    // Retrieve fit results
    RooFitResult *sb_model_fit = (RooFitResult*) w->obj("fitresult_sb_model_binned_mass");
    RooFitResult *b_only_model_fit = (RooFitResult*) w->obj("fitresult_b_only_model_binned_mass");


    // Plot
    // S plot
    TCanvas *cs = new TCanvas("cs", "cs", 800, 700); 
    RooPlot *frameS = m_mumu->frame(1.2, 4.0, 100);
    binned_tree_mass->plotOn(frameS);
    
    // B plot
    TCanvas *cb = new TCanvas("cb", "cb", 800, 700); 
    RooPlot *frameB = m_mumu->frame(1.2, 4.0, 100);
    binned_tree_mass->plotOn(frameB, RooFit::Name("dataB"));
    b_only_model->plotOn(frameB, RooFit::Name("b_only_plot"), RooFit::LineColor(kBlue), RooFit::Range("low,high"), RooFit::NormRange("low,high"));
    
    // Plot params
//    b_only_model->paramOn(frameB, RooFit::Layout(0.1, 0.5, 0.5), RooFit::Format("NEU"));
//    frameB->getAttText()->SetTextSize(0.02);
    frameB->Draw();
    frameB->SetTitle("b only model");
    TLegend *legB = new TLegend(0.1,0.8,0.4,0.9);
    legB->AddEntry(frameB->findObject("b_only_plot"), b_only_model_name);
    legB->Draw();
    cb->SetLogy();
    cb->Update();
    
    // S+B plot
    TCanvas *csb = new TCanvas("csb", "csb", 800, 700); 
    RooPlot *frameSB = m_mumu->frame(1.2, 4.0, 100);
    binned_tree_mass->plotOn(frameSB, RooFit::Name("dataSB"));
    sb_model->plotOn(frameSB, RooFit::Name("sb_plot"), RooFit::LineColor(kBlue), RooFit::Range("JPsi"), RooFit::NormRange("JPsi"));
    
    // Plot params
//    sb_model->paramOn(frameSB, RooFit::Layout(0.1, 0.5, 0.5), RooFit::Format("NEU"));
//    frameSB->getAttText()->SetTextSize(0.02);
    
    // Plot components
    sb_model->plotOn(frameSB, RooFit::Components(sb_model->pdfList()[0]), RooFit::LineStyle(kDashed), RooFit::LineColor(kGreen));
    sb_model->plotOn(frameSB, RooFit::Components(sb_model->pdfList()[1]), RooFit::LineStyle(kDashed), RooFit::LineColor(kRed));
    
    frameSB->Draw();
    frameSB->SetTitle("s + b model");
    TLegend *legSB = new TLegend(0.1,0.8,0.4,0.9);
    legSB->AddEntry(frameSB->findObject("sb_plot"), s_model_name + " " + b_model_name);
    legSB->Draw();
    csb->SetLogy();
    csb->Update();
    
    if (saveFig) {
        csb->SaveAs("sb_JPsi_workspace_" + s_model_name + "_" + b_model_name + "_" + tag + ".png");
        cb->SaveAs("b_JPsi_workspace_" + b_only_model_name + "_"  + tag + ".png");
    }

    // Print model info
    cout << endl;
    cout << endl;
    sb_model->Print();
    b_only_model->Print();

    cout << "=======================================================" << endl;
    cout << "FIT RESULTS" << endl;
    cout << "=======================================================" << endl;
    cout << endl;
    
    cout << "=======================================================" << endl;
    cout << "S+B Model" << endl;
    cout << "=======================================================" << endl;
    cout << "S: " << s_model_name << endl;
    cout << "B: " << b_model_name << endl;
    cout << endl;
    sb_model_fit->printMultiline(cout, 1, kTRUE);
    cout << endl;
    cout << endl;
    cout << "Number of NLL evaluations with problems: " << sb_model_fit->numInvalidNLL() << endl;

    // 2 ways to get chi2
    RooChi2Var chi2_sb("chi2_sb", "chi2_sb", *sb_model, *binned_tree_mass, RooFit::Extended(kTRUE), RooFit::Range("JPsi"));
    double chi2_sb_val = chi2_sb.getVal();
    double sb_ndof = sb_model_fit->floatParsFinal().getSize();
    
    cout << "Using frame - chi2 / ndof: " << frameSB->chiSquare("sb_plot", "dataSB", sb_ndof) << endl;
    cout << "Using RooChi2Var - chi2 / ndof: " << chi2_sb_val << " / " << sb_ndof << " = " << chi2_sb_val / sb_ndof << endl;

    cout << "minNLL: " << sb_model_fit->minNll() << endl;
    cout << endl;
    cout << endl;

    cout << "=======================================================" << endl;
    cout << "B Only Model" << endl;
    cout << "=======================================================" << endl;
    b_only_model_fit->printMultiline(cout, 1, kTRUE);
    cout << endl;
    cout << endl;
    cout << "Number of NLL evaluations with problems: " << sb_model_fit->numInvalidNLL() << endl;

    // 2 ways to get chi2
    RooChi2Var chi2_b_only("chi2_b_only", "chi2_b_only", *b_only_model, *binned_tree_mass, RooFit::Extended(kTRUE), RooFit::Range("low,high"));
    double chi2_b_only_val = chi2_b_only.getVal();
    double b_only_ndof = b_only_model_fit->floatParsFinal().getSize();
    
    cout << "Using frame - chi2 / ndof: " << frameB->chiSquare("b_only_plot", "dataB", b_only_ndof) << endl;
    cout << "Using RooChi2Var - chi2 / ndof: " << chi2_b_only_val << " / " << b_only_ndof << " = " << chi2_b_only_val / b_only_ndof << endl;

    cout << "minNLL: " << b_only_model_fit->minNll() << endl;
    cout << endl;
    cout << endl;

    return 0;
}
