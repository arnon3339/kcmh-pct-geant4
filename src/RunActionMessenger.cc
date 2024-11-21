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

    fPctDirectory = new G4UIdirectory("/run/pct/");
    fPctDirectory->SetGuidance("Run action control for pct");

    fLynxBeamDirectory = new G4UIdirectory("/run/lynx/beam/");
    fLynxBeamDirectory->SetGuidance("Run action control for beam lynx");

    fLynxDetDirectory = new G4UIdirectory("/run/lynx/det/");
    fLynxDetDirectory->SetGuidance("Run action control for detector lynx");

    fPctBeamDirectory = new G4UIdirectory("/run/pct/beam/");
    fPctBeamDirectory->SetGuidance("Run action control for beam pCT");

    fPctDetDirectory = new G4UIdirectory("/run/pct/det/");
    fPctDetDirectory->SetGuidance("Run action control for detector pct");

    fPctDetPhDirectory = new G4UIdirectory("/run/pct/det/ph/");
    fPctDetPhDirectory->SetGuidance("Run action control for detector phantom of pct");

    fLynxLabelDetLayer
      = new G4UIcmdWithAnInteger("/run/lynx/det/layerID",this);
    fLynxLabelDetLayer->SetGuidance("Input the layerID for lynx detector.");
    fLynxLabelDetLayer->SetParameterName("Layer ID", false);
    fLynxLabelDetLayer->SetDefaultValue(0);
    fLynxLabelDetLayer->AvailableForStates(G4State_PreInit,G4State_Idle);    

    fLynxLabelBeamSigma
      = new G4UIcmdWithADoubleAndUnit("/run/lynx/beam/sigma_r",this);
    fLynxLabelBeamSigma->SetGuidance("Input the beam sigma (mm) for lynx beam.");
    fLynxLabelBeamSigma->SetParameterName("Sigma(mm)", false);
    fLynxLabelBeamSigma->SetDefaultValue(0. *mm);
    fLynxLabelBeamSigma->SetDefaultUnit("mm");
    fLynxLabelBeamSigma->AvailableForStates(G4State_PreInit,G4State_Idle);    

    fLynxLabelBeamSigmaR
      = new G4UIcmdWithADoubleAndUnit("/run/lynx/beam/sigma_a",this);
    fLynxLabelBeamSigmaR->SetGuidance("Input the beam angle sigma (degree) for lynx beam.");
    fLynxLabelBeamSigmaR->SetParameterName("Sigma(deg)", false);
    fLynxLabelBeamSigmaR->SetDefaultValue(0.0001 *deg);
    fLynxLabelBeamSigmaR->SetDefaultUnit("deg");
    fLynxLabelBeamSigmaR->AvailableForStates(G4State_PreInit,G4State_Idle);    

    fLynxLabelBeamSigmaE
      = new G4UIcmdWithADoubleAndUnit("/run/lynx/beam/sigma_e",this);
    fLynxLabelBeamSigmaE->SetGuidance("Input the beam energy sigma (MeV) for lynx beam.");
    fLynxLabelBeamSigmaE->SetParameterName("Sigma(MeV)", false);
    fLynxLabelBeamSigmaE->SetDefaultValue(0.1 *MeV);
    fLynxLabelBeamSigmaE->SetDefaultUnit("MeV");
    fLynxLabelBeamSigmaE->AvailableForStates(G4State_PreInit,G4State_Idle);    

    fPctLabelBeamPosX
      = new G4UIcmdWithADoubleAndUnit("/run/pct/beam/posx",this);
    fPctLabelBeamPosX->SetGuidance("Input the beam x position for pct beam.");
    fPctLabelBeamPosX->SetParameterName("PosX(cm)", false);
    fPctLabelBeamPosX->SetDefaultValue(0. *cm);
    fPctLabelBeamPosX->SetDefaultUnit("cm");
    fPctLabelBeamPosX->AvailableForStates(G4State_PreInit,G4State_Idle);    

    fPctLabelPhAngle
      = new G4UIcmdWithADoubleAndUnit("/run/pct/det/ph/angle",this);
    fPctLabelPhAngle->SetGuidance("Input the phantom rotation for pct detetector.");
    fPctLabelPhAngle->SetParameterName("Angle(deg)", false);
    fPctLabelPhAngle->SetDefaultValue(0. *deg);
    fPctLabelPhAngle->SetDefaultUnit("deg");
    fPctLabelPhAngle->AvailableForStates(G4State_PreInit,G4State_Idle);    

  }

  RunActionMessenger::~RunActionMessenger()
  {
    delete fRunDirectory;

    delete fFileDirectory;
    delete fOutputFileCmd;

    delete fLynxDirectory;
    delete fPctDirectory;

    delete fLynxBeamDirectory;
    delete fLynxDetDirectory;

    delete fPctBeamDirectory;
    delete fPctDetDirectory;

    delete fPctDetPhDirectory;

    delete fLynxLabelBeamSigma;
    delete fLynxLabelBeamSigmaR;
    delete fLynxLabelBeamSigmaE;
    delete fLynxLabelDetLayer;

    delete fPctLabelBeamPosX;

    delete fPctLabelPhAngle;


    delete fCloseOutFile;
  }

  void RunActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
  {
    if (command == fOutputFileCmd)
    {
      fRunAction->OpenOutputFile(newValue);
    }

    if (command == fLynxLabelBeamSigma)
    {
      auto inputValue = fLynxLabelBeamSigma->GetNewDoubleValue(newValue);
      fRunAction->SetLynxLabeledBeamSigma(inputValue);
    }

    if (command == fLynxLabelBeamSigmaR)
    {
      auto inputValue = fLynxLabelBeamSigmaR->GetNewDoubleValue(newValue);
      fRunAction->SetLynxLabeledBeamSigmaR(inputValue);
    }

    if (command == fLynxLabelBeamSigmaE)
    {
      auto inputValue = fLynxLabelBeamSigmaE->GetNewDoubleValue(newValue);
      fRunAction->SetLynxLabeledBeamSigmaE(inputValue);
    }

    if (command == fLynxLabelDetLayer)
    {
      auto inputValue = fLynxLabelDetLayer->GetNewIntValue(newValue);
      fRunAction->SetLynxLabeledDetLayer(inputValue);
    }

    if (command == fPctLabelBeamPosX)
    {
      auto inputValue = fPctLabelBeamPosX->GetNewDoubleValue(newValue);
      fRunAction->SetPctLabeledBeamPosX(inputValue);
    }

    if (command == fPctLabelPhAngle)
    {
      auto inputValue = fPctLabelPhAngle->GetNewDoubleValue(newValue);
      fRunAction->SetPctLabeledPhAngle(inputValue);
    }

    if (command == fCloseOutFile)
    {
      fRunAction->CloseOutFile();
    }
  }
} // namespace kchm
