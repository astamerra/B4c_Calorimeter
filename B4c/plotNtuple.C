// ROOT macro file for plotting example B4 ntuple
// 
// Can be run from ROOT session:
// root[0] .x plotNtuple.C

{
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  
  // Draw histos filled by Geant4 simulation 
  //   

  // Open file filled by Geant4 simulation 
  TFile f("B4.root");

  // Create a canvas and divide it into 2x2 pads
  TCanvas* c1 = new TCanvas("c1", "", 20, 20, 1000, 1000);
  c1->Divide(2,2);
  
  // Get ntuple
  TNtuple* ntuple = (TNtuple*)f.Get("B4");

  // Draw Eabs histogram in the pad 1
  c1->cd(1);
  ntuple->Draw("Eabs");
  
  // Draw Labs histogram in the pad 2
  c1->cd(2);
  ntuple->Draw("Labs");
  
  // Draw Egap histogram in the pad 3
  // with logaritmic scale for y  ?? how to do this?
  c1->cd(3);
  gPad->SetLogy(1);
  ntuple->Draw("Egap");
  ntuple->GetXaxis()->SetTitle("Energy deposited in absorber layers")
  
  // Draw Lgap histogram in the pad 4
  // with logaritmic scale for y  ?? how to do this?
  c1->cd(4);
  gPad->SetLogy(1);
  ntuple->Draw("Lgap");

  TCanvas* c2 = new TCanvas("c2", "", 20, 20, 1000, 1000);
  c2->Divide(2);

  c2->cd(1);
  gPad->SetLogy(1);
  ntuple->Draw("Eabs_l1");

  c2->cd(2);
  gPad->SetLogy(1);
  ntuple->Draw("Egap_l1");

  
}  
