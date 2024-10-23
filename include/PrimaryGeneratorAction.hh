#ifndef PCTPRIMARYGENERATORACTION_HH
#define PCTPRIMARYGENERATORACTION_HH 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4GeneralParticleSource;

namespace kcmh
{
  class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
  {
    public:
      PrimaryGeneratorAction();
      ~PrimaryGeneratorAction() override;

      // method from the base class
      void GeneratePrimaries(G4Event*) override;

      // method to access particle gun
      const G4GeneralParticleSource* GetParticleGun() const { return fParticleGun; }

    private:
      G4GeneralParticleSource* fParticleGun = nullptr; // pointer a to G4 gun class

  };
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
