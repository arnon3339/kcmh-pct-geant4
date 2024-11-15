#include "RunActionMessenger.hh"
#include "RunAction.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcommand.hh"
#include "G4SystemOfUnits.hh"

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

    fFileDirectory = new G4UIdirectory("/run/lynx/");
    fFileDirectory->SetGuidance("Run action control for lynx");

    fInitLynx 
      = new G4UIcmdWithoutParameter("/run/lynx/init",this);
    fInitLynx->SetGuidance("Initialize lynx accumulators");
    fInitLynx->AvailableForStates(G4State_PreInit,G4State_Idle);    

    fResetLynx 
      = new G4UIcmdWithoutParameter("/run/lynx/reset",this);
    fResetLynx->SetGuidance("Reset lynx accumulators");
    fResetLynx->AvailableForStates(G4State_PreInit,G4State_Idle);    

    fBeamDirectory = new G4UIdirectory("/run/lynx/beam");
    fBeamDirectory->SetGuidance("Run action control for beam lynx");

    fLabelBeamSigma
      = new G4UIcmdWithADoubleAndUnit("/run/lynx/beam/sigma_r",this);
    fLabelBeamSigma->SetGuidance("Input the beam sigma (mm) for lynx beam.");
    fLabelBeamSigma->SetParameterName("Sigma(mm)", false);
    fLabelBeamSigma->SetDefaultValue(0. *mm);
    fLabelBeamSigma->SetDefaultUnit("mm");
    fLabelBeamSigma->AvailableForStates(G4State_PreInit,G4State_Idle);    

    fLabelBeamSigmaR
      = new G4UIcmdWithADoubleAndUnit("/run/lynx/beam/sigma_a",this);
    fLabelBeamSigmaR->SetGuidance("Input the beam angle sigma (degree) for lynx beam.");
    fLabelBeamSigmaR->SetParameterName("Sigma(deg)", false);
    fLabelBeamSigmaR->SetDefaultValue(0.0001 *deg);
    fLabelBeamSigmaR->SetDefaultUnit("deg");
    fLabelBeamSigmaR->AvailableForStates(G4State_PreInit,G4State_Idle);    

  }

  RunActionMessenger::~RunActionMessenger()
  {
    delete fRunDirectory;
    delete fFileDirectory;
    delete fOutputFileCmd;
    delete fLynxDirectory;
    delete fInitLynx;
    delete fResetLynx;
    delete fBeamDirectory;
    delete fLabelBeamSigma;
    delete fLabelBeamSigmaR;
  }

  void RunActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
  {
    if (command == fOutputFileCmd)
    {
      fRunAction->OpenOutputFile(newValue);
    }

    if (command == fInitLynx)
    {
      fRunAction->InitLynxAcc();
    }

    if (command == fResetLynx)
    {
      fRunAction->ResetLynxAcc();
    }

    if (command == fLabelBeamSigma)
    {
      auto inputValue = fLabelBeamSigma->GetNewDoubleValue(newValue);
      fRunAction->SetLabeledBeamSigma(inputValue);
    }

    if (command == fLabelBeamSigmaR)
    {
      auto inputValue = fLabelBeamSigmaR->GetNewDoubleValue(newValue);
      fRunAction->SetLabeledBeamSigmaR(inputValue);
    }
  }
} // namespace kchm
