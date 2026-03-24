#ifndef HVeVActionInitialization_hh
#define HVeVActionInitialization_hh 1

#include "G4VUserActionInitialization.hh"

class HVeVActionInitialization : public G4VUserActionInitialization {
public:
  HVeVActionInitialization() {;}
  virtual ~HVeVActionInitialization() {;}
  virtual void Build() const;
};

#endif	