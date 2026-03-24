#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"

#include "G4CMPPhysicsList.hh"
#include "G4CMPPhysics.hh"
#include "FTFP_BERT.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"

#include "G4CMPConfigManager.hh"
#include "HVeVActionInitialization.hh"
#include "HVeVConfigManager.hh"
#include "HVeVDetectorConstruction.hh"

int main(int argc,char** argv)
{
 // Construct the run manager
 //
 G4RunManager * runManager = new G4RunManager;

 // Set mandatory initialization classes
 //
 HVeVDetectorConstruction* detector = new HVeVDetectorConstruction();
 runManager->SetUserInitialization(detector);

 // Old codes where only G4CMPPhysicsList was registered.
 //G4VUserPhysicsList* physics = new G4CMPPhysicsList();
 //physics->SetCuts();
 //runManager->SetUserInitialization(physics);

 FTFP_BERT* physics = new FTFP_BERT;  
 physics->RegisterPhysics(new G4CMPPhysics);
 physics->RegisterPhysics(new G4DecayPhysics);
 physics->RegisterPhysics(new G4RadioactiveDecayPhysics);
 physics->RegisterPhysics(new G4EmStandardPhysics);
 physics->SetCuts();
 runManager->SetUserInitialization(physics);
 
 // Set user action classes (different for Geant4 10.0)
 //
 runManager->SetUserInitialization(new HVeVActionInitialization);

 // Create configuration managers to ensure macro commands exist
 G4CMPConfigManager::Instance();
 HVeVConfigManager::Instance();

 

 // Visualization manager
 //
 G4VisManager* visManager = new G4VisExecutive;
 visManager->Initialize();
 
 // Get the pointer to the User Interface manager
 //
 G4UImanager* UImanager = G4UImanager::GetUIpointer();  

 if (argc==1)   // Define UI session for interactive mode
 {
      G4UIExecutive * ui = new G4UIExecutive(argc,argv);
      ui->SessionStart();
      delete ui;
 }
 else           // Batch mode
 {
   G4String command = "/control/execute ";
   G4String fileName = argv[1];
   UImanager->ApplyCommand(command+fileName);
 }

 delete visManager;
 delete runManager;

 return 0;
}


