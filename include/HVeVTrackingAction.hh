#ifndef HVeVTrackingAction_h
#define HVeVTrackingAction_h 1

#include "G4UserTrackingAction.hh"

class HVeVTrackingAction : public G4UserTrackingAction
{
    public:
        HVeVTrackingAction();
        ~HVeVTrackingAction() {};
    
        virtual void  PreUserTrackingAction(const G4Track*);
        virtual void PostUserTrackingAction(const G4Track*);
};
#endif