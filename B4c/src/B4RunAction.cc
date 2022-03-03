//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B4RunAction.cc
/// \brief Implementation of the B4RunAction class

#include "B4RunAction.hh"
#include "B4Analysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4RunAction::B4RunAction()
 : G4UserRunAction()
{ 
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);     

  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in B4Analysis.hh
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Create directories 
  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
    // Note: merging ntuples is available only with Root output

  // Book histograms, ntuple
  //
  
  // Creating histograms
  analysisManager->CreateH1("Eabs","Edep in absorber", 100, 0., 10*GeV);
  analysisManager->CreateH1("Egap","Edep in gap", 100, 0., 100*MeV);
  analysisManager->CreateH1("Labs","trackL in absorber", 100, 0., 1*m);
  analysisManager->CreateH1("Lgap","trackL in gap", 100, 0., 50*cm);

	for(G4int i=1; i<11;i++) {
		G4String ntuple_name =  "Eabs_l"+std::to_string(i);
		G4String ntuple_title = "Deposited energy in absorber layer "+std::to_string(i);
		analysisManager->CreateH1(ntuple_name, ntuple_title, 100, 0., 10*GeV); //Hist ID from 4 to 13
	}

	for(G4int i=1; i<11;i++){
		G4String ntuple_name =  "Egap_l"+std::to_string(i);
		G4String ntuple_title = "Deposited energy in gap layer "+std::to_string(i);
		analysisManager->CreateH1(ntuple_name, ntuple_title, 100, 0., 1*MeV); //Hist ID from 14 to 23
	}

	for(G4int i=1; i<11;i++){
		G4String ntuple_name =  "X_distribution_gap_l"+std::to_string(i);
		G4String ntuple_title = "X hit position in gap layer "+std::to_string(i);
		analysisManager->CreateH1(ntuple_name, ntuple_title, 100, -15.*cm, +15.*cm); //Hist ID from 24 to 33
	}

	for(G4int i=1; i<11;i++){
		G4String ntuple_name =  "Y_distribution_gap_l"+std::to_string(i);
		G4String ntuple_title = "Y hit position in gap layer "+std::to_string(i);
		analysisManager->CreateH1(ntuple_name, ntuple_title, 100, -15.*cm, +15.*cm); //Hist ID from 34 to 43
	}

	for(G4int i=1; i<11;i++){
		G4String ntuple_name =  "Z_distribution_gap_l"+std::to_string(i);
		G4String ntuple_title = "Z hit position in gap layer "+std::to_string(i);
		analysisManager->CreateH1(ntuple_name, ntuple_title, 100, -15.*cm, +15.*cm); //Hist ID from 44 to 53
	}
/*
        analysisManager->CreateH1("Z_distribution_gap_l1", "Z hit position in gap layer 1", 100, -10.5*cm, -10.*cm);
        analysisManager->CreateH1("Z_distribution_gap_l2", "Z hit position in gap layer 2", 100, -8.*cm, -7.5*cm);
        analysisManager->CreateH1("Z_distribution_gap_l3", "Z hit position in gap layer 3", 100, -5.5*cm, -5.*cm);
        analysisManager->CreateH1("Z_distribution_gap_l4", "Z hit position in gap layer 4", 100, -3.*cm, -2.5*cm);
        analysisManager->CreateH1("Z_distribution_gap_l5", "Z hit position in gap layer 5", 100, -0.5*cm, 0.*cm);
        analysisManager->CreateH1("Z_distribution_gap_l6", "Z hit position in gap layer 6", 100, 2.*cm, 2.5*cm);
        analysisManager->CreateH1("Z_distribution_gap_l7", "Z hit position in gap layer 7", 100, 4.5*cm, 5*cm);
        analysisManager->CreateH1("Z_distribution_gap_l8", "Z hit position in gap layer 8", 100, 7.*cm, 7.5*cm);
        analysisManager->CreateH1("Z_distribution_gap_l9", "Z hit position in gap layer 9", 100, 9.5*cm, 10.*cm);
        analysisManager->CreateH1("Z_distribution_gap_l10", "Z hit position in gap layer 10", 100, 12.*cm, 12.5*cm);
  */  
	for(G4int i=1; i<11;i++){
		G4String ntuple_name =  "Time_distribution_gap_l"+std::to_string(i);
		G4String ntuple_title = "Time_distribution_gap_l"+std::to_string(i);
		analysisManager->CreateH1(ntuple_name, ntuple_title, 100, 0., 10.*us); //Hist ID from 44 to 53
	}
    
	analysisManager->CreateH1("n_hits","Number of hits", 100, 0, 1000); //ID 54

