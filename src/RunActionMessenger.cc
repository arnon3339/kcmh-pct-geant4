#include "RunActionMessenger.hh"
#include "RunAction.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
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

    fCloseOutFile
      = new G4UIcmdWithoutParameter("/run/file/close",this);
    fCloseOutFile->SetGuidance("Close output file");
    fCloseOutFile->AvailableForStates(G4State_PreInit,G4State_Idle);    

    fLynxDirectory = new G4UIdirectory("/run/lynx/");
    fLynxDirectory->SetGuidance("Run action control for lynx");

    fBeamDirectory = new G4UIdirectory("/run/lynx/beam/");
    fBeamDirectory->SetGuidance("Run action control for beam lynx");

    fDetDirectory = new G4UIdirectory("/run/lynx/det/");
    fDetDirectory->SetGuidance("Run action control for detector lynx");

    fLabelDetLayer
      = new G4UIcmdWithAnInteger("/run/lynx/det/layerID",this);
    fLabelDetLayer->SetGuidance("Input the layerID for lynx detector.");
    fLabelDetLayer->SetParameterName("Layer ID", false);
    fLabelDetLayer->SetDefaultValue(0);
    fLabelDetLayer->AvailableForStates(G4State_PreInit,G4State_Idle);    

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

    fLabelBeamSigmaE
      = new G4UIcmdWithADoubleAndUnit("/run/lynx/beam/sigma_e",this);
    fLabelBeamSigmaE->SetGuidance("Input the beam energy sigma (MeV) for lynx beam.");
    fLabelBeamSigmaE->SetParameterName("Sigma(MeV)", false);
    fLabelBeamSigmaE->SetDefaultValue(0.1 *MeV);
    fLabelBeamSigmaE->SetDefaultUnit("MeV");
    fLabelBeamSigmaE->AvailableForStates(G4State_PreInit,G4State_Idle);    

  }

  RunActionMessenger::~RunActionMessenger()
  {
    delete fRunDirectory;
    delete fFileDirectory;
    delete fOutputFileCmd;
    delete fBeamDirectory;
    delete fLabelBeamSigma;
    delete fLabelBeamSigmaR;
    delete fLabelBeamSigmaE;
    delete fDetDirectory;
    delete fLynxDirectory;
    delete fLabelDetLayer;
    delete fCloseOutFile;
  }

  void RunActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
  {
    if (command == fOutputFileCmd)
    {
      fRunAction->OpenOutputFile(newValue);
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

    if (command == fLabelBeamSigmaE)
    {
      auto inputValue = fLabelBeamSigmaE->GetNewDoubleValue(newValue);
      fRunAction->SetLabeledBeamSigmaE(inputValue);
    }

    if (command == fLabelDetLayer)
    {
      auto inputValue = fLabelDetLayer->GetNewIntValue(newValue);
      fRunAction->SetLabeledDetLayer(inputValue);
    }

    if (command == fCloseOutFile)
    {
      fRunAction->CloseOutFile();
    }
  }
} // namespace kchm
