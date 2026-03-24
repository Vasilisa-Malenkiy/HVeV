#include "HVeVRunAction.hh"
#include "HVeVAnalysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "HVeVConfigManager.hh"
#include "G4CMPConfigManager.hh"
#include "G4VNIELPartition.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

HVeVRunAction::HVeVRunAction()
: G4UserRunAction()
{
}

HVeVRunAction::~HVeVRunAction()
{
}

void HVeVRunAction::BeginOfRunAction(const G4Run* run)
{
    CreateNtuple();
    PrintRunInfo(run);

    G4CMPConfigManager* cmpConfigManager = G4CMPConfigManager::Instance();
    cmpConfigManager->printConfig(std::cout);

    /*
    const G4VNIELPartition* niel_partition = cmpConfigManager->GetNIELPartition();

    G4NistManager* nistManager = G4NistManager::Instance();
    G4Material* fSilicon = nistManager->FindOrBuildMaterial("G4_Si");
    
    G4double amu_c2 = 931.494028 * MeV;
    G4ParticleDefinition* proj = G4ParticleTable::GetParticleTable()->GetIonTable()->GetIon(3, 7, 0); // Z=4, A=7
    G4int proj_PDGcode = proj->GetPDGEncoding();
    if (proj) {
        G4double Z = proj->GetAtomicNumber();
        G4double A = proj->GetPDGMass()/amu_c2;
        G4cout << "Nuclear Recoil: type " << proj_PDGcode << " Z " << Z << " A " << A << G4endl;

        for (int i=0; i<100; i++) {
            G4double E = (i+1) * eV;
            G4double f = niel_partition->PartitionNIEL(E, fSilicon, Z, A);
            G4cout << "Lindhard scaling factor: " << " E " << E * eV << " f " << f << G4endl;
        }
    }
    */


}




void HVeVRunAction::EndOfRunAction(const G4Run* run)
{
    WriteNtuple();
    PrintRunInfo(run);
}


