#include "DetectorMessager.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcommand.hh"
#include "G4SystemOfUnits.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"

namespace kcmh
{
  DetectorMessager::DetectorMessager(DetectorConstruction* det)
  :fdet(det), fDirectory(0), fPhDirectory(0), fRotPhDirectory(0),
  fSetPhAngleCmd(0), fSetPhInstallCmd(0)
  {
    fDirectory = new G4UIdirectory("/det/");
    fDirectory->SetGuidance("Dector geometry control");

    fPhDirectory = new G4UIdirectory("/det/phantom");
    fPhDirectory->SetGuidance("Dector geometry control phantom");

    fRotPhDirectory = new G4UIdirectory("/det/phantom/rotate/");
    fRotPhDirectory->SetGuidance("Dector geometry phantom rotation");
        
    fSetPhAngleCmd
      = new G4UIcmdWithADoubleAndUnit("/det/phantom/rotate/angle",this);
    fSetPhAngleCmd->SetGuidance("Input the angle (degree) for phantom rotation in TOP axis.");
    fSetPhAngleCmd->SetParameterName("Angle(Degree)", false);
    fSetPhAngleCmd->SetDefaultValue(0.0);
    fSetPhAngleCmd->SetDefaultUnit("deg");
    fSetPhAngleCmd->AvailableForStates(G4State_PreInit,G4State_Idle);    

    fSetPhInstallCmd 
      = new G4UIcmdWithABool("/det/phantom/install", this);
    fSetPhInstallCmd->SetGuidance("Input the boolean for phantom installation.");
    fSetPhInstallCmd->SetParameterName("Install(Boolean)", false);
    fSetPhInstallCmd->SetDefaultValue(true);
    fSetPhInstallCmd->AvailableForStates(G4State_PreInit,G4State_Idle);    
  }

  DetectorMessager::~DetectorMessager()
  {
    delete fDirectory;
    delete fPhDirectory;
    delete fRotPhDirectory;
    delete fSetPhAngleCmd;
    delete fSetPhInstallCmd;
  }

  void DetectorMessager::SetNewValue(G4UIcommand* command, G4String newValue)
  {
    if( command == fSetPhAngleCmd ) {
      auto inputValue = fSetPhAngleCmd->GetNewDoubleValue(newValue);
      fdet->RotatePhantom(inputValue);
    }
  }
} // namespace kcmh
