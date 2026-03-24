#include "HVeVConfigManager.hh"
#include "HVeVConfigMessenger.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include <stdlib.h>

HVeVConfigManager* HVeVConfigManager::theInstance = 0;

HVeVConfigManager* HVeVConfigManager::Instance() {
  if (!theInstance) theInstance = new HVeVConfigManager;
  return theInstance;
}

HVeVConfigManager::HVeVConfigManager()
  : Hit_file(getenv("HVeV_HIT_FILE")?getenv("HVeV_HIT_FILE"):"phonon_hits.txt"),
    Hit_root_file(getenv("HVeV_HIT_ROOT_FILE")?getenv("HVeV_HIT_ROOT_FILE"):"test.root"),
    voltage(getenv("HVeV_VOLTAGE")?strtod(getenv("HVeV_VOLTAGE"),0)*volt:0.),
    PrimaryParticleName(getenv("HVeV_PRIMARY_PARTICLE_NAME")?getenv("HVeV_PRIMARY_PARTICLE_NAME"):"e-"),
    PrimaryParticleEnergy(getenv("HVeV_PRIMARY_PARTICLE_ENERGY")?strtod(getenv("HVeV_PRIMARY_PARTICLE_ENERGY"),0)*eV:1.*eV),
    PrimaryParticlePosType(getenv("HVeV_PRIMARY_PARTICLE_POS_TYPE")?getenv("HVeV_PRIMARY_PARTICLE_POS_TYPE"):"Point"),
    PrimaryParticlePosCenterX(getenv("HVeV_PRIMARY_PARTICLE_POS_CENTER_X")?strtod(getenv("HVeV_PRIMARY_PARTICLE_POS_CENTER_X"),0)*mm:0.*mm),
    PrimaryParticlePosCenterY(getenv("HVeV_PRIMARY_PARTICLE_POS_CENTER_Y")?strtod(getenv("HVeV_PRIMARY_PARTICLE_POS_CENTER_Y"),0)*mm:0.*mm),
    PrimaryParticlePosCenterZ(getenv("HVeV_PRIMARY_PARTICLE_POS_CENTER_Z")?strtod(getenv("HVeV_PRIMARY_PARTICLE_POS_CENTER_Z"),0)*mm:0.*mm),
    PrimaryParticlePosHalfX(getenv("HVeV_PRIMARY_PARTICLE_POS_HALF_X")?strtod(getenv("HVeV_PRIMARY_PARTICLE_POS_HALF_X"),0)*mm:5.0*mm),
    PrimaryParticlePosHalfY(getenv("HVeV_PRIMARY_PARTICLE_POS_HALF_Y")?strtod(getenv("HVeV_PRIMARY_PARTICLE_POS_HALF_Y"),0)*mm:5.0*mm),
    PrimaryParticlePosHalfZ(getenv("HVeV_PRIMARY_PARTICLE_POS_HALF_Z")?strtod(getenv("HVeV_PRIMARY_PARTICLE_POS_HALF_Z"),0)*mm:10.*nm),
    primaries_flag(getenv("HVeV_PRIMARY_PARTICLES_FLAG")?strtod(getenv("HVeV_PRIMARY_PARTICLES_FLAG"),0):0),
    messenger(new HVeVConfigMessenger(this)) {;}

HVeVConfigManager::~HVeVConfigManager() {
    delete messenger; messenger=0;
}

void HVeVConfigManager::UpdateGeometry() {
    G4RunManager::GetRunManager()->ReinitializeGeometry(true);
}
