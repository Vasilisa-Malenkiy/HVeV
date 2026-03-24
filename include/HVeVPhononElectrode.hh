#ifndef HVeVPhononElectrode_hh
#define HVeVPhononElectrode_hh 1

#include "G4CMPVElectrodePattern.hh"
#include "HVeVKaplanQP.hh"

class G4CMPKaplanQP;
class HVeVKaplanQP;
class G4ParticleChange;
class G4Step;
class G4Track;


class HVeVPhononElectrode : public G4CMPVElectrodePattern {
public:
  HVeVPhononElectrode();
  virtual ~HVeVPhononElectrode();

  // Implement cloning function along with default copiers
  HVeVPhononElectrode(const HVeVPhononElectrode&) = default;
  HVeVPhononElectrode(HVeVPhononElectrode&&) = default;
  HVeVPhononElectrode& operator=(const HVeVPhononElectrode&) = default;
  HVeVPhononElectrode& operator=(HVeVPhononElectrode&&) = default;

  virtual G4CMPVElectrodePattern* Clone() const {
    return new HVeVPhononElectrode(*this);
  }

  // Assumes that user has configured a border surface only at sensor pads
  virtual G4bool IsNearElectrode(const G4Step&) const;

  virtual void AbsorbAtElectrode(const G4Track&,
                                 const G4Step&,
                                 G4ParticleChange&) const;

  int GetVerboseLevel() const { return verboseLevel; }

protected:
  // Record energy deposition and re-emitted energies as secondary phonons
  void ProcessAbsorption(const G4Track& track, const G4Step& step,
			 G4double EDep, G4ParticleChange& particleChange) const;

  // Reflect phonon by changing direction randomly, no energy deposition
  void ProcessReflection(const G4Track& track, const G4Step& step,
			 G4ParticleChange& particleChange) const;

  // NOTE: "Mutable" because AbsorbAtElectrode() function is const
  //mutable G4CMPKaplanQP* kaplanQP;	// Create instance of QET simulator
  mutable HVeVKaplanQP* kaplanQP;	// Create instance of QET simulator
  mutable std::vector<G4double> phononEnergies;		// Reusable buffer
};

#endif