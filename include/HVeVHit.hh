#ifndef HVeVHit_h
#define HVeVHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class G4AttDef;
class G4AttValue;

// It will record which readout channel the hit is registered.

class HVeVHit : public G4VHit
{

    public:
        HVeVHit();
        HVeVHit(G4int channelID); // 0 for inner top,
                                  // 1 for outer top,
                                  // 2 for bottom,
        HVeVHit(const HVeVHit &right);
        virtual ~HVeVHit();

        const HVeVHit& operator=(const HVeVHit& right);
        G4bool operator==(const HVeVHit &right) const;

        inline void *operator new(size_t);
        inline void operator delete(void *aHit);

        virtual void Draw();
        virtual const std::map<G4String, G4AttDef>* GetAttDefs() const;
        virtual std::vector<G4AttValue>* CreateAttValues() const;
        virtual void Print();

        void SetChannelID(G4int z) { fChannelID = z; }
        G4int GetChannelID() const { return fChannelID; }

        void SetTime(G4double t) { fTime = t; }
        G4double GetTime() const { return fTime; }
    
        void SetPosition(G4ThreeVector xyz) { fPos = xyz; }
        G4ThreeVector GetPosition() const { return fPos; }

    private:
        G4int fChannelID;
        G4double fTime;
        G4ThreeVector fPos;
};

using HVeVHitsCollection = G4THitsCollection<HVeVHit>;

extern G4ThreadLocal G4Allocator<HVeVHit>* HVeVHitAllocator;

inline void* HVeVHit::operator new(size_t)
{
    if (!HVeVHitAllocator) {
        HVeVHitAllocator = new G4Allocator<HVeVHit>;
    }
    return (void*)HVeVHitAllocator->MallocSingle();
}

inline void HVeVHit::operator delete(void* aHit)
{
    HVeVHitAllocator->FreeSingle((HVeVHit*) aHit);
}


#endif