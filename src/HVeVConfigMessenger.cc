#include "HVeVConfigMessenger.hh"
#include "HVeVConfigManager.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4SystemOfUnits.hh"

HVeVConfigMessenger::HVeVConfigMessenger(HVeVConfigManager* mgr)
    : G4UImessenger("/hvev/", "User configuration for HVeV example"),
    theManager(mgr)
    , hitsCmd(0)
    , hitsRootfileCmd(0)
    , voltageCmd(0)
    , primaryParticleNameCmd(0)
    , primaryParticleEnergyCmd(0)
    , primariesFlagCmd(0)
    , bottomSurfaceHitsFlagCmd(0)
{
    hitsCmd = CreateCommand<G4UIcmdWithAString>("HitsFile",
        "Set filename for output of phonon hit locations");
    
    hitsRootfileCmd = CreateCommand<G4UIcmdWithAString>("HitsRootFile",
        "Set rootfile filename for output of phonon hits.");
    
    voltageCmd = CreateCommand<G4UIcmdWithADoubleAndUnit>("voltage",
        "Set voltage for uniform electric field");
    voltageCmd->SetUnitCategory("Electric potential");

    primaryParticleNameCmd = CreateCommand<G4UIcmdWithAString>("PrimaryParticleName",
        "Set name of primary particle");
    
    primaryParticleEnergyCmd = CreateCommand<G4UIcmdWithADoubleAndUnit>("PrimaryParticleEnergy",
        "Set energy of primary particle");

    primaryParticlePosTypeCmd = CreateCommand<G4UIcmdWithAString>("PrimaryParticlePosType",
        "Set type of primary particle position distribution");
    
    primaryParticlePosCenterXCmd = CreateCommand<G4UIcmdWithADoubleAndUnit>("PrimaryParticlePosCenterX",
        "Set center X coordinate of primary particle position distribution");

    primaryParticlePosCenterYCmd = CreateCommand<G4UIcmdWithADoubleAndUnit>("PrimaryParticlePosCenterY",
        "Set center Y coordinate of primary particle position distribution");

    primaryParticlePosCenterZCmd = CreateCommand<G4UIcmdWithADoubleAndUnit>("PrimaryParticlePosCenterZ",
        "Set center Z coordinate of primary particle position distribution");
    
    primaryParticlePosHalfXCmd = CreateCommand<G4UIcmdWithADoubleAndUnit>("PrimaryParticlePosHalfX",
        "Set half X size of primary particle position distribution");
    
    primaryParticlePosHalfYCmd = CreateCommand<G4UIcmdWithADoubleAndUnit>("PrimaryParticlePosHalfY",
        "Set half Y size of primary particle position distribution");
    
    primaryParticlePosHalfZCmd = CreateCommand<G4UIcmdWithADoubleAndUnit>("PrimaryParticlePosHalfZ",
        "Set half Z size of primary particle position distribution");

    primariesFlagCmd = CreateCommand<G4UIcmdWithABool>("PrimariesFlag",
        "Set flag for primary particle information saving.");
    
    bottomSurfaceHitsFlagCmd = CreateCommand<G4UIcmdWithABool>("BottomSurfaceHitsFlag",
        "Set flag for bottom surface hits information saving.");
    
}

HVeVConfigMessenger::~HVeVConfigMessenger() {
    delete hitsCmd; hitsCmd = 0;
    delete hitsRootfileCmd; hitsRootfileCmd = 0;
    delete voltageCmd; voltageCmd = 0;
    delete primaryParticleNameCmd; primaryParticleNameCmd = 0;
    delete primaryParticleEnergyCmd; primaryParticleEnergyCmd = 0;
    delete primaryParticlePosTypeCmd; primaryParticlePosTypeCmd = 0;
    delete primaryParticlePosCenterXCmd; primaryParticlePosCenterXCmd = 0;
    delete primaryParticlePosCenterYCmd; primaryParticlePosCenterYCmd = 0;
    delete primaryParticlePosCenterZCmd; primaryParticlePosCenterZCmd = 0;
    delete primaryParticlePosHalfXCmd; primaryParticlePosHalfXCmd = 0;
    delete primaryParticlePosHalfYCmd; primaryParticlePosHalfYCmd = 0;
    delete primaryParticlePosHalfZCmd; primaryParticlePosHalfZCmd = 0;
    delete primariesFlagCmd; primariesFlagCmd = 0;
    delete bottomSurfaceHitsFlagCmd; bottomSurfaceHitsFlagCmd = 0;
}

void HVeVConfigMessenger::SetNewValue(G4UIcommand* cmd, G4String value) {
    if (cmd == hitsCmd) theManager->SetHitOutput(value);

    if (cmd == hitsRootfileCmd) theManager->SetHitRootFile(value);

    //if (cmd == voltageCmd) {theManager->SetVoltage(voltageCmd->GetNewDoubleValue(value)/volt);}
    if (cmd == voltageCmd) theManager->SetVoltage(voltageCmd->GetNewDoubleValue(value));

    if (cmd == primaryParticleNameCmd) theManager->SetPrimaryParticleName(value);

    if (cmd == primaryParticleEnergyCmd) theManager->SetPrimaryParticleEnergy(primaryParticleEnergyCmd->GetNewDoubleValue(value)/eV);

    if (cmd == primaryParticlePosTypeCmd) theManager->SetPrimaryParticlePosType(value);

    if (cmd == primaryParticlePosCenterXCmd) theManager->SetPrimaryParticlePosCenterX(primaryParticlePosCenterXCmd->GetNewDoubleValue(value)/mm);

    if (cmd == primaryParticlePosCenterYCmd) theManager->SetPrimaryParticlePosCenterY(primaryParticlePosCenterYCmd->GetNewDoubleValue(value)/mm);

    if (cmd == primaryParticlePosCenterZCmd) theManager->SetPrimaryParticlePosCenterZ(primaryParticlePosCenterZCmd->GetNewDoubleValue(value)/mm);

    if (cmd == primaryParticlePosHalfXCmd) theManager->SetPrimaryParticlePosHalfX(primaryParticlePosHalfXCmd->GetNewDoubleValue(value)/mm);

    if (cmd == primaryParticlePosHalfYCmd) theManager->SetPrimaryParticlePosHalfY(primaryParticlePosHalfYCmd->GetNewDoubleValue(value)/mm);

    if (cmd == primaryParticlePosHalfZCmd) theManager->SetPrimaryParticlePosHalfZ(primaryParticlePosHalfZCmd->GetNewDoubleValue(value)/nm);

    if (cmd == primariesFlagCmd) theManager->SetPrimariesFlag(primariesFlagCmd->GetNewBoolValue(value));

    if (cmd == bottomSurfaceHitsFlagCmd) theManager->SetBottomSurfaceHitsFlag(bottomSurfaceHitsFlagCmd->GetNewBoolValue(value));
}
