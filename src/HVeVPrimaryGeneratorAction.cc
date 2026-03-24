#include "HVeVPrimaryGeneratorAction.hh"
#include "HVeVConfigManager.hh"

#include "G4Event.hh"
#include "G4Geantino.hh"
#include "G4RandomDirection.hh"
#include "G4PhononTransFast.hh"
#include "G4PhononTransSlow.hh"
#include "G4PhononLong.hh"
#include "G4Electron.hh"
#include "G4CMPDriftElectron.hh"
#include "G4CMPDriftHole.hh"
#include "G4SystemOfUnits.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4IonTable.hh"
#include "Randomize.hh"


using namespace std;

HVeVPrimaryGeneratorAction::HVeVPrimaryGeneratorAction() { 
    fParticleGPS = new G4GeneralParticleSource();
}

HVeVPrimaryGeneratorAction::~HVeVPrimaryGeneratorAction() {
    delete fParticleGPS;
}

void HVeVPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

    G4String f_primaryParticleName = HVeVConfigManager::GetPrimaryParticleName();
    G4double f_primaryParticleEnergy = HVeVConfigManager::GetPrimaryParticleEnergy();
    G4String f_primaryParticlePosType = HVeVConfigManager::GetPrimaryParticlePosType();
    G4double f_primaryParticlePosCenterX = HVeVConfigManager::GetPrimaryParticlePosCenterX()/mm;
    G4double f_primaryParticlePosCenterY = HVeVConfigManager::GetPrimaryParticlePosCenterY()/mm;
    G4double f_primaryParticlePosCenterZ = HVeVConfigManager::GetPrimaryParticlePosCenterZ()/mm;
    G4double f_primaryParticlePosHalfX = HVeVConfigManager::GetPrimaryParticlePosHalfX()/mm;
    G4double f_primaryParticlePosHalfY = HVeVConfigManager::GetPrimaryParticlePosHalfY()/mm;
    G4double f_primaryParticlePosHalfZ = HVeVConfigManager::GetPrimaryParticlePosHalfZ()/mm;
    f_primaryParticlePosHalfZ = 1e-5 * mm; // 10 nm

    G4cout << "Primary particle pos-type is " << f_primaryParticlePosType
           << " center at ( " 
           << f_primaryParticlePosCenterX << ", "
           << f_primaryParticlePosCenterY << ", "
           << f_primaryParticlePosCenterZ
           << " ) and the volume half lengths are ( " 
           << f_primaryParticlePosHalfX << ", "
           << f_primaryParticlePosHalfY << ", "
           << f_primaryParticlePosHalfZ
           << ") "
           << G4endl;


    if (f_primaryParticleName == "chargepair") {
        // Generate Drift Electron:
        fParticleGPS->SetParticleDefinition(G4CMPDriftElectron::Definition());
        // Set the energy for the drift electron
        fParticleGPS->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
        fParticleGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(f_primaryParticleEnergy * eV);
        // Set the position of the drift electron
        fParticleGPS->GetCurrentSource()->GetPosDist()->SetPosDisType("Point");
        fParticleGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(0.0, 0.0, 0.0));
        // Set the angular distribution of the drift electron
        fParticleGPS->GetCurrentSource()->GetAngDist()->SetAngDistType("iso");
        // Generate the drift electron
        fParticleGPS->GeneratePrimaryVertex(anEvent);

        // Generate Drift Hole:
        fParticleGPS->SetParticleDefinition(G4CMPDriftHole::Definition());
        // Set the energy for the drift hole
        fParticleGPS->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
        fParticleGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(f_primaryParticleEnergy * eV);
        // Set the position of the drift hole
        fParticleGPS->GetCurrentSource()->GetPosDist()->SetPosDisType("Point");
        fParticleGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(0.0, 0.0, 0.0));
        // Set the angular distribution of the drift hole
        fParticleGPS->GetCurrentSource()->GetAngDist()->SetAngDistType("iso");
        // Generate the drift hole
        fParticleGPS->GeneratePrimaryVertex(anEvent);
    
    }

    else if (f_primaryParticleName == "Be7_G4") {
        // Somehow there is no hit when I use G4GeneralParticleSource to generate Be7.
        G4ParticleDefinition* ion = G4ParticleTable::GetParticleTable()->GetIonTable()->GetIon(4, 7, 0); // Z=4, A=7
        fParticleGPS->SetParticleDefinition(ion);
        fParticleGPS->GetCurrentSource()->GetAngDist()->SetAngDistType("iso");
        fParticleGPS->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
        fParticleGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(0.0 * eV);
        fParticleGPS->GeneratePrimaryVertex(anEvent);

    }
    else if (f_primaryParticleName == "Be7_KEC_ground") {

        // K-shell ground state Be-7 electron capture
        G4ParticleDefinition* ion = G4ParticleTable::GetParticleTable()->GetIonTable()->GetIon(3, 7, 0); // Z=4, A=7
        fParticleGPS->SetParticleDefinition(ion);
        fParticleGPS->GetCurrentSource()->GetAngDist()->SetAngDistType("iso");
        fParticleGPS->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
        fParticleGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(56.83 * eV);
        fParticleGPS->GetCurrentSource()->GetPosDist()->SetPosDisType("Point");
        //fParticleGPS->GetCurrentSource()->GetPosDist()->SetPosDisType(f_primaryParticlePosType);
        
        if (f_primaryParticlePosType == "Volume") {
            G4double RandVar = G4UniformRand();
            G4double r_primaryParticlePosCenterX = f_primaryParticlePosCenterX - f_primaryParticlePosHalfX 
                                                 + 2 * f_primaryParticlePosHalfX * RandVar;
            RandVar = G4UniformRand();
            G4double r_primaryParticlePosCenterY = f_primaryParticlePosCenterY - f_primaryParticlePosHalfY 
                                                 + 2 * f_primaryParticlePosHalfY * RandVar;
            RandVar = G4UniformRand();
            G4double r_primaryParticlePosCenterZ = f_primaryParticlePosCenterZ - f_primaryParticlePosHalfZ 
                                                 + 2 * f_primaryParticlePosHalfZ * RandVar;
            fParticleGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(r_primaryParticlePosCenterX, 
                                                                                          r_primaryParticlePosCenterY,
                                                                                          r_primaryParticlePosCenterZ));
        }
        
        if (f_primaryParticlePosType == "Point") {
            fParticleGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(f_primaryParticlePosCenterX, 
                                                                                          f_primaryParticlePosCenterY,
                                                                                          f_primaryParticlePosCenterZ));
        }
        fParticleGPS->GeneratePrimaryVertex(anEvent);

        G4PrimaryVertex* vertex = anEvent->GetPrimaryVertex(0);
        G4double vertex_X = vertex->GetX0();
        G4double vertex_Y = vertex->GetY0();
        G4double vertex_Z = vertex->GetZ0();
        G4cout << "Primary Vertex at ( " << vertex_X << ", " 
                                         << vertex_Y << ", "
                                         << vertex_Z << " ) "
                                         << G4endl;

        fParticleGPS->SetParticleDefinition(G4Electron::Definition());
        fParticleGPS->GetCurrentSource()->GetAngDist()->SetAngDistType("iso");
        fParticleGPS->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
        fParticleGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(54 * eV);
        fParticleGPS->GetCurrentSource()->GetPosDist()->SetPosDisType("Point");
        fParticleGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(vertex_X, 
                                                                                      vertex_Y,
                                                                                      vertex_Z
                                                                        ));
        fParticleGPS->GeneratePrimaryVertex(anEvent);
    }

    else if (f_primaryParticleName == "Be7_LEC_ground") {
        // L-shell ground state Be-7 electron capture
        G4ParticleDefinition* ion = G4ParticleTable::GetParticleTable()->GetIonTable()->GetIon(3, 7, 0); // Z=4, A=7
        fParticleGPS->SetParticleDefinition(ion);
        fParticleGPS->GetCurrentSource()->GetAngDist()->SetAngDistType("iso");
        fParticleGPS->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
        fParticleGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(56.83 * eV);
        fParticleGPS->GeneratePrimaryVertex(anEvent);

    }

    else if (f_primaryParticleName == "Li7") {
        G4ParticleDefinition* ion = G4ParticleTable::GetParticleTable()->GetIonTable()->GetIon(3, 7, 0); // Z=4, A=7
        fParticleGPS->SetParticleDefinition(ion);
        fParticleGPS->GetCurrentSource()->GetAngDist()->SetAngDistType("iso");
        fParticleGPS->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
        fParticleGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(56.83 * eV);
        fParticleGPS->GeneratePrimaryVertex(anEvent);
    }

    else {
        fParticleGPS->GeneratePrimaryVertex(anEvent);
    }
}
