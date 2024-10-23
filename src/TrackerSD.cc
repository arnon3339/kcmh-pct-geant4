#include "TrackerSD.hh"
#include "G4HCofThisEvent.hh"
#include "TrackerHit.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"

#include <cmath>

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

    G4String volumeName = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume(3)->GetName();
    G4cout << "XXXXXXXXXXXXXXXXXX" << G4endl;
    if (volumeName.compare("trackerFlexPhys"))
      newHit->SetLayerID(step->GetPreStepPoint()->
        GetTouchableHandle()->GetReplicaNumber(6) + 1);
    else
      newHit->SetLayerID(0);
    newHit->SetPixels(
        step->GetPreStepPoint()->GetTouchableHandle()->GetReplicaNumber(1),
        step->GetPreStepPoint()->GetTouchableHandle()->GetReplicaNumber(2)
    );
    G4cout << newHit->GetLayerID() << G4endl;
    newHit->SetEdep(edep);
    newHit->SetTrackID(step->GetTrack()->GetTrackID());
    fHitsCollection->insert(newHit);
    G4cout << "XXXXXXXXXXXXXXXXXX" << G4endl;

    return true;
  }

  void TrackerSD::EndOfEvent(G4HCofThisEvent* hce)
  {
    if ( verboseLevel>1 ) {
    G4int nofHits = fHitsCollection->entries();
    G4cout << G4endl
          << "-------->Hits Collection: in this event they are " << nofHits
          << " hits in the tracker chambers: " << G4endl;
    for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
  }
  }
  
} // namespace kcmh
