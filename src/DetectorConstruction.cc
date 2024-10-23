#include "DetectorConstruction.hh"

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
#include "DetectorMessager.hh"

namespace kcmh
{
  DetectorConstruction::DetectorConstruction(G4String phName)
  :detMessager(0), phanLog(0), phAngle(0), worldLog(0), phPhys(0), ph(0)
  {
    ph = new PhantomConstruction(phName);
    phanLog = ph->GetLogVolume();
    detMessager = new DetectorMessager(this);
  }

  DetectorConstruction::~DetectorConstruction()
  {}

  G4VPhysicalVolume* DetectorConstruction::Construct()
  {
    G4bool checkOverlaps = true;
    auto nist = G4NistManager::Instance();
    auto airMat = nist->FindOrBuildMaterial("G4_AIR");
    auto nonVis = new G4VisAttributes();
    nonVis->SetVisibility(false);

    G4int numAlpideCol = 9;
    G4int numAlpideRow = 12;
    G4int numOfDtcLayer = 42;
    G4double alpideSizeX = 3. *cm;
    G4double alpideSizeY = 1.38 *cm;
    G4double alpideSizeZ = 50. *um;
    G4double absorberSizeZ = 3.5 *mm;
    G4double absorberSizeX = alpideSizeX;
    G4double absorberSizeY = alpideSizeY;
    G4double absorberGapSizeZ = 4. *mm;
    G4double dtcLayerDis = 2.5 *cm;
    G4double detSizeX = alpideSizeX*numAlpideCol;
    G4double detSizeY = alpideSizeY*numAlpideRow;
    G4double dtcSizeZ = dtcLayerDis*numOfDtcLayer;
    G4double envSizeXY = 30. *cm;
    G4double envSizeZ = 2.8 *m;
    G4double phSizeXY = 30. *cm;
    G4double phSizeZ = 30. *cm;
    G4double phToTrackerSizeZ = 2.5 *cm;

    auto trackerPos = G4ThreeVector(0, 0,
      phSizeZ/2 + phToTrackerSizeZ + alpideSizeZ/2
    );
    auto dtcPos = G4ThreeVector(0, 0, 
      phSizeZ/2 + phToTrackerSizeZ + alpideSizeZ/2 + dtcLayerDis + dtcSizeZ/2);
    auto absorberInDtcPos = G4ThreeVector(
      0, 0, (-dtcLayerDis/2) + alpideSizeZ + absorberGapSizeZ + absorberSizeZ/2
    );
    auto alpideInDtcPos = G4ThreeVector(
      0, 0, (-dtcLayerDis/2) + alpideSizeZ/2
    );

    auto worldSol = new G4Box(
      "worldSol", 1.2*envSizeXY/2, 1.2*envSizeXY/2, 1.2*envSizeZ/2
      );
    auto envSol = new G4Box(
      "envSol", envSizeXY/2, envSizeXY/2, envSizeZ/2
    );
    auto dtcPixelSol = new G4Box(
      "dtcPixelSol", alpideSizeX/2, alpideSizeY/2, dtcLayerDis/2
    );
    auto dtcPixelXSol = new G4Box(
      "dtcPixelXSol", detSizeX/2, alpideSizeY/2, dtcLayerDis/2
    );
    auto dtcPixelYSol = new G4Box(
      "dtcPixelYSol", detSizeX/2, detSizeY/2, dtcLayerDis/2
    );
    auto dtcLayerSol = new G4Box(
      "dtcLayerSol", detSizeX/2, detSizeY/2, dtcSizeZ/2
    );
    auto alpdieSol = new G4Box(
      "alpideSol", alpideSizeX/2, alpideSizeY/2, alpideSizeZ/2
    );
    auto absorberSol = new G4Box(
      "absorberSol", absorberSizeX/2, absorberSizeY/2, absorberSizeZ/2
    );
    auto trackerSol = new G4Box(
      "trakcerSol", detSizeX/2, detSizeY/2, alpideSizeZ/2
    );
    auto trackerFlexSol = new G4Box(
      "trackerFlexSol", detSizeX/2, alpideSizeY/2, alpideSizeZ/2
    );

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
    auto dtcPixelLog = new G4LogicalVolume(
      dtcPixelSol, airMat, "dtcPixelSol"
    );
    dtcPixelLog->SetVisAttributes(nonVis);
    auto dtcPixelXLog = new G4LogicalVolume(
      dtcPixelXSol, airMat, "dtcPixelXLog"
    );
    dtcPixelXLog->SetVisAttributes(nonVis);
    auto dtcPixelYLog = new G4LogicalVolume(
      dtcPixelYSol, airMat, "dtcPixelYLog"
    );
    dtcPixelYLog->SetVisAttributes(nonVis);
    auto dtcLayerLog = new G4LogicalVolume(
      dtcLayerSol, airMat, "dtcLayerLog"
    );
    dtcLayerLog->SetVisAttributes(nonVis);
    auto alpideLog = new G4LogicalVolume(
      alpdieSol, airMat, "alpideLog"
    );
    auto alpideVis = new G4VisAttributes();
    alpideVis->SetColor(1, 0, 0, 1);
    alpideLog->SetVisAttributes(alpideVis);
    auto absorberLog = new G4LogicalVolume(
      absorberSol, airMat, "absorberLog"
    );
    auto trackerLog = new G4LogicalVolume(
      trackerSol, airMat, "trackerLog"
    );
    trackerLog->SetVisAttributes(nonVis);
    auto trackerFlexLog = new G4LogicalVolume(
      trackerFlexSol, airMat, "trackerFlexLog"
    );
    trackerFlexLog->SetVisAttributes(nonVis);

    phAngle = 90 *deg;
    auto rMatrix = new G4RotationMatrix();;
    rMatrix->rotateY(phAngle);
    phPhys = new G4PVPlacement(
      rMatrix,
      G4ThreeVector(0, 0, 0),
      phanLog,
      "phantomPhys",
      envLog,
      false,
      0,
      checkOverlaps
    );

    new G4PVPlacement(
      nullptr,
      absorberInDtcPos,
      absorberLog,
      "absorberPixelPhys",
      dtcPixelLog,
      false,
      0,
      checkOverlaps
    );
    new G4PVPlacement(
      nullptr,
      alpideInDtcPos,
      alpideLog,
      "absorberPixelPhys",
      dtcPixelLog,
      false,
      0,
      checkOverlaps
    );
    new G4PVReplica(
      "dtcXPhys",
      dtcPixelLog,
      dtcPixelXLog,
      kXAxis,
      numAlpideCol,
      alpideSizeX 
    );
    new G4PVReplica(
      "dtcYPhys",
      dtcPixelXLog,
      dtcPixelYLog,
      kYAxis,
      numAlpideRow,
      alpideSizeY
    );
    new G4PVReplica(
      "dtcLayerPhys",
      dtcPixelYLog,
      dtcLayerLog,
      kZAxis,
      numOfDtcLayer,
      dtcLayerDis
    );
    new G4PVReplica(
      "trackerFlexPhys",
      alpideLog,
      trackerFlexLog,
      kXAxis,
      numAlpideCol,
      alpideSizeX
    );
    new G4PVReplica(
      "trackerPhys",
      trackerFlexLog,
      trackerLog,
      kYAxis,
      numAlpideRow,
      alpideSizeY
    );

    new G4PVPlacement(
      nullptr,
      trackerPos,
      trackerLog,
      "trackerPhys",
      envLog,
      false,
      0,
      checkOverlaps
    );
    new G4PVPlacement(
      nullptr,
      dtcPos,
      dtcLayerLog,
      "dtcPixelYPhys",
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

  void DetectorConstruction::ConstructSDandField(){}
  void DetectorConstruction::RotatePhantom(G4double)
  {}
} // namespace kcmh
