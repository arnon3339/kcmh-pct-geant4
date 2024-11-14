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
      LynxDetectorConstruction(G4bool isVis=false);
      ~LynxDetectorConstruction() override;

      G4VPhysicalVolume* Construct() override;
      void ConstructSDandField() override;
      void SetPosZ(const G4int);

    private:
      G4double fZPos;
      G4bool fIsVis;
      DetectorMessenger* detMessenger;
  };
} // namespace kcmh
#endif