void HVeVRunAction::CreateNtuple()
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    analysisManager->SetVerboseLevel(1);

    G4String filename = HVeVConfigManager::GetHitRootFile();
    analysisManager->OpenFile(filename);

    // Create ntuple
    analysisManager->SetFirstNtupleId(1);
    analysisManager->CreateNtuple("InnerHits", "Inner channel hit information");
    analysisManager->CreateNtupleIColumn(1, "eventid");
    analysisManager->CreateNtupleIColumn(1, "trackid");
    analysisManager->CreateNtupleDColumn(1, "hittime");
    analysisManager->CreateNtupleDColumn(1, "Edep");
    analysisManager->CreateNtupleDColumn(1, "x");
    analysisManager->CreateNtupleDColumn(1, "y");
    analysisManager->CreateNtupleDColumn(1, "z");
    analysisManager->FinishNtuple(1);

    analysisManager->CreateNtuple("OuterHits", "Outer channel hit information");
    analysisManager->CreateNtupleIColumn(2, "eventid");
    analysisManager->CreateNtupleIColumn(2, "trackid");
    analysisManager->CreateNtupleDColumn(2, "hittime");
    analysisManager->CreateNtupleDColumn(2, "Edep");
    analysisManager->CreateNtupleDColumn(2, "x");
    analysisManager->CreateNtupleDColumn(2, "y");
    analysisManager->CreateNtupleDColumn(2, "z");
    analysisManager->FinishNtuple(2);

    G4bool primariesFlag = HVeVConfigManager::GetPrimariesFlag();
    //if (primariesFlag) {
        analysisManager->CreateNtuple("PrimaryPhonon", "Primary phonon information");
        analysisManager->CreateNtupleIColumn(3, "eventid");
        analysisManager->CreateNtupleIColumn(3, "trackid");
        analysisManager->CreateNtupleIColumn(3, "parentid");
        analysisManager->CreateNtupleIColumn(3, "parentpdg");
        analysisManager->CreateNtupleDColumn(3, "energy");
        analysisManager->CreateNtupleDColumn(3, "startx");
        analysisManager->CreateNtupleDColumn(3, "starty");
        analysisManager->CreateNtupleDColumn(3, "startz");
        analysisManager->FinishNtuple(3);

        analysisManager->CreateNtuple("PrimaryCharge", "Primary charge pair information");
        analysisManager->CreateNtupleIColumn(4, "eventid");
        analysisManager->CreateNtupleIColumn(4, "trackid");
        analysisManager->CreateNtupleIColumn(4, "parentid");
        analysisManager->CreateNtupleIColumn(4, "parentpdg");
        analysisManager->CreateNtupleDColumn(4, "energy");
        analysisManager->CreateNtupleDColumn(4, "startx");
        analysisManager->CreateNtupleDColumn(4, "starty");
        analysisManager->CreateNtupleDColumn(4, "startz");
        analysisManager->FinishNtuple(4);

        analysisManager->CreateNtuple("LukePhonon", "Luke phonon information");
        analysisManager->CreateNtupleIColumn(5, "eventid");
        analysisManager->CreateNtupleIColumn(5, "trackid");
        analysisManager->CreateNtupleIColumn(5, "parentid");
        analysisManager->CreateNtupleIColumn(5, "parentpdg");
        analysisManager->CreateNtupleDColumn(5, "energy");
        analysisManager->CreateNtupleDColumn(5, "starttime");
        analysisManager->CreateNtupleDColumn(5, "startx");
        analysisManager->CreateNtupleDColumn(5, "starty");
        analysisManager->CreateNtupleDColumn(5, "startz");
        analysisManager->FinishNtuple(5);

        analysisManager->CreateNtuple("Primaries", "Primary information");
        analysisManager->CreateNtupleIColumn(6, "eventid");
        analysisManager->CreateNtupleIColumn(6, "trackid");
        analysisManager->CreateNtupleIColumn(6, "pdgcode");
        analysisManager->CreateNtupleIColumn(6, "parentid");
        analysisManager->CreateNtupleDColumn(6, "energy");
        analysisManager->CreateNtupleDColumn(6, "startx");
        analysisManager->CreateNtupleDColumn(6, "starty");
        analysisManager->CreateNtupleDColumn(6, "startz");
        analysisManager->CreateNtupleDColumn(6, "px");
        analysisManager->CreateNtupleDColumn(6, "py");
        analysisManager->CreateNtupleDColumn(6, "pz");
        analysisManager->CreateNtupleDColumn(6, "time");
        analysisManager->CreateNtupleDColumn(6, "endx");
        analysisManager->CreateNtupleDColumn(6, "endy");
        analysisManager->CreateNtupleDColumn(6, "endz");
        analysisManager->FinishNtuple(6);
    //}
    
    G4bool bottom_surface_hits_flag = HVeVConfigManager::GetBottomSurfaceHitsFlag();
    //if (bottom_surface_hits_flag) {
        analysisManager->CreateNtuple("BottomHits", "Bottom surface hits information (if recorded)");
        analysisManager->CreateNtupleIColumn(7, "eventid");
        analysisManager->CreateNtupleIColumn(7, "trackid");
        analysisManager->CreateNtupleDColumn(7, "hittime");
        analysisManager->CreateNtupleDColumn(7, "Edep");
        analysisManager->CreateNtupleDColumn(7, "x");
        analysisManager->CreateNtupleDColumn(7, "y");
        analysisManager->CreateNtupleDColumn(7, "z");
        analysisManager->FinishNtuple(7);
    //}

    analysisManager->CreateNtuple("RecombinePhonon", "Recombine phonon information");
    analysisManager->CreateNtupleIColumn(8, "eventid");
    analysisManager->CreateNtupleIColumn(8, "trackid");
    analysisManager->CreateNtupleDColumn(8, "energy");
    analysisManager->CreateNtupleDColumn(8, "starttime");
    analysisManager->CreateNtupleDColumn(8, "startx");
    analysisManager->CreateNtupleDColumn(8, "starty");
    analysisManager->CreateNtupleDColumn(8, "startz");
    analysisManager->FinishNtuple(8);

    analysisManager->CreateNtuple("EnergyDeposit", "Energy deposition Info");
    analysisManager->CreateNtupleIColumn(9, "eventid");
    analysisManager->CreateNtupleDColumn(9, "Edep");
    analysisManager->FinishNtuple(9);

}

void HVeVRunAction::WriteNtuple()
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    analysisManager->Write();
    analysisManager->CloseFile();
}

void HVeVRunAction::PrintRunInfo(const G4Run* run)
{
    G4cout << "================= Run is = "
           << run->GetRunID() << " =================" << G4endl;
    G4cout << "================= Run type is = "
           << G4RunManager::GetRunManager()->GetRunManagerType() << " =================" << G4endl;
    G4cout << "================= Event to be processed = "
           << run->GetNumberOfEventToBeProcessed() << " =================" << G4endl;
    G4cout << "================= NEvent = "
           << run->GetNumberOfEvent() << " =================" << G4endl;
}
