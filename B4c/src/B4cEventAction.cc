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
/// \file B4cEventAction.cc
/// \brief Implementation of the B4cEventAction class

#include "B4cEventAction.hh"
#include "B4cCalorimeterSD.hh"
#include "B4cCalorHit.hh"
#include "B4Analysis.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4cEventAction::B4cEventAction()
 : G4UserEventAction(),
   fAbsHCID(-1),
   fGapHCID(-1)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4cEventAction::~B4cEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4cCalorHitsCollection* 
B4cEventAction::GetHitsCollection(G4int hcID,
                                  const G4Event* event) const
{
  auto hitsCollection 
    = static_cast<B4cCalorHitsCollection*>(
        event->GetHCofThisEvent()->GetHC(hcID));
  
  if ( ! hitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID; 
    G4Exception("B4cEventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }         

  return hitsCollection;
}    

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4cEventAction::PrintEventStatistics(
                              G4double absoEdep, G4double absoTrackLength,
                              G4double gapEdep, G4double gapTrackLength) const
{
  // print event statistics
  G4cout
     << "   Absorber: total energy: " 
     << std::setw(7) << G4BestUnit(absoEdep, "Energy")
     << "       total track length: " 
     << std::setw(7) << G4BestUnit(absoTrackLength, "Length")
     << G4endl
     << "        Gap: total energy: " 
     << std::setw(7) << G4BestUnit(gapEdep, "Energy")
     << "       total track length: " 
     << std::setw(7) << G4BestUnit(gapTrackLength, "Length")
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4cEventAction::BeginOfEventAction(const G4Event* /*event*/)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4cEventAction::EndOfEventAction(const G4Event* event)
{  
  // Get hits collections IDs (only once)
  if ( fAbsHCID == -1 ) {
    fAbsHCID 
      = G4SDManager::GetSDMpointer()->GetCollectionID("AbsorberHitsCollection");
    fGapHCID 
      = G4SDManager::GetSDMpointer()->GetCollectionID("GapHitsCollection");
  }

   // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Get hits collections
  auto absoHC = GetHitsCollection(fAbsHCID, event);
  auto gapHC = GetHitsCollection(fGapHCID, event);

  // Get hit with total values
  auto absoHit = (*absoHC)[absoHC->entries()-1];
  auto gapHit = (*gapHC)[gapHC->entries()-1];
  

  std::vector<B4cCalorHit*> absoHit_layer;
  std::vector<B4cCalorHit*> gapHit_layer;  
  std::vector<int> layer;
  
  

    G4int nHits = absoHC->entries();
    G4cout << "Entries collection " << nHits << G4endl;



    for(G4int i= 0; i<nHits; i++){
	absoHit_layer.push_back((*absoHC)[i]);
        gapHit_layer.push_back((*gapHC)[i]);
      //  G4cout << " Edep abs layer " << i << " : " << G4BestUnit(absoHit_layer.at(i)->GetEdep(), "Energy") << G4endl;
	//G4cout << " Edep gap layer " << i << " : " << G4BestUnit(gapHit_layer.at(i)->GetEdep(), "Energy") << G4endl;
	//G4cout << " Hit position in gap " << i << " : " << G4BestUnit(gapHit_layer.at(i)->GetEdep(), "Energy") << G4endl;
	//analysisManager->FillH2(0, i, absoHit_layer.at(i)->GetEdep());
	//analysisManager->FillH2(1, i, gapHit_layer.at(i)->GetEdep());
	//analysisManager->FillNtupleDColumn(4, absoHit_layer1->GetEdep());
    }

  // Print per event (modulo n)
  //
  auto eventID = event->GetEventID();
  auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    G4cout << "---> End of event: " << eventID << G4endl;     
    
    for (int i=1; i<=10; i++) layer.push_back(i);

    PrintEventStatistics(
      absoHit->GetEdep(), absoHit->GetTrackLength(),
      gapHit->GetEdep(), gapHit->GetTrackLength());
  }  
  
  // Fill histograms, ntuple
  //

  //analysisManager->FillH2(0, layer, absoHit_layer.GetEdep());
  //analysisManager->FillH2(1, layer, gapHit_layer.GetEdep());
 
  // fill histograms
  analysisManager->FillH1(0, absoHit->GetEdep());

  analysisManager->FillH1(1, gapHit->GetEdep());
  analysisManager->FillH1(2, absoHit->GetTrackLength());
  analysisManager->FillH1(3, gapHit->GetTrackLength());

 

  analysisManager->FillH1(54, gapHit->GetNHits());

 for(G4int i=0; i<10; ++i){
		G4ThreeVector Position;
		Position = gapHit_layer.at(i)->GetHitPosition();
		G4double X_position = Position[0]; G4double Y_position = Position[1]; G4double Z_position = Position[2];
		analysisManager->FillH1(i+4, absoHit_layer.at(i)->GetEdep());
  		analysisManager->FillH1(i+14, gapHit_layer.at(i)->GetEdep());
		analysisManager->FillH1(i+24, X_position);
		analysisManager->FillH1(i+34, Y_position);
		analysisManager->FillH1(i+44, Z_position);
		analysisManager->FillH1(i+54, gapHit_layer.at(i)->GetGlobalTime());
		
	}

	for(G4int i=0; i<11; ++i){
		analysisManager->FillH2(0, i, absoHit_layer.at(i)->GetEdep());
		analysisManager->FillH2(1, i, gapHit_layer.at(i)->GetEdep());
	}

/*
	for(G4int i=0; i<absoHit_layer.size(); i++){
		analysisManager->FillH1(i+4, absoHit_layer.at(i)->GetEdep());
  		analysisManager->FillH1(i+14, gapHit_layer.at(i)->GetEdep());
	}
	
/*
	std::vector<G4ThreeVector> Position_gap;

	for(G4int i=0; i<absoHit_layer.size(); i++){
		Position_gap.at(i) = gapHit_layer.at(i)->GetHitPosition();
		//analysisManager->FillH3(i, (Position_gap.at(i))[0], (Position_gap.at(i))[1], (Position_gap.at(i))[2]);
  		//analysisManager->FillH1(i+14, gapHit_layer.at(i)->GetEdep());
	}
*/
  //analysisManager->FillH3(0, (Position_gap.at(0))[0], (Position_gap.at(0))[1], (Position_gap.at(0))[2]);
 
  // fill ntuple
  analysisManager->FillNtupleDColumn(0, absoHit->GetEdep());
  analysisManager->FillNtupleDColumn(1, gapHit->GetEdep());
  analysisManager->FillNtupleDColumn(2, absoHit->GetTrackLength());
  analysisManager->FillNtupleDColumn(3, gapHit->GetTrackLength());

 

  analysisManager->AddNtupleRow();  
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
