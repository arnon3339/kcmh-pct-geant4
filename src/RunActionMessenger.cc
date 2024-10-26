#include "RunActionMessenger.hh"
#include "RunAction.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcommand.hh"

namespace kcmh
{
  RunActionMessenger::RunActionMessenger(RunAction* runAction)
  :fRunAction(runAction)
  {
    fRunDirectory= new G4UIdirectory("/run/");
    fRunDirectory->SetGuidance("Run action control");

    fFileDirectory = new G4UIdirectory("/run/file/");
    fFileDirectory->SetGuidance("Run action control file");

    fOutputFileCmd
      = new G4UIcmdWithAString("/run/file/output",this);
    fOutputFileCmd->SetGuidance("Input a file name to store outputs");
    fOutputFileCmd->SetParameterName("output file", false);
    fOutputFileCmd->AvailableForStates(G4State_PreInit,G4State_Idle);    
  }

  RunActionMessenger::~RunActionMessenger()
  {
    delete fRunDirectory;
    delete fFileDirectory;
    delete fOutputFileCmd;
  }

  void RunActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
  {
    if (command == fOutputFileCmd)
    {
      fRunAction->OpenOutputFile(newValue);

    }
  }
} // namespace kchm
