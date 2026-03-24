#ifndef HVeVConfigManager_hh
#define HVeVConfigManager_hh 1

#include "globals.hh"

class HVeVConfigMessenger;

class HVeVConfigManager {
    public:
        ~HVeVConfigManager();
        static HVeVConfigManager* Instance();

        // Getter
        static const G4String& GetHitOutput() { return Instance()->Hit_file; }
        static const G4String& GetHitRootFile() { return Instance()->Hit_root_file; }
        static G4double GetVoltage() { return Instance()->voltage; }
        static const G4String& GetPrimaryParticleName() { return Instance()->PrimaryParticleName; }
        static G4double GetPrimaryParticleEnergy() { return Instance()->PrimaryParticleEnergy; }
        static G4bool GetPrimariesFlag() { return Instance()->primaries_flag; }
        static G4bool GetBottomSurfaceHitsFlag() { return Instance()->bottom_surface_hits_flag; }
        static G4String GetPrimaryParticlePosType() { return Instance()->PrimaryParticlePosType; }
        static G4double GetPrimaryParticlePosCenterX() { return Instance()->PrimaryParticlePosCenterX; }
        static G4double GetPrimaryParticlePosCenterY() { return Instance()->PrimaryParticlePosCenterY; }
        static G4double GetPrimaryParticlePosCenterZ() { return Instance()->PrimaryParticlePosCenterZ; }
        static G4double GetPrimaryParticlePosHalfX() { return Instance()->PrimaryParticlePosHalfX; }
        static G4double GetPrimaryParticlePosHalfY() { return Instance()->PrimaryParticlePosHalfY; }
        static G4double GetPrimaryParticlePosHalfZ() { return Instance()->PrimaryParticlePosHalfZ; }

        // Setter
        static void SetHitOutput(const G4String& name) 
            { Instance()->Hit_file = name; UpdateGeometry(); }
        static void SetHitRootFile(const G4String& name) 
            { Instance()->Hit_root_file = name; UpdateGeometry(); }
        static void SetVoltage(G4double value) 
            { Instance()->voltage = value; UpdateGeometry(); }
        static void SetPrimaryParticleName(const G4String& name) 
            { Instance()->PrimaryParticleName = name; UpdateGeometry(); }
        static void SetPrimaryParticleEnergy(G4double value) 
            { Instance()->PrimaryParticleEnergy = value; UpdateGeometry(); }
        static void SetPrimaryParticlePosType(const G4String& type) 
            { Instance()->PrimaryParticlePosType = type; UpdateGeometry(); }
        static void SetPrimaryParticlePosCenterX(G4double value) 
            { Instance()->PrimaryParticlePosCenterX = value; UpdateGeometry(); }
        static void SetPrimaryParticlePosCenterY(G4double value) 
            { Instance()->PrimaryParticlePosCenterY = value; UpdateGeometry(); }
        static void SetPrimaryParticlePosCenterZ(G4double value) 
            { Instance()->PrimaryParticlePosCenterZ = value; UpdateGeometry(); }
        static void SetPrimaryParticlePosHalfX(G4double value) 
            { Instance()->PrimaryParticlePosHalfX = value; UpdateGeometry(); }
        static void SetPrimaryParticlePosHalfY(G4double value) 
            { Instance()->PrimaryParticlePosHalfY = value; UpdateGeometry(); }
        static void SetPrimaryParticlePosHalfZ(G4double value) 
            { Instance()->PrimaryParticlePosHalfZ = value; UpdateGeometry(); }

        static void SetPrimariesFlag(G4bool flag) 
            { Instance()->primaries_flag = flag; UpdateGeometry(); }
        static void SetBottomSurfaceHitsFlag(G4bool flag)
            { Instance()->bottom_surface_hits_flag = flag; UpdateGeometry(); }

        static void UpdateGeometry();

    private:
        HVeVConfigManager();
        HVeVConfigManager(const HVeVConfigManager&) = delete;
        HVeVConfigManager(HVeVConfigManager&&) = delete;
        HVeVConfigManager& operator=(const HVeVConfigManager&) = delete;
        HVeVConfigManager& operator=(HVeVConfigManager&&) = delete;

        static HVeVConfigManager* theInstance;

    private:
        G4String Hit_file;
        G4String Hit_root_file;
        G4double voltage;
        // Primary charge pair configurations
        G4String PrimaryParticleName;
        G4double PrimaryParticleEnergy;
        // somehow the gps commands do not work to specify the position, I have to code it here.
        G4String PrimaryParticlePosType;
        G4double PrimaryParticlePosCenterX;
        G4double PrimaryParticlePosCenterY;
        G4double PrimaryParticlePosCenterZ;
        G4double PrimaryParticlePosHalfX;
        G4double PrimaryParticlePosHalfY;
        G4double PrimaryParticlePosHalfZ;

        // If store primaries information in the event
        G4bool primaries_flag;
        G4bool bottom_surface_hits_flag;

        HVeVConfigMessenger* messenger;

};
#endif
