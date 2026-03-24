#include "globals.hh"
#include "HVeVKaplanQP.hh"
#include "G4CMPConfigManager.hh"
#include "G4CMPUtils.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "Randomize.hh"
#include <numeric>
#include <iostream>
#include <iomanip>

// Global function for Kaplan quasiparticle downconversion
G4double G4CMP::KaplanPhononQP(G4double energy,
                               G4MaterialPropertiesTable* prop,
                               std::vector<G4double>& reflectedEnergies) {
  G4cout << "!!! EXECUTING KAPLAN PHYSICS !!!" << G4endl;
  static G4ThreadLocal HVeVKaplanQP* theKaplanQP = new HVeVKaplanQP(prop);
  theKaplanQP->SetFilmProperties(prop);
  theKaplanQP->SetVerboseLevel(G4CMPConfigManager::GetVerboseLevel());

  // Removed unit division to bypass compiler errors
  G4cout << "\n[TOP-LEVEL] KaplanPhononQP Entry: E=" << energy << G4endl;

  G4double Edep = theKaplanQP->AbsorbPhonon(energy, reflectedEnergies);

  G4cout << "[TOP-LEVEL] Result: Total E_dep=" << Edep << " | "
         << "Reflected Count=" << reflectedEnergies.size() << G4endl;
  return Edep;
}

// Constructor
HVeVKaplanQP::HVeVKaplanQP(G4MaterialPropertiesTable* prop, G4int vb)
  : verboseLevel(vb), keepAllPhonons(true),
    filmProperties(nullptr), filmThickness(0.), gapEnergy(0.),
    lowQPLimit(3.), highQPLimit(0.), directAbsorption(0.), absorberGap(0.),
    absorberEff(1.), absorberEffSlope(0.), phononLifetime(0.),
    phononLifetimeSlope(0.), vSound(0.), temperature(0.) {
  if (prop) SetFilmProperties(prop);
}

HVeVKaplanQP::~HVeVKaplanQP() {}

// Set thin film material properties
void HVeVKaplanQP::SetFilmProperties(G4MaterialPropertiesTable* prop) {
  if (!prop) {
    G4Exception("HVeVKaplanQP::SetFilmProperties()", "G4CMP001",
                RunMustBeAborted, "Null MaterialPropertiesTable.");
  }

  bool missing = false;
  if (!prop->ConstPropertyExists("gapEnergy"))           missing = true;
  if (!prop->ConstPropertyExists("phononLifetime"))      missing = true;
  if (!prop->ConstPropertyExists("phononLifetimeSlope")) missing = true;
  if (!prop->ConstPropertyExists("vSound"))              missing = true;
  if (!prop->ConstPropertyExists("filmThickness"))       missing = true;

  if (missing) {
    G4Exception("HVeVKaplanQP::SetFilmProperties()", "G4CMP002",
                RunMustBeAborted, "Critical keys missing in MaterialPropertiesTable.");
  }

  if (filmProperties != prop) {
    filmThickness       = prop->GetConstProperty("filmThickness");
    gapEnergy           = prop->GetConstProperty("gapEnergy");
    phononLifetime      = prop->GetConstProperty("phononLifetime");
    phononLifetimeSlope = prop->GetConstProperty("phononLifetimeSlope");
    vSound              = prop->GetConstProperty("vSound");

    absorberEff = prop->ConstPropertyExists("absorberEff") ? prop->GetConstProperty("absorberEff") : 1.;
    absorberEffSlope = prop->ConstPropertyExists("absorberEffSlope") ? prop->GetConstProperty("absorberEffSlope") : 0.;
    lowQPLimit = prop->ConstPropertyExists("lowQPLimit") ? prop->GetConstProperty("lowQPLimit") : 3.;
    highQPLimit = prop->ConstPropertyExists("highQPLimit") ? prop->GetConstProperty("highQPLimit") : 0.;
    directAbsorption = prop->ConstPropertyExists("directAbsorption") ? prop->GetConstProperty("directAbsorption") : 0.;
    absorberGap = prop->ConstPropertyExists("absorberGap") ? prop->GetConstProperty("absorberGap") : 0.;
    temperature = prop->ConstPropertyExists("temperature") ? prop->GetConstProperty("temperature") : G4CMPConfigManager::GetTemperature();

    filmProperties = prop;

    G4cout << "--- FILM PROPERTIES LOADED ---" << G4endl;
    G4cout << " Thickness: " << filmThickness << G4endl;
    G4cout << " Gap Energy: " << gapEnergy << G4endl;
    G4cout << " vSound: " << vSound << G4endl;
    G4cout << " Phonon Lifetime: " << phononLifetime << G4endl;
    G4cout << "-------------------------------" << G4endl;
  }
}