/*
	for(G4int i=1; i<11;i++) {
		G4String histo_name =  "Eabs_l";
		histo_name += to_string(i);
		G4String histo_title =  "Edep in absorber - layer";
		histo_title += to_string(i);
		analysisManager->CreateH1(histo_name, histo_title, 100, 0., 800*MeV); //Hist ID from 4 to 13
	}

	for(G4int i=1; i<11;i++){
		G4String histo_name =  "Egap_l"+std::to_string(i);
		G4String histo_title =  "Edep in gap - layer"+std::to_string(i);
		analysisManager->CreateH1(histo_name, histo_title, 100, 0., 1*MeV); //Hist ID from 14 to 23
	}

*/
  // Creating 2D histograms
  analysisManager->CreateH2("Eabs_l","Edep in absorber - layer ", 10, 0.5, 10.5, 100, 0., 4*GeV); //Hist ID 0
  analysisManager->CreateH2("Egap_l","Edep in gap - layer ", 10, 0.5, 10.5, 100, 0., 1*MeV); //Hist ID 1
  //analysisManager->CreateH3("Hit_position_l1",  "Track position gap layer 1", 100, -20*cm, 20*cm, 100, -7*cm, 7*cm, 100, -7*cm, 7*cm); //Hist ID 0
/*
  for(G4int i=1; i<11;i++){
		G4String ntuple_name =  "Hit_position_l"+std::to_string(i);
		G4String ntuple_title =  "Track position layer "+std::to_string(i);
		//analysisManager->CreateH3(ntuple_name, ntuple_title, 100, -20*cm, 20*cm, 100, -7*cm, 7*cm, 100, -7*cm, 7*cm); //Hist ID from 0 to 10
	}
*/
  // Creating ntuple
  //
  analysisManager->CreateNtuple("B4", "Edep and TrackL");
  analysisManager->CreateNtupleDColumn("Eabs");
  analysisManager->CreateNtupleDColumn("Egap");
  analysisManager->CreateNtupleDColumn("Labs");
  analysisManager->CreateNtupleDColumn("Lgap");

  	
	
  // // ID 54 ()

  analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4RunAction::~B4RunAction()
{
  delete G4AnalysisManager::Instance();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4RunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //
  G4String fileName = "B4";
  analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // print histogram statistics
  //
  auto analysisManager = G4AnalysisManager::Instance();
  if ( analysisManager->GetH1(1) ) {
    G4cout << G4endl << " ----> print histograms statistic ";
    if(isMaster) {
      G4cout << "for the entire run " << G4endl << G4endl; 
    }
    else {
      G4cout << "for the local thread " << G4endl << G4endl; 
    }
    
    G4cout << " EAbs : mean = " 
       << G4BestUnit(analysisManager->GetH1(0)->mean(), "Energy") 
       << " rms = " 
       << G4BestUnit(analysisManager->GetH1(0)->rms(),  "Energy") << G4endl;
    
    G4cout << " EGap : mean = " 
       << G4BestUnit(analysisManager->GetH1(1)->mean(), "Energy") 
       << " rms = " 
       << G4BestUnit(analysisManager->GetH1(1)->rms(),  "Energy") << G4endl;
    
    G4cout << " LAbs : mean = " 
      << G4BestUnit(analysisManager->GetH1(2)->mean(), "Length") 
      << " rms = " 
      << G4BestUnit(analysisManager->GetH1(2)->rms(),  "Length") << G4endl;

    G4cout << " LGap : mean = " 
      << G4BestUnit(analysisManager->GetH1(3)->mean(), "Length") 
      << " rms = " 
      << G4BestUnit(analysisManager->GetH1(3)->rms(),  "Length") << G4endl;
  }

  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
