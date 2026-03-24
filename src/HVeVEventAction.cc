#include "HVeVEventAction.hh"
#include "HVeVConfigManager.hh"
#include "HVeVAnalysis.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

HVeVEventAction::HVeVEventAction()
: G4UserEventAction()
{}

void HVeVEventAction::BeginOfEventAction(const G4Event* event)
{
    G4cout << "***** Event ID "
           << event->GetEventID()
           << G4endl;

    SetEdepOfThisEvent(0.0);
   
}

void HVeVEventAction::EndOfEventAction(const G4Event*)
{
    G4cout << "Total energy deposited in this event is: "
           << GetEdepOfThisEvent()
           << " eV"
           << G4endl;
    
           G4RunManager* runMan = G4RunManager::GetRunManager();
           auto analysisManager = G4AnalysisManager::Instance();

           analysisManager->FillNtupleIColumn(9, 0, runMan->GetCurrentEvent()->GetEventID());
           analysisManager->FillNtupleDColumn(9, 1, GetEdepOfThisEvent());
           analysisManager->AddNtupleRow(9);

}
