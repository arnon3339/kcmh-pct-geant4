#include "LynxDetectorConstruction.hh"

#include "LynxTrackerSD.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4NistManager.hh"
#include "G4PVReplica.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "phantomConstruction.hh"
#include "DetectorMessenger.hh"
#include "G4SDManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4RunManager.hh"

namespace kcmh
{
  LynxDetectorConstruction::LynxDetectorConstruction(G4bool isVis)
  :fIsVis(isVis), detMessenger(0)
  {
    
  }

  LynxDetectorConstruction::~LynxDetectorConstruction()
  {
    delete detMessenger;
  }

  G4VPhysicalVolume* LynxDetectorConstruction::Construct()
  {
    G4bool checkOverlaps = true;
    auto nist = G4NistManager::Instance();
    auto airMat = nist->FindOrBuildMaterial("G4_AIR");
    auto nonVis = new G4VisAttributes();
    nonVis->SetVisibility(false);

    // Define elements
    G4Element* H = nist->FindOrBuildElement("H");
    G4Element* C = nist->FindOrBuildElement("C");
    G4Element* O = nist->FindOrBuildElement("O");
    G4Element* Gd = nist->FindOrBuildElement("Gd");
    
    // Define polystyrene (C8H8)n as the plastic base
    G4Material* polystyrene = new G4Material("Polystyrene", 1.06*g/cm3, 2);
    polystyrene->AddElement(C, 8);
    polystyrene->AddElement(H, 8);

    // Define gadolinium oxide (Gd2O3)
    G4Material* gadoliniumOxide = new G4Material("Gd2O3", 7.41*g/cm3, 2);
    gadoliniumOxide->AddElement(Gd, 2);
    gadoliniumOxide->AddElement(O, 3);

    // Define the gadolinium-based plastic scintillator material
    // Here we assume 5% gadolinium oxide by weight in the plastic
    G4double polystyreneFraction = 0.95;
    G4double gadoliniumOxideFraction = 0.05;

    G4Material* GdPlasticScintillator = new G4Material("GdPlasticScintillator", 1.06*g/cm3, 2);
    GdPlasticScintillator->AddMaterial(polystyrene, polystyreneFraction);
    GdPlasticScintillator->AddMaterial(gadoliniumOxide, gadoliniumOxideFraction);

    // Geometry
    G4int numOfPixels = 600;
    G4double envSizeXY = 30. *cm;
    G4double envSizeZ = 4. *m;
    G4double detSizeXY = 300. *mm; 
    G4double detSizeZ = 0.4 *mm;
    G4double pixelSizeXY = detSizeXY/numOfPixels;

    auto worldSol = new G4Box(
      "worldSol", 1.2*envSizeXY/2, 1.2*envSizeXY/2, 1.2*envSizeZ/2
      );
    auto envSol = new G4Box(
      "envSol", envSizeXY/2, envSizeXY/2, envSizeZ/2
    );
    auto pixelSol = new G4Box("pixel", pixelSizeXY/2, pixelSizeXY/2,
      detSizeZ/2);
    auto trackerSol = new G4Box("tracker", detSizeXY/2, detSizeXY/2, detSizeZ/2);
    auto trackerRowSol = new G4Box("trackerRowSol", detSizeXY/2, pixelSizeXY/2,
      detSizeZ/2);

    auto pixelLog = new G4LogicalVolume(pixelSol, GdPlasticScintillator, "pixelLog");
    auto worldLog = new G4LogicalVolume(
      worldSol, airMat, "worldLog"
    );
    worldLog->SetVisAttributes(nonVis);
    auto envLog = new G4LogicalVolume(
      envSol, airMat, "envLog"
    );
    auto envVis = new G4VisAttributes();
    envVis->SetForceWireframe(true);
    envLog->SetVisAttributes(envVis);

    auto trackerRowLog = new G4LogicalVolume(
      trackerRowSol, airMat, "trackerRowLog"
    );
    auto trackerLog = new G4LogicalVolume(
      trackerSol, airMat, "trackerLog"
    );

    new G4PVReplica(
      "trackerRowPhys",
      pixelLog,
      trackerRowLog,
      kXAxis,
      numOfPixels,
      pixelSizeXY
    );
    new G4PVReplica(
      "trackerRowPhys",
      trackerRowLog,
      trackerLog,
      kYAxis,
      numOfPixels,
      pixelSizeXY
    );

    trackerPhys = new G4PVPlacement(
      nullptr,
      G4ThreeVector(),
      trackerLog,
      "trackerPhys",
      envLog,
      false,
      0,
      checkOverlaps
    );

    new G4PVPlacement(
      nullptr,
      G4ThreeVector(),
      envLog,
      "envPhys",
      worldLog,
      false,
      0,
      checkOverlaps
    );

    return new G4PVPlacement(
      nullptr,
      G4ThreeVector(),
      worldLog,
      "worldPhys",
      nullptr,
      false,
      0,
      checkOverlaps
    );
  }

  void LynxDetectorConstruction::ConstructSDandField()
  {
    G4String lynxSDName = "/Lynx";
    auto lynxSD = new LynxTrackerSD(lynxSDName, "LynxHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(lynxSD);    
    SetSensitiveDetector("pixelLog", lynxSD, true);
  }

  void LynxDetectorConstruction::SetPosZ(const G4double posZ)
  {
    trackerPhys->SetTranslation(G4ThreeVector(0, 0, posZ));
    auto runManager = G4RunManager::GetRunManager();
    runManager->GeometryHasBeenModified();
  }
} // namespace kcmh
