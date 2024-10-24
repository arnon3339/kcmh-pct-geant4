#ifndef PHYSICSLIST_HH
#define PHYSICSLIST_HH 1

#include "G4VModularPhysicsList.hh"


namespace kcmh
{
/// Modular physics list
///
/// It includes the folowing physics builders
/// - G4DecayPhysics
/// - G4RadioactiveDecayPhysics
/// - G4EmStandardPhysics

class PhysicsList: public G4VModularPhysicsList
{
public:
  PhysicsList();
  ~PhysicsList() override = default;

  void SetCuts() override;

};

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

