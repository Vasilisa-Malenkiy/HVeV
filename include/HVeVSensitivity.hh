#ifndef HVeVSensitivity_h
#define HVeVSensitivity_h 1

#include "G4CMPElectrodeSensitivity.hh"

class HVeVSensitivity final : public G4CMPElectrodeSensitivity {
    public:
        HVeVSensitivity(G4String name);
        virtual ~HVeVSensitivity();

    public:
        HVeVSensitivity(const HVeVSensitivity&) = delete;
        HVeVSensitivity& operator=(const HVeVSensitivity&) = delete;

        HVeVSensitivity(HVeVSensitivity&&) = delete;
        HVeVSensitivity& operator=(HVeVSensitivity&&) = delete;

        virtual void EndOfEvent(G4HCofThisEvent*);

        void SetOutputFile(const G4String& fn);

    protected:
        virtual G4bool IsHit(const G4Step*, const G4TouchableHistory*) const;

    private:
        std::ofstream output;
        G4String fileName;
};

#endif