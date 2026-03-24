#ifndef HVeVElectrodePattern_h
#define HVeVElectrodePattern_h 1

#include "G4CMPVElectrodePattern.hh"

class HVeVElectrodePattern : public G4CMPVElectrodePattern
{
    public:
        HVeVElectrodePattern();
        virtual ~HVeVElectrodePattern();

        HVeVElectrodePattern(const HVeVElectrodePattern&) = default;
        HVeVElectrodePattern(HVeVElectrodePattern&&) = default;
        HVeVElectrodePattern& operator=(const HVeVElectrodePattern&) = default;
        HVeVElectrodePattern& operator=(HVeVElectrodePattern&&) = default;  

        virtual G4CMPVElectrodePattern* Clone() const {
            return new HVeVElectrodePattern(*this);
        }

        virtual G4bool IsNearElectrode(const G4Step& aStep) const;
        virtual void AbsorbAtElectrode(const G4Track& aTrack, const G4Step& aStep, G4ParticleChange& aParticleChange) const;
};


#endif