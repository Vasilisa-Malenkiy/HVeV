#include "HVeVHit.hh"

#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4Circle.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4UIcommand.hh"
#include "G4ios.hh"

G4ThreadLocal G4Allocator<HVeVHit>* HVeVHitAllocator;

HVeVHit::HVeVHit()
: G4VHit(),
  fChannelID(-1), fTime(0.), fPos(0)
{}

HVeVHit::HVeVHit(G4int channelID)
: G4VHit(),
  fChannelID(channelID), fTime(0.), fPos(0)
{}

HVeVHit::~HVeVHit()
{}

HVeVHit::HVeVHit(const HVeVHit &right)
: G4VHit(),
  fChannelID(right.fChannelID),
  fTime(right.fTime),
  fPos(right.fPos)
{}  

const HVeVHit& HVeVHit::operator=(const HVeVHit &right)
{
  fChannelID = right.fChannelID;
  fTime = right.fTime;
  fPos = right.fPos;
  return *this;
}

G4bool HVeVHit::operator==(const HVeVHit &/*right*/) const
{
  return false;
}

void HVeVHit::Draw()
{
  auto visManager = G4VVisManager::GetConcreteInstance();
  if (! visManager) return;

  G4Circle circle(fPos);
  circle.SetScreenSize(2);
  circle.SetFillStyle(G4Circle::filled);
  G4Colour colour(1.,1.,0.);
  G4VisAttributes attribs(colour);
  circle.SetVisAttributes(attribs);
  visManager->Draw(circle);
}

const std::map<G4String,G4AttDef>* HVeVHit::GetAttDefs() const
{
  G4bool isNew;
  auto store = G4AttDefStore::GetInstance("HVeVHit",isNew);

  if (isNew) {
      (*store)["HitType"]
        = G4AttDef("HitType","Hit Type","Physics","","G4String");

      (*store)["ID"]
        = G4AttDef("ID","ID","Physics","","G4int");

      (*store)["Time"]
        = G4AttDef("Time","Time","Physics","G4BestUnit","G4double");

      (*store)["Pos"]
        = G4AttDef("Pos", "Position", "Physics","G4BestUnit","G4ThreeVector");
  }

  return store;
}

std::vector<G4AttValue>* HVeVHit::CreateAttValues() const
{
  auto values = new std::vector<G4AttValue>;

  values
    ->push_back(G4AttValue("HitType","DriftChamberHit",""));
  values
    ->push_back(G4AttValue("ID",G4UIcommand::ConvertToString(fChannelID),""));
  values
    ->push_back(G4AttValue("Time",G4BestUnit(fTime,"Time"),""));
  values
    ->push_back(G4AttValue("Pos",G4BestUnit(fPos,"Length"),""));

  return values;
}

void HVeVHit::Print()
{
  G4cout << "  Channel [" << fChannelID << "] : time " << fTime/ns
  << " (nsec) --- (x,y) " << fPos.x()
  << G4endl;
}

