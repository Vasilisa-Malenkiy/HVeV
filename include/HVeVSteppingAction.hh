#ifndef HVeVSteppingAction_hh
#define HVeVSteppingAction_hh 1

class G4CMPProcessUtils;

#include "G4UserSteppingAction.hh"

class HVeVSteppingAction : public G4UserSteppingAction
{
    public:
        HVeVSteppingAction();
        virtual ~HVeVSteppingAction();

        virtual void UserSteppingAction(const G4Step*);
    
    private:
        G4CMPProcessUtils* process_utils;


};

#endif