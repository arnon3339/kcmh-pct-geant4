#ifndef DETECTORMESSENGER_HH
#define DETECTORMESSENGER_HH 1

#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcommand;
class G4VUserDetectorConstruction;

namespace kcmh
{
  class PctDetectorConstruction;
  class DetectorMessenger: public G4UImessenger
    {
    private:
      G4VUserDetectorConstruction* fdet;
      G4UIdirectory*              fDirectory;
      G4UIdirectory*              fPhDirectory;
      G4UIdirectory*              fRotPhDirectory;
      G4UIcmdWithADoubleAndUnit*  fSetPhAngleCmd;

    public:
      DetectorMessenger(G4VUserDetectorConstruction*);
      ~DetectorMessenger();

      void SetNewValue(G4UIcommand*, G4String) override;
    };

  
} // namespace kcmh

#endif