// Calculate Escape Probability
G4double HVeVKaplanQP::CalcEscapeProbability(G4double energy, G4double frac) const {
  if (gapEnergy <= 0. || energy < gapEnergy) return 1.0;

  G4double path = frac * filmThickness;
  G4double mfp = vSound * phononLifetime / (1. + phononLifetimeSlope * (energy/gapEnergy - 2.));
  G4double prob = std::exp(-path/mfp);

  G4cout << "  [CALC] E=" << std::setw(6) << energy << " | "
         << "Path=" << std::setw(6) << path << " | "
         << "MFP=" << std::setw(6) << mfp << " | "
         << "EscapeProb=" << prob << G4endl;

  return prob;
}

// Main phonon absorption function
G4double HVeVKaplanQP::AbsorbPhonon(G4double energy, std::vector<G4double>& reflectedEnergies) const {
  if (!ParamsReady()) return 0.;
  if (!reflectedEnergies.empty()) reflectedEnergies.clear();
  keepAllPhonons = G4CMPConfigManager::KeepKaplanPhonons();

  if (DoDirectAbsorption(energy)) {
    G4cout << "  [EVENT] Direct Absorption triggered for E=" << energy << G4endl;
    return energy;
  }

  G4double frac = 4.0;
  G4double pEscape = CalcEscapeProbability(energy, frac);
  G4double roll = G4UniformRand();

  if (IsSubgap(energy) || roll <= pEscape) {
    G4cout << "  [EVENT] Phonon REFLECTED (Roll " << roll << " <= Prob " << pEscape << ")" << G4endl;
    reflectedEnergies.push_back(energy);
    return 0.;
  }

  G4cout << "  [EVENT] Phonon ABSORBED into film. Starting cascade..." << G4endl;

  G4double EDep = 0.;
  G4int nQPpairs = (highQPLimit > 0. ? std::ceil(energy/(2.*highQPLimit*gapEnergy)) : 1);

  qpEnergyList.clear();
  phononEnergyList.clear();
  phononEnergyList.resize(nQPpairs, energy/nQPpairs);

  while (!qpEnergyList.empty() || !phononEnergyList.empty()) {
    if (!phononEnergyList.empty()) EDep += CalcQPEnergies(phononEnergyList, qpEnergyList);
    if (!qpEnergyList.empty())     EDep += CalcPhononEnergies(phononEnergyList, qpEnergyList);
    if (!phononEnergyList.empty()) CalcReflectedPhononEnergies(phononEnergyList, reflectedEnergies);
  }

  return EDep;
}

void HVeVKaplanQP::CalcReflectedPhononEnergies(std::vector<G4double>& phonEnergies,
                                               std::vector<G4double>& reflectedEnergies) const {
  newPhonEnergies.clear();
  for (const G4double& E: phonEnergies) {
    if (G4CMP::IsThermalized(temperature, E)) continue;
 
    G4double frac = ((G4UniformRand()<0.5?0.5:1.5)/cos(G4UniformRand()*1.47));
    G4double pEscape = CalcEscapeProbability(E, frac);

    if (G4UniformRand() < pEscape) {
      G4cout << "  [CASCADE] Secondary Phonon Escaped: " << E << G4endl;
      reflectedEnergies.push_back(E);
    } else if (keepAllPhonons || !IsSubgap(E)) {
      newPhonEnergies.push_back(E);
    }
  }
  phonEnergies.swap(newPhonEnergies);
}

G4double HVeVKaplanQP::CalcQPEnergies(std::vector<G4double>& phonEnergies,
                                      std::vector<G4double>& qpEnergies) const {
  G4double EDep = 0.;
  newPhonEnergies.clear();
  for (const G4double& E: phonEnergies) {
    if (IsSubgap(E)) { newPhonEnergies.push_back(E); continue; }
    G4double qpE = QPEnergyRand(E);
    EDep += CalcQPAbsorption(qpE, newPhonEnergies, qpEnergies);
    EDep += CalcQPAbsorption(E-qpE, newPhonEnergies, qpEnergies);
  }
  phonEnergies.swap(newPhonEnergies);
  return EDep;
}

