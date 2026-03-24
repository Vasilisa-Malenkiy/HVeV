#ifndef HVeVRunAction_hh
#define HVeVRunAction_hh 1

#include "G4UserRunAction.hh"
#include "globals.hh"


class G4Run;


class HVeVRunAction : public G4UserRunAction
{
    public:
        HVeVRunAction();
        virtual ~HVeVRunAction();

        virtual void BeginOfRunAction(const G4Run*);
        virtual void EndOfRunAction(const G4Run*);

    private:
        void CreateNtuple();
        void WriteNtuple();

        void PrintRunInfo(const G4Run*);


};
#endif