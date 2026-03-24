#ifndef HVeVPrimaryGeneratorAction_h
#define HVeVPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4GeneralParticleSource;
class G4Event;

class HVeVPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
    public:
        HVeVPrimaryGeneratorAction();
        virtual ~HVeVPrimaryGeneratorAction();
    
    public:
        virtual void GeneratePrimaries(G4Event*);
    
    private:
        G4GeneralParticleSource* fParticleGPS;

};
#endif