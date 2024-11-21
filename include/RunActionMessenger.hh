#ifndef RUNACTIONMESSENGER_HH
#define RUNACTIONMESSENGER_HH 1

#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcommand;
class G4UIcmdWithAnInteger;
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
  G4UIdirectory*      fPctDirectory;

  G4UIdirectory*      fLynxBeamDirectory;
  G4UIdirectory*      fLynxDetDirectory;

  G4UIdirectory*      fPctBeamDirectory;
  G4UIdirectory*      fPctDetDirectory;

  G4UIdirectory*      fPctDetPhDirectory;

  G4UIcmdWithADoubleAndUnit* fLynxLabelBeamSigma;
  G4UIcmdWithADoubleAndUnit* fLynxLabelBeamSigmaR;
  G4UIcmdWithADoubleAndUnit* fLynxLabelBeamSigmaE;
  G4UIcmdWithAnInteger* fLynxLabelDetLayer;

  G4UIcmdWithADoubleAndUnit* fPctLabelBeamPosX;

  G4UIcmdWithADoubleAndUnit* fPctLabelPhAngle;


  G4UIcmdWithoutParameter* fCloseOutFile;

public:
  RunActionMessenger(RunAction*);
  ~RunActionMessenger();

  void SetNewValue(G4UIcommand*, G4String) override;
};
} // namespace kcmh


#endif