#include "HVeVSteppingAction.hh"
#include "HVeVConfigManager.hh"
#include "HVeVEventAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"
#include "HVeVAnalysis.hh"
#include "G4RunManager.hh"
#include "G4VPhysicalVolume.hh"

#include "G4LatticeManager.hh"
#include "G4LatticePhysical.hh"
#include "G4CMPDriftTrackInfo.hh"
#include "G4CMPUtils.hh"
#include "G4CMPProcessUtils.hh"
#include "G4CMPTrackUtils.hh"
#include "G4PhysicalConstants.hh"

HVeVSteppingAction::HVeVSteppingAction()
: process_utils(0)
{
    process_utils = new G4CMPProcessUtils();
}

HVeVSteppingAction::~HVeVSteppingAction()
{}

void HVeVSteppingAction::UserSteppingAction(const G4Step* step)
{
    G4Track* track = step->GetTrack();
    G4String particleName = track->GetParticleDefinition()->GetParticleName();
    G4int trackID = track->GetTrackID();
    G4int parentID = track->GetParentID();
    G4int pdgCode = track->GetDefinition()->GetPDGEncoding();
    G4double ekin = track->GetKineticEnergy()/eV;

    G4double ekin_initial = step->GetPreStepPoint()->GetKineticEnergy()/eV;
    G4double ekin_final = step->GetPostStepPoint()->GetKineticEnergy()/eV;

    G4RunManager* runMan = G4RunManager::GetRunManager();
    auto analysisManager = G4AnalysisManager::Instance();

    const std::vector<const G4Track*>* secondaries = step->GetSecondaryInCurrentStep();
    if (secondaries->size() > 0) {

        if (particleName != "phononL" and particleName != "phononTS" and particleName != "phononTF"
            and particleName != "G4CMPDriftElectron" and particleName != "G4CMPDriftHole") {

                G4cout << "Primary Track ID: " << trackID << " " << particleName
                       << " with Kinetic Energy: " << ekin << " eV "
                       << " produced " << secondaries->size() << " secondaries" << G4endl;
                G4double total_energy_charge_pair = 0.0;
                G4double total_energy_phonon = 0.0;
                G4double total_energy_others = 0.0;

                G4VPhysicalVolume* postVolume = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume();
                if (step->GetTotalEnergyDeposit() > 0 and postVolume->GetName() == "fSiliconPhysical" and trackID != 1) {
                    G4cout << "Total energy deposit in this step is : " << step->GetTotalEnergyDeposit()/eV << " eV"
                           << ", (non-ionizing part: " << step->GetNonIonizingEnergyDeposit()/eV << " eV)"
                           << " in the physical volume: "
                           << postVolume->GetName()
                           << G4endl;
                
                    HVeVEventAction* eventAction = (HVeVEventAction*)runMan->GetUserEventAction();
                    G4double edep = eventAction->GetEdepOfThisEvent();
                    edep += step->GetTotalEnergyDeposit()/eV;
                    eventAction->SetEdepOfThisEvent(edep);

                }
                
//                //NEW ADDITION (RESOLVING EDEP 11/17/2025)
//                // ---- NEW: Record energy deposition in aluminum films ----
//                G4double edep = step->GetTotalEnergyDeposit();
//
//                if (edep > 0.) {
//                    G4VPhysicalVolume* postVolume = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume();
//                    G4String volName = postVolume->GetName();
//
//                    G4cout << "DEBUG: Step in volume " << volName
//                           << " with energy deposit = " << edep/eV << " eV" << G4endl;
//
//                    if (volName == "fAluminumInnerTopPhysical") {
//                        analysisManager->FillNtupleDColumn(1, 0, edep);
//                        analysisManager->AddNtupleRow(1);
//                    }
//                    else if (volName == "fAluminumOuterTopPhysical") {
//                        analysisManager->FillNtupleDColumn(2, 0, edep);
//                        analysisManager->AddNtupleRow(2);
//                    }
//                    else if (volName == "fAluminumBottomPhysical") {
//                        analysisManager->FillNtupleDColumn(7, 0, edep);
//                        analysisManager->AddNtupleRow(7);
//                    }
//                }

                for (auto secondary : *secondaries) {
                    G4int secID = secondary->GetTrackID();
                    G4double secEkin = secondary->GetKineticEnergy()/eV;
                    G4String secName = secondary->GetParticleDefinition()->GetParticleName();
                    if (secName == "G4CMPDriftElectron" or secName == "G4CMPDriftHole") {
                        total_energy_charge_pair += secEkin;
    
                        if (HVeVConfigManager::Instance()->GetPrimariesFlag()) {
                            G4double x = secondary->GetPosition().x()/mm;
                            G4double y = secondary->GetPosition().y()/mm;
                            G4double z = secondary->GetPosition().z()/mm;

                            analysisManager->FillNtupleIColumn(4, 0, runMan->GetCurrentEvent()->GetEventID());
                            analysisManager->FillNtupleIColumn(4, 1, secID);
                            analysisManager->FillNtupleIColumn(4, 2, trackID);
                            analysisManager->FillNtupleIColumn(4, 3, pdgCode);
                            analysisManager->FillNtupleDColumn(4, 4, secEkin);
                            analysisManager->FillNtupleDColumn(4, 5, x);
                            analysisManager->FillNtupleDColumn(4, 6, y);
                            analysisManager->FillNtupleDColumn(4, 7, z);
                            analysisManager->AddNtupleRow(4);
                        }
                    }
                    else if (secName == "phononL" or secName == "phononTS" or secName == "phononTF") {
                        total_energy_phonon += secEkin;

                        if (HVeVConfigManager::Instance()->GetPrimariesFlag()) {
                            G4double x = secondary->GetPosition().x()/mm;
                            G4double y = secondary->GetPosition().y()/mm;
                            G4double z = secondary->GetPosition().z()/mm;

                            analysisManager->FillNtupleIColumn(3, 0, runMan->GetCurrentEvent()->GetEventID());
                            analysisManager->FillNtupleIColumn(3, 1, secID);
                            analysisManager->FillNtupleIColumn(3, 2, trackID);
                            analysisManager->FillNtupleIColumn(3, 3, pdgCode);
                            analysisManager->FillNtupleDColumn(3, 4, secEkin);
                            analysisManager->FillNtupleDColumn(3, 5, x);
                            analysisManager->FillNtupleDColumn(3, 6, y);
                            analysisManager->FillNtupleDColumn(3, 7, z);
                            analysisManager->AddNtupleRow(3);
                        }

                    }
                    else {
                        total_energy_others += secEkin;
                        G4cout << "Secondary Track ID: " << secID << " " << secName << " Ekin: " << secEkin << G4endl;
                    }
                }
            G4cout << "\nTotal energy partition of " << particleName << ": " << ekin_initial - ekin_final << " eV\n"
                   << "Charge-pair: " << total_energy_charge_pair << " eV\n"
                   << "Phonon: " << total_energy_phonon << " eV\n"
                   << "Others: " << total_energy_others << " eV\n" << G4endl;
        }

        if (HVeVConfigManager::Instance()->GetPrimariesFlag()) {
            for (auto secondary : *secondaries) {
                G4int secID = secondary->GetTrackID();
                G4double secEkin = secondary->GetKineticEnergy()/eV;
                G4String secName = secondary->GetParticleDefinition()->GetParticleName();
                G4double x = secondary->GetPosition().x()/mm;
                G4double y = secondary->GetPosition().y()/mm;
                G4double z = secondary->GetPosition().z()/mm;
                G4double t = secondary->GetGlobalTime()/ns;
                const  G4VProcess* creator = secondary->GetCreatorProcess();

                if (creator)
                {
                    if (creator->GetProcessName() == "G4CMPLukeScattering") {
                        //G4cout << "This is a Luke "
                        //       << secName
                        //       << " generated from the parent trackID = "
                        //       << trackID
                        //       << ", which is a "
                        //       << particleName
                        //       << G4endl;
                        analysisManager->FillNtupleIColumn(5, 0, runMan->GetCurrentEvent()->GetEventID());
                        analysisManager->FillNtupleIColumn(5, 1, secID);
                        analysisManager->FillNtupleIColumn(5, 2, trackID);
                        if (particleName == "G4CMPDriftElectron")
                            analysisManager->FillNtupleIColumn(5, 3, 1);
                        else if (particleName == "G4CMPDriftHole")
                            analysisManager->FillNtupleIColumn(5, 3, -1);
                        analysisManager->FillNtupleDColumn(5, 4, secEkin);
                        analysisManager->FillNtupleDColumn(5, 5, t);
                        analysisManager->FillNtupleDColumn(5, 6, x);
                        analysisManager->FillNtupleDColumn(5, 7, y);
                        analysisManager->FillNtupleDColumn(5, 8, z);
                        analysisManager->AddNtupleRow(5);
                    }
                }
                if (creator)
                {
                    if (creator->GetProcessName() == "G4CMPChargeRecombine") {
                        analysisManager->FillNtupleIColumn(8, 0, runMan->GetCurrentEvent()->GetEventID());
                        analysisManager->FillNtupleIColumn(8, 1, secID);
                        analysisManager->FillNtupleDColumn(8, 2, secEkin);
                        analysisManager->FillNtupleDColumn(8, 3, t);
                        analysisManager->FillNtupleDColumn(8, 4, x);
                        analysisManager->FillNtupleDColumn(8, 5, y);
                        analysisManager->FillNtupleDColumn(8, 6, z);
                        analysisManager->AddNtupleRow(8);

                    }
                }
            }
        }

    }

    /*

    // Test the Luke phonon generation threshold and energy loss
    if (particleName == "G4CMPDriftElectron" or particleName == "G4CMPDriftHole") {
        auto trackInfo = G4CMP::GetTrackInfo<G4CMPDriftTrackInfo>(*track);
        const G4LatticePhysical* lat = trackInfo->Lattice();
        G4int iValley = trackInfo->ValleyIndex();
        //G4cout << "Test point: valley index is " << iValley << G4endl;

        G4ThreeVector ktrk(0.);
        G4double mass = 0.;
        G4double Etrk = 0.;
        //G4ThreeVector ptrk = process_utils->GetLocalDirection(step->GetPostStepPoint()->GetMomentum());
        //G4cout << ptrk.x() << " " << ptrk.y() << " " << ptrk.z() << G4endl;

        G4ThreeVector kdir = ktrk.unit();
        G4double kmag = ktrk.mag();
        G4double gammaSound = 1 / sqrt(1. - lat->GetSoundSpeed() * lat->GetSoundSpeed() / c_squared);
        G4double kSound = gammaSound * lat->GetSoundSpeed() * mass / hbar_Planck;

        //if (G4CMP::IsElectron(track)) {
        //    ktrk = lat->MapPtoK(iValley, ptrk);
        //    // Tuning wave-vector to spherical frame where electrons act like holes.
        //    ktrk = lat->EllipsoidalToSphericalTranformation(iValley, ktrk);
        //    mass = lat->GetElectronMass();
        //    Etrk = lat->MapPtoEkin(iValley, ptrk);
        //} else if (G4CMP::IsHole(track)) {
        //    ktrk = process_utils->GetLocalWaveVector(track);
        //    mass = lat->GetHoleMass();
        //    Etrk = process_utils->GetKineticEnergy(track);
        //} else {
        //    G4cout << "Unknown charge carrier." << G4endl;
        //}
    }
    


    G4ThreeVector kdir = ktrk.unit();
    G4double kmag = ktrk.mag();
    G4double gammaSound = 1 / sqrt(1. - lat->GetSoundSpeed() * lat->GetSoundSpeed() / c_squared);
    G4double kSound = gammaSound * lat->GetSoundSpeed() * mass / hbar_Planck;

    G4cout << " E_track " << Etrk/eV << " eV"
           << " mass " << mass*c_squared/electron_mass_c2 << " m_e"
           << G4endl;

    G4ThreeVector p_global = process_utils->GetGlobalMomentum(track);
    G4cout << "p_global = " << p_global/eV << " " << p_global.mag()/eV << " eV"
           << G4endl
           << " p_local = " << ptrk/eV << " " << ptrk.mag()/eV<< " eV"
           << G4endl;

    if (G4CMP::IsElectron(track)) {
      G4ThreeVector kvalley = lat->MapPtoK(iValley, ptrk);
      G4ThreeVector pvalley = kvalley * hbarc;
      G4cout << " valley " << iValley << " along "
             << lat->GetValleyAxis(iValley) << G4endl
             << " p_valley = " << pvalley/eV << " " << pvalley.mag()/eV
             << " eV" << G4endl
             << " k = " << kvalley/eV << " " << kvalley.mag()/eV
             << " eV" << G4endl;
    }

    G4cout << " ktrk = " << ktrk << " kmag " << kmag << G4endl
           << " k/ks = " << kmag/kSound << " acos(ks/k) = " << acos(kSound/kmag)
           << G4endl;
           */
}
