#include "phantomConstruction.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Element.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4RotationMatrix.hh"
#include "G4Tubs.hh"

#include "CADMesh.hh"

#include <map>
#include <string>

namespace kcmh
{
  PhantomConstruction::~PhantomConstruction(){
    delete phLV;
  }

  PhantomConstruction::PhantomConstruction(G4String phName):
  fPhIndex(0), fPhName(phName){
    G4String phPath(PHANTOM_PATH);
    genMat(phanMat);

    G4bool checkOverlaps = true;

    auto nonVis = new G4VisAttributes();
    nonVis->SetVisibility(false);
    G4VisAttributes *C0vis = new G4VisAttributes();
    C0vis->SetVisibility(true);
    C0vis->SetColor(G4Color(0, 1.0, 0, 0.75));

    G4VisAttributes *C1vis = new G4VisAttributes();
    C1vis->SetVisibility(true);
    C1vis->SetColor(G4Color(1.0, 0, 0, 0.75));

    if (!phName.compare(refPhantoms.at(0))){
      fPhIndex = 0;
      auto mesh = CADMesh::TessellatedMesh::FromOBJ(phPath + "/ctp404.obj");
      mesh->SetScale(1000);
      G4ThreeVector pos1_5 = G4ThreeVector(0, 0, 0);
      G4LogicalVolume *C0log = new G4LogicalVolume(
        mesh->GetSolid("C0"),
        phanMat["Water"],
        "C0log"
      );
      C0log->SetVisAttributes(C0vis);

      G4LogicalVolume *C1log = new G4LogicalVolume(
        mesh->GetSolid("C1"),
        phanMat["AcrylicLow"],
        "C1log"
      );
      C1log->SetVisAttributes(C1vis);
      new G4PVPlacement(nullptr,  // no rotation
        G4ThreeVector(0, 0, 0),                     // at position
        C1log,              // its logical volume
        "C1phys",                 // its name
        C0log,                 // its mother  volume
        false,                    // no boolean operation
        0,                        // copy number
        checkOverlaps);           // overlaps checking

      std::map<std::string, G4Material*> objMat = {
        {"C0", phanMat["Water"]},
        {"C1", phanMat["AcrylicLow"]},
        {"sen_acrylic", phanMat["Acrylic"]},
        {"sen_air", phanMat["Air"]},
        {"sen_delrin", phanMat["Delrin"]},
        {"sen_ldpe", phanMat["LDPE"]},
        {"sen_pmp", phanMat["PMP"]},
        {"sen_polystyrene", phanMat["Polystyrene"]},
        {"sen_teflon", phanMat["Teflon"]},
        {"sen_water", phanMat["Water"]},
        {"pix13", phanMat["Air"]},
        {"pix4", phanMat["Teflon"]},
        {"cen1", phanMat["Acrylic"]},
        {"cen2", phanMat["Acrylic"]},
        {"cen3", phanMat["Acrylic"]},
        {"cen4", phanMat["Acrylic"]},
        {"cen5", phanMat["Acrylic"]},
        {"ramp", phanMat["Ramp"]}
      };

      G4String logName = "";
      G4String physName = "";
      for (size_t i = 0; i < mesh->GetSolids().size(); i++)
      {
        if (!mesh->GetSolid(i)->GetName().compare("C0") || !mesh->GetSolid(i)->GetName().compare("C1"))
          continue;
        logName = mesh->GetSolid(i)->GetName()  + std::string("Log");
        auto logicShape1_5 = new G4LogicalVolume(mesh->GetSolid(i),  // its solid
          objMat[mesh->GetSolid(i)->GetName()],                                        // its material
          logName);                                         // its name

        physName = mesh->GetSolid(i)->GetName() + std::string("Phys");

        new G4PVPlacement(nullptr,  // no rotation
          G4ThreeVector(0, 0, 0),                     // at position
          logicShape1_5,              // its logical volume
          physName,                 // its name
          C1log,                 // its mother  volume
          false,                    // no boolean operation
          0,                        // copy number
          checkOverlaps);           // overlaps checking
      }
      phLV = C0log;
    }
    else if (!phName.compare(refPhantoms.at(1))){
      fPhIndex = 1;
      G4double = 5. *cm;
      auto kcmhSol = new G4Tubs(
        "kcmhSol",  // Name
        0,          // Inner radius (can be 0 for solid cylinder)
        5. *cm,          // Outer radius
        height/2,             // Half-height along Z-axis
        0*deg,                // Starting angle (0 degrees)
        90*deg                // Angular span (90 degrees for one-fourth cylinder)
      );

      auto kcmhLV = new G4LogicalVolume(
        kcmhSol, phanMat["Acrylic"], 'kcmhLog'
      );

      phLV = kcmhLV;
    }
    else if (!phName.compare(refPhantoms.at(2))){
      fPhIndex = 2;
      // heart phantom
      auto brainMesh = CADMesh::TessellatedMesh::FromOBJ(phPath + "brain.obj");
      brainMesh->SetScale(100);
      auto brainLV = new G4LogicalVolume(brainMesh->GetSolid(0), phanMat["brain"], "brainhLV");
      phLV = brainLV;
    }
    else if (!phName.compare(refPhantoms.at(3))){
      fPhIndex = 3;
      // heart phantom
      auto heartMesh = CADMesh::TessellatedMesh::FromOBJ(phPath + "heart.obj");
      heartMesh->SetScale(100);
      auto heartLV = new G4LogicalVolume(heartMesh->GetSolid(0), phanMat["heartwall"], "heartLV");
      phLV = heartLV;
    }
    else if (!phName.compare(refPhantoms.at(4))){
      fPhIndex = 4;
      // lung phantom
      auto lungMesh = CADMesh::TessellatedMesh::FromOBJ(phPath + "lung.obj");
      lungMesh->SetScale(100);
      auto lungLV = new G4LogicalVolume(lungMesh->GetSolid(0), phanMat["lung"], "lungLV");
      phLV = lungLV;
    }
    else{
      fPhIndex = 5;
      // small phantom
      auto smPhWrapS = new G4Box("smPhWrapS", 6 *cm, 6 *cm, 6 *cm);
      auto smPhWrapLV = new G4LogicalVolume(smPhWrapS, phanMat["Air"], "smPhWrapLV");
      smPhWrapLV->SetVisAttributes(nonVis);
      auto smPhS = new G4Tubs("smPhS", 0, 5.0 *cm, 2.5 *cm, 0 *deg, 360 *deg);
      auto smPhLV = new G4LogicalVolume(smPhS, phanMat["Water"], "smPhLV");
      smPhLV->SetVisAttributes(C0vis);
      auto smPhInnerS = new G4Box("smPhInnerS", 4 *cm, 1 *cm, 1 *cm);
      auto smPhInnerLV = new G4LogicalVolume(smPhInnerS, phanMat["Teflon"], "smPHInnerLV");
      smPhInnerLV->SetVisAttributes(C1vis);

      auto rMatrix = new G4RotationMatrix();
      rMatrix->rotateX(90 *deg);

      new G4PVPlacement(
        rMatrix, 
        G4ThreeVector(0, 0, 0),
        smPhLV, 
        "smPhWrapPV",
        smPhWrapLV,
        false,
        0,
        checkOverlaps
      );

      new G4PVPlacement(
        nullptr,
        G4ThreeVector(0, 0, 0),
        smPhInnerLV,
        "smPhPV",
        smPhLV,
        false,
        0,
        checkOverlaps 
      );
    phLV = smPhWrapLV;
    }
  }

