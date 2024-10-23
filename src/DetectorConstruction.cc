#include "DetectorConstruction.hh"

#include "TrackerSD.hh"
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
#include "G4SDManager.hh"

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
    auto siliconMat = nist->FindOrBuildMaterial("G4_Si");
    auto aluminiumMat = nist->FindOrBuildMaterial("G4_Al");
    auto nonVis = new G4VisAttributes();
    nonVis->SetVisibility(false);

    G4int numAlpideCol = 9;
    G4int numAlpideRow = 12;
    G4int numOfDtcLayer = 42;
    G4int numOfPixelRow = 1024;
    G4int numOfPixelCol = 512;
    G4double alpideSizeX = 3. *cm;
    G4double alpideSizeY = 1.38 *cm;
    G4double alpidePixelSizeX = alpideSizeX/numOfPixelRow;
    G4double alpidePixelSizeY = alpideSizeY/numOfPixelCol;
    G4double alpideSizeZ = 50. *um;
    G4double alpidePixelCircuitSizeZ = 11. *um;
    G4double alpidePixelEpiSizeZ = 25. *um;
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
    auto alpidePixelCircuitInAlpidePos = G4ThreeVector(
      0, 0, (-alpideSizeZ/2) + alpidePixelCircuitSizeZ/2
    );
    auto alpidePixelEpiInAlpidePos = G4ThreeVector(
      0, 0, (-alpideSizeZ/2) + alpidePixelCircuitSizeZ + alpidePixelEpiSizeZ/2
    );

    auto worldSol = new G4Box(
      "worldSol", 1.2*envSizeXY/2, 1.2*envSizeXY/2, 1.2*envSizeZ/2
      );
    auto envSol = new G4Box(
      "envSol", envSizeXY/2, envSizeXY/2, envSizeZ/2
    );
    auto dtcChipSol = new G4Box(
      "dtcChipSol", alpideSizeX/2, alpideSizeY/2, dtcLayerDis/2
    );
    auto dtcChipXSol = new G4Box(
      "dtcChipXSol", detSizeX/2, alpideSizeY/2, dtcLayerDis/2
    );
    auto dtcChipYSol = new G4Box(
      "dtcChipYSol", detSizeX/2, detSizeY/2, dtcLayerDis/2
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
    auto alpidePixelCircuitSol = new G4Box(
      "alpidePixelCircuitSol", alpidePixelSizeX/2, alpidePixelSizeY/2, alpidePixelCircuitSizeZ/2
    );
    auto alpidePixelEpiSol = new G4Box(
      "alpidePixelEpiSol", alpidePixelSizeX/2, alpidePixelSizeY/2, alpidePixelEpiSizeZ/2
    );
    auto alpidePixelSol = new G4Box(
      "alpidePixelSol", alpidePixelSizeX/2, alpidePixelSizeY/2, alpideSizeZ/2
    );
    auto alpidePixelRowSol = new G4Box(
      "alpidePixelRowSol", alpideSizeX/2, alpidePixelSizeY/2, alpideSizeZ/2
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
    auto dtcChipLog = new G4LogicalVolume(
      dtcChipSol, airMat, "dtcChipSol"
    );
    dtcChipLog->SetVisAttributes(nonVis);
    auto dtcChipXLog = new G4LogicalVolume(
      dtcChipXSol, airMat, "dtcChipXLog"
    );
    dtcChipXLog->SetVisAttributes(nonVis);
    auto dtcChipYLog = new G4LogicalVolume(
      dtcChipYSol, airMat, "dtcChipYLog"
    );
    dtcChipYLog->SetVisAttributes(nonVis);
    auto dtcLayerLog = new G4LogicalVolume(
      dtcLayerSol, airMat, "dtcLayerLog"
    );
    dtcLayerLog->SetVisAttributes(nonVis);
    auto alpideLog = new G4LogicalVolume(
      alpdieSol, siliconMat, "alpideLog"
    );
    auto alpideVis = new G4VisAttributes();
    alpideVis->SetColor(1, 0, 0, 1);
    alpideLog->SetVisAttributes(alpideVis);
    auto alpidePixelCircuitLog = new G4LogicalVolume(
      alpidePixelCircuitSol, aluminiumMat, "alideCircuitLog"
    );
    alpidePixelCircuitLog->SetVisAttributes(nonVis);
    auto alpidePixelEpiLog = new G4LogicalVolume(
      alpidePixelEpiSol, siliconMat, "alpidePixelEpiLog"
    );
    alpidePixelEpiLog->SetVisAttributes(nonVis);
    auto absorberLog = new G4LogicalVolume(
      absorberSol, airMat, "absorberLog"
    );
    auto alpidePixelLog = new G4LogicalVolume(
      alpidePixelSol, siliconMat, "alpidePixelLog"
    );
    auto alpidePixelRowLog = new G4LogicalVolume(
      alpidePixelRowSol, siliconMat, "alpidePixelRowLog"
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
      dtcChipLog,
      false,
      0,
      checkOverlaps
    );
    new G4PVPlacement(
      nullptr,
      alpidePixelCircuitInAlpidePos,
      alpidePixelCircuitLog,
      "alpidePixelCircuitPhys",
      alpidePixelLog,
      false,
      0,
      checkOverlaps
    );
    new G4PVPlacement(
      nullptr,
      alpidePixelEpiInAlpidePos,
      alpidePixelEpiLog,
      "alpidePixelEpiPhys",
      alpidePixelLog,
      false,
      0,
      checkOverlaps
    );
    new G4PVPlacement(
      nullptr,
      alpideInDtcPos,
      alpideLog,
      "absorberPixelPhys",
      dtcChipLog,
      false,
      0,
      checkOverlaps
    );

    new G4PVReplica(
      "alpidePixelRowPhys",
      alpidePixelLog,
      alpidePixelRowLog,
      kXAxis,
      numOfPixelRow,
      alpidePixelSizeX
    );
    new G4PVReplica(
      "alpidePixelPhys",
      alpidePixelRowLog,
      alpideLog,
      kYAxis,
      numOfPixelCol,
      alpidePixelSizeY
    );
    new G4PVReplica(
      "dtcXPhys",
      dtcChipLog,
      dtcChipXLog,
      kXAxis,
      numAlpideCol,
      alpideSizeX 
    );
    new G4PVReplica(
      "dtcYPhys",
      dtcChipXLog,
      dtcChipYLog,
      kYAxis,
      numAlpideRow,
      alpideSizeY
    );
    new G4PVReplica(
      "dtcLayerPhys",
      dtcChipYLog,
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
      "dtcChipYPhys",
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

  void DetectorConstruction::ConstructSDandField()
  {
    G4String dtcSDName = "/DTC";
    auto dtcSD = new TrackerSD(dtcSDName, "dtcHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(dtcSD);    
    SetSensitiveDetector("alpidePixelEpiLog", dtcSD, true);
  }

  void DetectorConstruction::RotatePhantom(G4double)
  {}
} // namespace kcmh
