#ifndef HVeVConfigMessenger_hh
#define HVeVConfigMessenger_hh 1

#include "G4UImessenger.hh"

class HVeVConfigManager;
class G4UIcommand;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithABool;

class HVeVConfigMessenger : public G4UImessenger {
    public:
        HVeVConfigMessenger(HVeVConfigManager* theData);
        virtual ~HVeVConfigMessenger();

        void SetNewValue(G4UIcommand* cmd, G4String value);

    private:
        HVeVConfigManager* theManager;
        G4UIcmdWithAString* hitsCmd;
        G4UIcmdWithAString* hitsRootfileCmd;
        G4UIcmdWithADoubleAndUnit* voltageCmd;
        G4UIcmdWithAString* primaryParticleNameCmd;
        G4UIcmdWithADoubleAndUnit* primaryParticleEnergyCmd;
        G4UIcmdWithAString* primaryParticlePosTypeCmd;
        G4UIcmdWithADoubleAndUnit* primaryParticlePosCenterXCmd;
        G4UIcmdWithADoubleAndUnit* primaryParticlePosCenterYCmd;
        G4UIcmdWithADoubleAndUnit* primaryParticlePosCenterZCmd;
        G4UIcmdWithADoubleAndUnit* primaryParticlePosHalfXCmd;
        G4UIcmdWithADoubleAndUnit* primaryParticlePosHalfYCmd;
        G4UIcmdWithADoubleAndUnit* primaryParticlePosHalfZCmd;

        G4UIcmdWithABool* primariesFlagCmd;
        G4UIcmdWithABool* bottomSurfaceHitsFlagCmd;

    private:
        HVeVConfigMessenger(const HVeVConfigMessenger&); // Copying is forbidden
        HVeVConfigMessenger& operator=(const HVeVConfigMessenger&);

};

#endif

