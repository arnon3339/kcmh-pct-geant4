#ifndef RUNACTIONMESSENGER_HH
#define RUNACTIONMESSENGER_HH 1

#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcommand;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;

namespace kcmh
{

class RunAction;

class RunActionMessenger: public G4UImessenger
{
private:
  RunAction* fRunAction;

  G4UIdirectory*      fRunDirectory;

  G4UIdirectory*      fFileDirectory;
  G4UIcmdWithAString* fOutputFileCmd;

  G4UIdirectory*      fLynxDirectory;
  G4UIcmdWithoutParameter* fResetLynx;
  G4UIcmdWithoutParameter* fInitLynx;

  G4UIdirectory*      fBeamDirectory;

  G4UIcmdWithADoubleAndUnit* fLabelBeamSigma;
  G4UIcmdWithADoubleAndUnit* fLabelBeamSigmaR;

public:
  RunActionMessenger(RunAction*);
  ~RunActionMessenger();

  void SetNewValue(G4UIcommand*, G4String) override;
};
} // namespace kcmh


#endif