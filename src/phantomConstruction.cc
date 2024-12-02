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
    auto nonVis = new G4VisAttributes();
    nonVis->SetVisibility(false);

    G4bool checkOverlaps = true;

    if(!phName.compare(refPhantoms.at(0))){
      fPhIndex = 1;
      G4double height = 8. *cm;
      auto kcmhWrapSol = new G4Box(
        "kcmhWrapSol", 8. *cm, 8. *cm, 8. *cm
      );
      auto kcmhSol = new G4Tubs(
        "kcmhSol",  // Name
        0,          // Inner radius (can be 0 for solid cylinder)
        5. *cm,          // Outer radius
        height/2,             // Half-height along Z-axis
        0*deg,                // Starting angle (0 degrees)
        90*deg                // Angular span (90 degrees for one-fourth cylinder)
      );

      auto RMatrix = new G4RotationMatrix();
      RMatrix->rotateX(90 *deg);

      auto kcmhLV = new G4LogicalVolume(
        kcmhSol, phanMat["Acrylic"], "kcmhLog"
      );
      auto kcmhWrapLV = new G4LogicalVolume(
        kcmhWrapSol, phanMat["Air"], "kcmhWrapLog"
      );

      kcmhWrapLV->SetVisAttributes(nonVis);

      new G4PVPlacement(
        RMatrix,
        G4ThreeVector(0, 0, 0),
        kcmhLV,
        "kcmhWrapPhys",
        kcmhWrapLV,
        false,
        0,
        checkOverlaps
      );

      phLV = kcmhWrapLV;
    }
  }

  void PhantomConstruction::genMat(std::map<std::string, G4Material*> &mat)
  {
    auto nist = G4NistManager::Instance();
    mat.insert({std::string("Air"), nist->FindOrBuildMaterial("G4_AIR")});
    mat.insert({std::string("Water"), nist->FindOrBuildMaterial("G4_WATER")});
    
    G4Material *m_acrylic_low = new G4Material("myAcrylicLow", 1.14*g/cm3, 3);
    G4Material *m_acrylic = new G4Material("myAcrylic", 1.18*g/cm3, 3);

    m_acrylic->AddElementByMassFraction(nist->FindOrBuildElement("C"), 0.621);  // 62.1% Carbon by mass
    m_acrylic->AddElementByMassFraction(nist->FindOrBuildElement("H"), 0.050);  // 5.0% Hydrogen by mass
    m_acrylic->AddElementByMassFraction(nist->FindOrBuildElement("O"), 0.329);  // 32.9% Oxygen by mass

    m_acrylic_low->AddElementByMassFraction(nist->FindOrBuildElement("C"), 0.621);  // 62.1% Carbon by mass
    m_acrylic_low->AddElementByMassFraction(nist->FindOrBuildElement("H"), 0.050);  // 5.0% Hydrogen by mass
    m_acrylic_low->AddElementByMassFraction(nist->FindOrBuildElement("O"), 0.329);  // 32.9% Oxygen by mass
    mat.insert({std::string("AcrylicLow"), m_acrylic_low});
    mat.insert({std::string("Acrylic"), m_acrylic});

    G4Material *m_pmp = new G4Material("myPMP", 0.853*g/cm3, 2);
    m_pmp->AddElementByMassFraction(nist->FindOrBuildElement("C"), 0.625);
    m_pmp->AddElementByMassFraction(nist->FindOrBuildElement("H"), 0.375);
    mat.insert({std::string("PMP"), m_pmp});

    G4Material *m_ldpe = new G4Material("myLDPE", 0.945*g/cm3, 2);
    m_ldpe->AddElementByMassFraction(nist->FindOrBuildElement("C"), 0.858);
    m_ldpe->AddElementByMassFraction(nist->FindOrBuildElement("H"), 0.142);
    mat.insert({std::string("LDPE"), m_ldpe});

    G4Material *m_polystr = new G4Material("myPolystr", 0.998*g/cm3, 2);
    m_polystr->AddElementByMassFraction(nist->FindOrBuildElement("C"), 0.927);
    m_polystr->AddElementByMassFraction(nist->FindOrBuildElement("H"), 0.073);
    mat.insert({std::string("Polystyrene"), m_polystr});

    G4Material *m_delrin = new G4Material("myDelrin", 1.363*g/cm3, 3);
    m_delrin->AddElementByMassFraction(nist->FindOrBuildElement("C"), 0.613);
    m_delrin->AddElementByMassFraction(nist->FindOrBuildElement("H"), 0.125);
    m_delrin->AddElementByMassFraction(nist->FindOrBuildElement("O"), 0.262);
    mat.insert({std::string("Delrin"), m_delrin});
    
    G4Material *m_teflon = new G4Material("myTeflon", 1.868*g/cm3, 2);
    m_teflon->AddElementByMassFraction(nist->FindOrBuildElement("C"), 0.241);
    m_teflon->AddElementByMassFraction(nist->FindOrBuildElement("F"), 0.759);
    mat.insert({std::string("Teflon"), m_teflon});

    G4Material *m_ramp = new G4Material("myRamp", 1.252*g/cm3, 11);
    m_ramp->AddElementByMassFraction(nist->FindOrBuildElement("H"), 7.9/100);
    m_ramp->AddElementByMassFraction(nist->FindOrBuildElement("C"), 34.5/100);
    m_ramp->AddElementByMassFraction(nist->FindOrBuildElement("N"), 2.9/100);
    m_ramp->AddElementByMassFraction(nist->FindOrBuildElement("O"), 41.3/100);
    m_ramp->AddElementByMassFraction(nist->FindOrBuildElement("Na"), 0.2/100);
    m_ramp->AddElementByMassFraction(nist->FindOrBuildElement("Mg"), 0.1/100);
    m_ramp->AddElementByMassFraction(nist->FindOrBuildElement("F"), 3.9/100);
    m_ramp->AddElementByMassFraction(nist->FindOrBuildElement("S"), 0.2/100);
    m_ramp->AddElementByMassFraction(nist->FindOrBuildElement("Cl"), 0.1/100);
    m_ramp->AddElementByMassFraction(nist->FindOrBuildElement("K"), 0.1/100);
    m_ramp->AddElementByMassFraction(nist->FindOrBuildElement("Ca"), 8.8/100);
    mat.insert({std::string("Ramp"), m_ramp});

    G4Material *m_heartwall = new G4Material("heartWall", 1.051*g/cm3, 9);
    m_heartwall->AddElementByMassFraction(nist->FindOrBuildElement("H"), 10.4/100);
    m_heartwall->AddElementByMassFraction(nist->FindOrBuildElement("C"), 13.5/100);
    m_heartwall->AddElementByMassFraction(nist->FindOrBuildElement("N"), 2.9/100);
    m_heartwall->AddElementByMassFraction(nist->FindOrBuildElement("O"), 72.3/100);
    m_heartwall->AddElementByMassFraction(nist->FindOrBuildElement("Na"), 0.1/100);
    m_heartwall->AddElementByMassFraction(nist->FindOrBuildElement("P"), 0.2/100);
    m_heartwall->AddElementByMassFraction(nist->FindOrBuildElement("S"), 0.2/100);
    m_heartwall->AddElementByMassFraction(nist->FindOrBuildElement("Cl"), 0.2/100);
    m_heartwall->AddElementByMassFraction(nist->FindOrBuildElement("K"), 0.3/100);
    mat.insert({std::string("heartwall"), m_heartwall});

    G4Material *m_brain = new G4Material("brain", 1.041*g/cm3, 9);
    m_brain->AddElementByMassFraction(nist->FindOrBuildElement("H"), 10.7/100);
    m_brain->AddElementByMassFraction(nist->FindOrBuildElement("C"), 14.3/100);
    m_brain->AddElementByMassFraction(nist->FindOrBuildElement("N"), 2.3/100);
    m_brain->AddElementByMassFraction(nist->FindOrBuildElement("O"), 71.3/100);
    m_brain->AddElementByMassFraction(nist->FindOrBuildElement("Na"), 0.2/100);
    m_brain->AddElementByMassFraction(nist->FindOrBuildElement("P"), 0.4/100);
    m_brain->AddElementByMassFraction(nist->FindOrBuildElement("S"), 0.2/100);
    m_brain->AddElementByMassFraction(nist->FindOrBuildElement("Cl"), 0.3/100);
    m_brain->AddElementByMassFraction(nist->FindOrBuildElement("K"), 0.3/100);
    mat.insert({std::string("brain"), m_brain});

    G4Material *m_lung = new G4Material("lung", 0.415*g/cm3, 10);
    m_lung->AddElementByMassFraction(nist->FindOrBuildElement("H"), 10.2/100);
    m_lung->AddElementByMassFraction(nist->FindOrBuildElement("C"), 10.8/100);
    m_lung->AddElementByMassFraction(nist->FindOrBuildElement("N"), 3.2/100);
    m_lung->AddElementByMassFraction(nist->FindOrBuildElement("O"), 74.8/100);
    m_lung->AddElementByMassFraction(nist->FindOrBuildElement("Na"), 0.1/100);
    m_lung->AddElementByMassFraction(nist->FindOrBuildElement("P"), 0.1/100);
    m_lung->AddElementByMassFraction(nist->FindOrBuildElement("S"), 0.2/100);
    m_lung->AddElementByMassFraction(nist->FindOrBuildElement("Cl"), 0.3/100);
    m_lung->AddElementByMassFraction(nist->FindOrBuildElement("K"), 0.2/100);
    m_lung->AddElementByMassFraction(nist->FindOrBuildElement("Fe"), 0.1/100);
    mat.insert({std::string("lung"), m_lung});

  }
} // namespace kcmh
