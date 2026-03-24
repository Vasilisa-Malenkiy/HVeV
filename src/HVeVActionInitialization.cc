
#include "HVeVActionInitialization.hh"
#include "HVeVPrimaryGeneratorAction.hh"
#include "G4CMPStackingAction.hh"

#include "HVeVRunAction.hh"
#include "HVeVEventAction.hh"
#include "HVeVTrackingAction.hh"
#include "HVeVSteppingAction.hh"

void HVeVActionInitialization::Build() const {
  SetUserAction(new HVeVPrimaryGeneratorAction);
  SetUserAction(new G4CMPStackingAction);

  SetUserAction(new HVeVRunAction());
  SetUserAction(new HVeVEventAction());
  SetUserAction(new HVeVTrackingAction());
  SetUserAction(new HVeVSteppingAction());
} 
