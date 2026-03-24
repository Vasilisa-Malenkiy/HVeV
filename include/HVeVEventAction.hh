#ifndef HVeVEventAction_h
#define HVeVEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class HVeVEventAction : public G4UserEventAction
{
    public:
        HVeVEventAction();
        ~HVeVEventAction() {};

    public:
        virtual void BeginOfEventAction(const G4Event*);
        virtual void   EndOfEventAction(const G4Event*);

    public:
        double edep;

        void SetEdepOfThisEvent(double val) {edep = val;}
        double GetEdepOfThisEvent() {return edep;}
    
};

#endif
