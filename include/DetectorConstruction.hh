#ifndef DETECTORCONSTRUCTION_HH
#define DETECTORCONSTRUCTION_HH 1

#include "G4VUserDetectorConstruction.hh"
#include "phantomConstruction.hh"

class G4VPhysicalVolume;

namespace kcmh
{
  class DetectorMessenger;

  class DetectorConstruction: public G4VUserDetectorConstruction
  {
    public:
      DetectorConstruction(G4String, G4bool isVis=false);
      ~DetectorConstruction() override;

      G4VPhysicalVolume* Construct() override;
      void ConstructSDandField() override;

      void RotatePhantom(const G4double&);
      G4double GetPHangle() {return phAngle;};
      G4int GetPhIndex() {return ph->GetPhIndex();};
      G4String GetPhName() {return ph->GetPhName();};

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