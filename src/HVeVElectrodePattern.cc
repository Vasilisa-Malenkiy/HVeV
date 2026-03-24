#include "HVeVElectrodePattern.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4AffineTransform.hh"
#include "G4SystemOfUnits.hh"

// construct and destructor
HVeVElectrodePattern::HVeVElectrodePattern()
: G4CMPVElectrodePattern() {;}

HVeVElectrodePattern::IsNearElectrode(const G4Step& aStep) const{
    if (verboseLevel)
    G4cout << "HVeVElectrodePattern::IsNearElectrode" << G4endl;

    const G4VTouchable* touch = aStep.GetPreStepPoint()->GetTouchable();    
    G4AffineTransform toGlobal(touch->GetRotation(), touch->GetTranslation());
    G4AffineTransform toLocal = toGlobal.Inverse();

    G4ThreeVector pos = aStep.GetPreStepPoint()->GetPosition();
    toLocal.ApplyPointTransform(pos);

    
}