  void PhantomConstruction::genMat(std::map<std::string, G4Material*> &mat)
  {
    auto nist = G4NistManager::Instance();
    mat.insert({std::string("Air"), nist->FindOrBuildMaterial("G4_AIR")});
    mat.insert({std::string("Water"), nist->FindOrBuildMaterial("G4_WATER")});
    
    G4Material *m_acrylic_low = new G4Material("myAcrylicLow", 1.14*g/cm3, 3);
    G4Material *m_acrylic = new G4Material("myAcrylic", 1.18*g/cm3, 3);
    G4Material* m_acrylic = new G4Material("myAcrylic", 1.18 * g / cm3, 3);

    m_acrylic->AddElement(G4Element::GetElement("C"), 0.621);  // 62.1% Carbon by mass
    m_acrylic->AddElement(G4Element::GetElement("H"), 0.050);  // 5.0% Hydrogen by mass
    m_acrylic->AddElement(G4Element::GetElement("O"), 0.329);  // 32.9% Oxygen by mass

    m_acrylic_low->AddElement(G4Element::GetElement("C"), 0.621);  // 62.1% Carbon by mass
    m_acrylic_low->AddElement(G4Element::GetElement("H"), 0.050);  // 5.0% Hydrogen by mass
    m_acrylic_low->AddElement(G4Element::GetElement("O"), 0.329);  // 32.9% Oxygen by mass
    mat.insert({std::string("AcrylicLow"), m_acrylic_low});
    mat.insert({std::string("Acrylic"), m_acrylic});

    G4Material *m_pmp = new G4Material("myPMP", 0.853*g/cm3, 2);
    m_pmp->AddElement(G4Element::GetElement("C"), 0.625);
    m_pmp->AddElement(G4Element::GetElement("H"), 0.375);
    mat.insert({std::string("PMP"), m_pmp});

    G4Material *m_ldpe = new G4Material("myLDPE", 0.945*g/cm3, 2);
    m_ldpe->AddElement(G4Element::GetElement("C"), 0.858);
    m_ldpe->AddElement(G4Element::GetElement("H"), 0.142);
    mat.insert({std::string("LDPE"), m_ldpe});

    G4Material *m_polystr = new G4Material("myPolystr", 0.998*g/cm3, 2);
    m_polystr->AddElement(G4Element::GetElement("C"), 0.927);
    m_polystr->AddElement(G4Element::GetElement("H"), 0.073);
    mat.insert({std::string("Polystyrene"), m_polystr});

    G4Material *m_delrin = new G4Material("myDelrin", 1.363*g/cm3, 3);
    m_delrin->AddElement(G4Element::GetElement("C"), 0.613);
    m_delrin->AddElement(G4Element::GetElement("H"), 0.125);
    m_delrin->AddElement(G4Element::GetElement("O"), 0.262);
    mat.insert({std::string("Delrin"), m_delrin});
    
    G4Material *m_teflon = new G4Material("myTeflon", 1.868*g/cm3, 2);
    m_teflon->AddElement(nist->GetElement("C"), 0.241);
    m_teflon->AddElement(nist->GetElement("F"), 0.759);
    mat.insert({std::string("Teflon"), m_teflon});

    G4Material *m_ramp = new G4Material("myRamp", 1.252*g/cm3, 11);
    m_ramp->AddElementByMassFraction(nist->GetElement("H"), 7.9/100);
    m_ramp->AddElementByMassFraction(nist->GetElement("C"), 34.5/100);
    m_ramp->AddElementByMassFraction(nist->GetElement("N"), 2.9/100);
    m_ramp->AddElementByMassFraction(nist->GetElement("O"), 41.3/100);
    m_ramp->AddElementByMassFraction(nist->GetElement("Na"), 0.2/100);
    m_ramp->AddElementByMassFraction(nist->GetElement("Mg"), 0.1/100);
    m_ramp->AddElementByMassFraction(nist->GetElement("F"), 3.9/100);
    m_ramp->AddElementByMassFraction(nist->GetElement("S"), 0.2/100);
    m_ramp->AddElementByMassFraction(nist->GetElement("Cl"), 0.1/100);
    m_ramp->AddElementByMassFraction(nist->GetElement("K"), 0.1/100);
    m_ramp->AddElementByMassFraction(nist->GetElement("Ca"), 8.8/100);
    mat.insert({std::string("Ramp"), m_ramp});

    G4Material *m_heartwall = new G4Material("heartWall", 1.051*g/cm3, 9);
    m_heartwall->AddElementByMassFraction(nist->GetElement("H"), 10.4/100);
    m_heartwall->AddElementByMassFraction(nist->GetElement("C"), 13.5/100);
    m_heartwall->AddElementByMassFraction(nist->GetElement("N"), 2.9/100);
    m_heartwall->AddElementByMassFraction(nist->GetElement("O"), 72.3/100);
    m_heartwall->AddElementByMassFraction(nist->GetElement("Na"), 0.1/100);
    m_heartwall->AddElementByMassFraction(nist->GetElement("P"), 0.2/100);
    m_heartwall->AddElementByMassFraction(nist->GetElement("S"), 0.2/100);
    m_heartwall->AddElementByMassFraction(nist->GetElement("Cl"), 0.2/100);
    m_heartwall->AddElementByMassFraction(nist->GetElement("K"), 0.3/100);
    mat.insert({std::string("heartwall"), m_heartwall});

    G4Material *m_brain = new G4Material("brain", 1.041*g/cm3, 9);
    m_brain->AddElementByMassFraction(nist->GetElement("H"), 10.7/100);
    m_brain->AddElementByMassFraction(nist->GetElement("C"), 14.3/100);
    m_brain->AddElementByMassFraction(nist->GetElement("N"), 2.3/100);
    m_brain->AddElementByMassFraction(nist->GetElement("O"), 71.3/100);
    m_brain->AddElementByMassFraction(nist->GetElement("Na"), 0.2/100);
    m_brain->AddElementByMassFraction(nist->GetElement("P"), 0.4/100);
    m_brain->AddElementByMassFraction(nist->GetElement("S"), 0.2/100);
    m_brain->AddElementByMassFraction(nist->GetElement("Cl"), 0.3/100);
    m_brain->AddElementByMassFraction(nist->GetElement("K"), 0.3/100);
    mat.insert({std::string("brain"), m_brain});

    G4Material *m_lung = new G4Material("lung", 0.415*g/cm3, 10);
    m_lung->AddElementByMassFraction(nist->GetElement("H"), 10.2/100);
    m_lung->AddElementByMassFraction(nist->GetElement("C"), 10.8/100);
    m_lung->AddElementByMassFraction(nist->GetElement("N"), 3.2/100);
    m_lung->AddElementByMassFraction(nist->GetElement("O"), 74.8/100);
    m_lung->AddElementByMassFraction(nist->GetElement("Na"), 0.1/100);
    m_lung->AddElementByMassFraction(nist->GetElement("P"), 0.1/100);
    m_lung->AddElementByMassFraction(nist->GetElement("S"), 0.2/100);
    m_lung->AddElementByMassFraction(nist->GetElement("Cl"), 0.3/100);
    m_lung->AddElementByMassFraction(nist->GetElement("K"), 0.2/100);
    m_lung->AddElementByMassFraction(nist->GetElement("Fe"), 0.1/100);
    mat.insert({std::string("lung"), m_lung});

  }
} // namespace kcmh
