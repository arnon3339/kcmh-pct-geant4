#include "DetectorMessenger.hh"

#include "PctDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcommand.hh"
#include "G4SystemOfUnits.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4VUserDetectorConstruction.hh"

namespace kcmh
{
  DetectorMessenger::DetectorMessenger(G4VUserDetectorConstruction* det)
  :fdet(det), fDirectory(0), fPhDirectory(0), fRotPhDirectory(0),
  fSetPhAngleCmd(0)
  {
    fDirectory = new G4UIdirectory("/det/");
    fDirectory->SetGuidance("Dector geometry control");

    fPhDirectory = new G4UIdirectory("/det/phantom/");
    fPhDirectory->SetGuidance("Dector geometry control phantom");

    fRotPhDirectory = new G4UIdirectory("/det/phantom/rotate/");
    fRotPhDirectory->SetGuidance("Dector geometry phantom rotation");
        
    fSetPhAngleCmd
      = new G4UIcmdWithADoubleAndUnit("/det/phantom/rotate/angle",this);
    fSetPhAngleCmd->SetGuidance("Input the angle (degree) for phantom rotation in TOP axis.");
    fSetPhAngleCmd->SetParameterName("Angle(Degree)", false);
    fSetPhAngleCmd->SetDefaultValue(180 *deg);
    fSetPhAngleCmd->SetDefaultUnit("deg");
    fSetPhAngleCmd->AvailableForStates(G4State_PreInit,G4State_Idle);    
  }

  DetectorMessenger::~DetectorMessenger()
  {
    delete fDirectory;
    delete fPhDirectory;
    delete fRotPhDirectory;
    delete fSetPhAngleCmd;
  }

  void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
  {
    if( command == fSetPhAngleCmd ) {
      auto inputValue = fSetPhAngleCmd->GetNewDoubleValue(newValue);
      dynamic_cast<PctDetectorConstruction*>(fdet)->RotatePhantom(inputValue);
    }
  }
} // namespace kcmh
