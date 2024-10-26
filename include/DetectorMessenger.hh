#ifndef DETECTORMESSENGER_HH
#define DETECTORMESSENGER_HH 1

#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcommand;

namespace kcmh
{
  class DetectorConstruction;
  class DetectorMessenger: public G4UImessenger
    {
    private:
      DetectorConstruction* fdet;
      G4UIdirectory*              fDirectory;
      G4UIdirectory*              fPhDirectory;
      G4UIdirectory*              fRotPhDirectory;
      G4UIcmdWithADoubleAndUnit*  fSetPhAngleCmd;

    public:
      DetectorMessenger(DetectorConstruction*);
      ~DetectorMessenger();

      void SetNewValue(G4UIcommand*, G4String) override;
    };

  
} // namespace kcmh

#endif