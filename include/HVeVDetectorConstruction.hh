#ifndef HVeVDetectorConstruction_h
#define HVeVDetectorConstruction_h 1

#include "CLHEP/Units/SystemOfUnits.h"
#include "G4VUserDetectorConstruction.hh"

class G4Material;
class G4VPhysicalVolume;
class G4CMPSurfaceProperty;
class G4ElectricField;
class HVeVSensitivity;

class HVeVDetectorConstruction : public G4VUserDetectorConstruction 
{
    public:
        HVeVDetectorConstruction();
        virtual ~HVeVDetectorConstruction();

    public:
        virtual G4VPhysicalVolume* Construct();

    private:
        void DefineMaterials();
        void SetupGeometry();
        void AttachPhononSensor(G4CMPSurfaceProperty* surfProp);
        void AttachField(G4LogicalVolume* lv);
        // Implemented this to describe the bottom aluminum grid
        void AttachGrid(G4CMPSurfaceProperty* surfProp);

    private:
        G4Material* fLiquidHelium;
        G4Material* fSilicon;
        G4Material* fAluminum;
        G4VPhysicalVolume* fWorldPhys;
        G4CMPSurfaceProperty* topSurfProp;
        G4CMPSurfaceProperty* botSurfProp;
        G4CMPSurfaceProperty* wallSurfProp;
        HVeVSensitivity*      electrodeSensitivity;

    private:
        // Geometry of silicon substrate
        G4double dp_siliconChipDimX = 10.0 * CLHEP::mm;
        G4double dp_siliconChipDimY = 10.0 * CLHEP::mm;
        G4double dp_siliconChipDimZ =  4.0 * CLHEP::mm;

        // Geometry of aluminum superconducting film
        G4double dp_aluminumFilmDimX = dp_siliconChipDimX;
        G4double dp_aluminumFilmDimY = dp_siliconChipDimY;
        G4double dp_aluminumFilmDimZ = 100 * CLHEP::nm;

        // Geometry of two aluminum superconducting readout channels on the top surface
        // The two Al readout channels have the same coverage areas
        G4double dp_aluminumInnerTopFilmDimX = dp_siliconChipDimX * 0.7071;
        G4double dp_aluminumInnerTopFilmDimY = dp_siliconChipDimY * 0.7071;
        G4double dp_aluminumTopFilmDimZ = 600 * CLHEP::nm;
        // Geometry thickness — keep this FIXED for all runs
        //G4double dp_aluminumBottomFilmDimZ = 1 * CLHEP::nm; // thin enough to be geometrically invisible
        // COUPLED SCAN
        G4double dp_aluminumBottomFilmDimZ = 10.*CLHEP::nm;
        G4double dp_aluminumBottomFilmThickness = dp_aluminumBottomFilmDimZ;
        // KaplanQP physics thickness — this is what you vary between runs
        //G4double dp_aluminumBottomFilmThickness = 50.*CLHEP::nm;  // vary this: 10, 30, 60, 100, 200, 400, 600 nm

        G4bool fConstructed;
        G4ElectricField* fEMField;
        // Simulation parameters:
        G4double voltage;           // voltage across the silicon substrate
        G4String outputFileName;    // output hit filename
        G4String outputRootFileName; 
};

#endif
