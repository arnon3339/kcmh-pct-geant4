#include "TrackerSD.hh"
#include "G4HCofThisEvent.hh"
#include "TrackerHit.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"

namespace kcmh
{
  TrackerSD::TrackerSD(const G4String& name,
    const G4String& hitsCollectionName)
  :G4VSensitiveDetector(name)
  {
    collectionName.insert(hitsCollectionName);
  }

  TrackerSD::~TrackerSD(){}

  void TrackerSD::Initialize(G4HCofThisEvent* hce)
  {
    fHitsCollection = new TrackerHitsCollection(SensitiveDetectorName,
     collectionName[0]);

    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection(hcID, fHitsCollection);
  }

  G4bool TrackerSD::ProcessHits(G4Step* step, G4TouchableHistory* history)
  {
    auto newHit = new TrackerHit();
    G4Track* track = step->GetTrack();
    G4double edep = step->GetTotalEnergyDeposit();
    if (track->GetParentID() != 0) return true;
    if (edep == 0.) return true;

  }
  
} // namespace kcmh
