#include "PhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmExtraPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4LossTableManager.hh"
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4SystemOfUnits.hh"
#include "PctDetectorConstruction.hh"
#include "G4RegionStore.hh"

namespace kcmh
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList()
{
  // EM Physics
  RegisterPhysics( new G4EmStandardPhysics_option3());

  // Synchroton Radiation & GN Physics
  RegisterPhysics( new G4EmExtraPhysics() );

  // Decays
  RegisterPhysics( new G4DecayPhysics() );

   // Hadron Elastic scattering
  RegisterPhysics( new G4HadronElasticPhysics() );

  //  Hadron Physics
  RegisterPhysics( new G4HadronPhysicsQGSP_BIC() );

  // Stopping Physics
  RegisterPhysics( new G4StoppingPhysics() );

  // Ion Physics
  RegisterPhysics( new G4IonBinaryCascadePhysics());
  
  // Neutron tracking cut
  RegisterPhysics( new G4NeutronTrackingCut());

  // G4VUserPhysicsList::SetCutsForRegion(1.* mm, "myPhantom");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCutsWithDefault();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