G4double HVeVKaplanQP::CalcPhononEnergies(std::vector<G4double>& phonEnergies,
                                          std::vector<G4double>& qpEnergies) const {
  G4double EDep = 0.;
  newQPEnergies.clear();
  for (const G4double& E: qpEnergies) {
    G4double phonE = PhononEnergyRand(E);
    G4double qpE = E - phonE;
    if (IsSubgap(phonE)) EDep += CalcDirectAbsorption(phonE, phonEnergies);
    else phonEnergies.push_back(phonE);
    EDep += CalcQPAbsorption(qpE, phonEnergies, newQPEnergies);
  }
  qpEnergies.swap(newQPEnergies);
  return EDep;
}

G4bool HVeVKaplanQP::DoDirectAbsorption(G4double energy) const {
  if (energy < 2.*absorberGap) return false;
  return (G4UniformRand() < directAbsorption);
}

G4double HVeVKaplanQP::CalcDirectAbsorption(G4double energy, std::vector<G4double>& keepEnergies) const {
  if (DoDirectAbsorption(energy)) return energy;
  keepEnergies.push_back(energy);
  return 0.;
}

G4double HVeVKaplanQP::CalcQPAbsorption(G4double qpE,
                                        std::vector<G4double>& phonEnergies,
                                        std::vector<G4double>& qpEnergies) const {
  G4double EDep = 0.;
  if (G4UniformRand() > CalcQPEfficiency(qpE)) return 0.;

  if (qpE >= lowQPLimit*gapEnergy) qpEnergies.push_back(qpE);
  else if (qpE > gapEnergy) {
    EDep += CalcDirectAbsorption(qpE-gapEnergy, phonEnergies);
    EDep += gapEnergy;
  }
  else EDep += qpE;

  return EDep;
}

G4double HVeVKaplanQP::CalcQPEfficiency(G4double qpE) const {
  return std::max(0., std::min(absorberEff + absorberEffSlope*qpE/gapEnergy, 1.));
}

G4double HVeVKaplanQP::QPEnergyRand(G4double Energy) const {
  const G4double BUFF=1000.;
  G4double xmin = gapEnergy + (Energy-2.*gapEnergy)/BUFF;
  G4double xmax = gapEnergy + (Energy-2.*gapEnergy)*(BUFF-1.)/BUFF;
  G4double ymax = QPEnergyPDF(Energy,xmin);
  G4double xtest=0., ytest=ymax;
  do { ytest = G4UniformRand()*ymax; xtest = G4UniformRand()*(xmax-xmin)+xmin; }
  while (ytest > QPEnergyPDF(Energy,xtest));
  return xtest;
}

G4double HVeVKaplanQP::QPEnergyPDF(G4double E, G4double x) const {
  const G4double gapsq = gapEnergy*gapEnergy;
  const G4double occupy = 1.-ThermalPDF(E)-ThermalPDF(E-x);
  return occupy*(x*(E-x)+gapsq)/sqrt((x*x-gapsq)*((E-x)*(E-x)-gapsq));
}

G4double HVeVKaplanQP::ThermalPDF(G4double E) const {
  const G4double kT = k_Boltzmann*temperature;
  return (temperature>0.? 1./(exp(E/kT)+1.):0.);
}

G4double HVeVKaplanQP::PhononEnergyRand(G4double Energy) const {
  const G4double BUFF=1000.;
  G4double xmin = gapEnergy + gapEnergy/BUFF;
  G4double xmax = Energy;
  G4double ymax = PhononEnergyPDF(Energy,xmin);
  G4double xtest=0., ytest=ymax;
  do { ytest = G4UniformRand()*ymax; xtest = G4UniformRand()*(xmax-xmin)+xmin; }
  while (ytest>PhononEnergyPDF(Energy,xtest));
  return Energy-xtest;
}

G4double HVeVKaplanQP::PhononEnergyPDF(G4double E, G4double x) const {
  const G4double gapsq = gapEnergy*gapEnergy;
  return (E-x)*(E-x)*(x-gapsq/E)/sqrt(x*x-gapsq);
}
