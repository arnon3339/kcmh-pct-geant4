#ifndef LYNXDETECTORCONSTRUCTION_HH
#define LYNXDETECTORCONSTRUCTION_HH 1

#include "G4VUserDetectorConstruction.hh"
#include "phantomConstruction.hh"

class G4VPhysicalVolume;

namespace kcmh
{
  class DetectorMessenger;

  class LynxDetectorConstruction: public G4VUserDetectorConstruction
  {
    public:
      LynxDetectorConstruction(G4String, G4bool isVis=false);
      ~LynxDetectorConstruction() override;

      G4VPhysicalVolume* Construct() override;
      void ConstructSDandField() override;

    private:
      G4bool fIsVis;
      DetectorMessenger* detMessenger;
      G4LogicalVolume* phanLog;
      G4double phAngle;
      G4LogicalVolume* envLog;
      G4VPhysicalVolume* phPhys;
      PhantomConstruction* ph;
  };
} // namespace kcmh
#endif