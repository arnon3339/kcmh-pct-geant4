#ifndef DETECTORMESSAGER_HH
#define DETECTORMESSAGER_HH 1

#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcommand;

namespace kcmh
{
  class DetectorConstruction;
  class DetectorMessager: public G4UImessenger
    {
    private:
      DetectorConstruction* fdet;
      G4UIdirectory*              fDirectory;
      G4UIdirectory*              fPhDirectory;
      G4UIdirectory*              fRotPhDirectory;
      G4UIcmdWithADoubleAndUnit*  fSetPhAngleCmd;

    public:
      DetectorMessager(DetectorConstruction*);
      ~DetectorMessager();

      virtual void SetNewValue(G4UIcommand*, G4String);
    };

  
} // namespace kcmh

#endif