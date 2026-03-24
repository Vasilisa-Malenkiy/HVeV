#include "HVeVTrackingAction.hh"
#include "HVeVAnalysis.hh"
#include "HVeVConfigManager.hh"

#include "G4Track.hh"
#include "G4StepStatus.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4VProcess.hh"
#include "G4TrackingManager.hh"
#include "G4ParticleDefinition.hh"

HVeVTrackingAction::HVeVTrackingAction()
:G4UserTrackingAction()
{ }

void HVeVTrackingAction::PreUserTrackingAction(const G4Track* track)
{
    G4String name              = track->GetDefinition()->GetParticleName();
    G4double energy            = track->GetKineticEnergy()/eV;  
    G4int pdgCode              = track->GetDefinition()->GetPDGEncoding(); 
    G4ThreeVector pretrackpos  = track->GetPosition();
    G4double pretrack_x        = pretrackpos.getX()/mm; 
    G4double pretrack_y        = pretrackpos.getY()/mm;
    G4double pretrack_z        = pretrackpos.getZ()/mm;
    G4ThreeVector p            = track->GetMomentum();
    G4double px                = p.getX()/eV;
    G4double py                = p.getY()/eV;
    G4double pz                = p.getZ()/eV;
    G4int trackID              = track->GetTrackID();
    G4int parentID             = track->GetParentID();
    G4double globalTime        = track->GetGlobalTime()/ns;
    const  G4VProcess* creator = track->GetCreatorProcess();

    if (name != "phononL" and name != "phononTS" and name != "phononTF" 
        and name != "G4CMPDriftElectron" and name != "G4CMPDriftHole") {
        if (creator) {
            G4cout << "***** Creator process: " << creator->GetProcessName() << G4endl;
        }
        G4cout << "+++++ Primaries pre track ID " << trackID << " "
               << name << " "
               << energy << " eV "
               << pretrack_x << " "
               << pretrack_y << " "
               << pretrack_z << " mm "
               << px << " "
               << py << " "
               << pz << " "
               << G4endl;

        if (HVeVConfigManager::Instance()->GetPrimariesFlag()) {
            G4RunManager* runMan = G4RunManager::GetRunManager();
            auto analysisManager = G4AnalysisManager::Instance();
            analysisManager->FillNtupleIColumn(6, 0, runMan->GetCurrentEvent()->GetEventID());
            analysisManager->FillNtupleIColumn(6, 1, trackID);
            analysisManager->FillNtupleIColumn(6, 2, pdgCode);
            analysisManager->FillNtupleIColumn(6, 3, parentID);
            analysisManager->FillNtupleDColumn(6, 4, energy);
            analysisManager->FillNtupleDColumn(6, 5, pretrack_x);
            analysisManager->FillNtupleDColumn(6, 6, pretrack_y);
            analysisManager->FillNtupleDColumn(6, 7, pretrack_z);
            analysisManager->FillNtupleDColumn(6, 8, px);
            analysisManager->FillNtupleDColumn(6, 9, py);
            analysisManager->FillNtupleDColumn(6, 10, pz);
            analysisManager->FillNtupleDColumn(6, 11, globalTime);
            //analysisManager->AddNtupleRow(5);
        }
    
    }
    
    if (name == "phononL" or name == "phononTS" or name == "phononTF") {
        if (creator) {
            if (creator -> GetProcessName() == "G4CMPChargeRecombine" ) {

            }
        }
    }
    
    // Set tracks to be visualized
    if (track->GetParentID() == 0 or track->GetParentID() == 1 or track->GetParentID() == 2) {
        fpTrackingManager->SetStoreTrajectory(true);
    } else {
        fpTrackingManager->SetStoreTrajectory(false);
    }
}

void HVeVTrackingAction::PostUserTrackingAction(const G4Track* track)
{
    G4StepStatus stauts = track->GetStep()->GetPostStepPoint()->GetStepStatus();
    G4String name               = track->GetDefinition()->GetParticleName();
    G4double energy             = track->GetKineticEnergy()/eV;  
    G4ThreeVector posttrackpos  = track->GetPosition();
    G4double posttrack_x        = posttrackpos.getX()/mm; 
    G4double posttrack_y        = posttrackpos.getY()/mm;
    G4double posttrack_z        = posttrackpos.getZ()/mm;
    G4int trackID               = track->GetTrackID();
    G4int parentID              = track->GetParentID();
    const  G4VProcess* creator = track->GetCreatorProcess();

    if (name != "phononL" and name != "phononTS" and name != "phononTF" 
        and name != "G4CMPDriftElectron" and name != "G4CMPDriftHole") {
        G4cout << "+++++ Primaries post track ID " << trackID << " "
               << name << " "
               << energy << " eV "
               << posttrack_x << " "
               << posttrack_y << " "
               << posttrack_z << " mm"
               << G4endl;

        if (HVeVConfigManager::Instance()->GetPrimariesFlag()) {
            G4RunManager* runMan = G4RunManager::GetRunManager();
            auto analysisManager = G4AnalysisManager::Instance();
            analysisManager->FillNtupleDColumn(6, 12, posttrack_x);
            analysisManager->FillNtupleDColumn(6, 13, posttrack_y);
            analysisManager->FillNtupleDColumn(6, 14, posttrack_z);
            analysisManager->AddNtupleRow(6);
        }
    }
}
