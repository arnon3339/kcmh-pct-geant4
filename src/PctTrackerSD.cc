#include "PctTrackerSD.hh"
#include "G4HCofThisEvent.hh"
#include "TrackerHit.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"

#include <cmath>

namespace kcmh
{
  PctTrackerSD::PctTrackerSD(const G4String& name,
    const G4String& hitsCollectionName)
  :G4VSensitiveDetector(name)
  {
    collectionName.insert(hitsCollectionName);
  }

  PctTrackerSD::~PctTrackerSD(){}

  void PctTrackerSD::Initialize(G4HCofThisEvent* hce)
  {
    fHitsCollection = new TrackerHitsCollection(SensitiveDetectorName,
     collectionName[0]);

    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection(hcID, fHitsCollection);
  }

  G4bool PctTrackerSD::ProcessHits(G4Step* step, G4TouchableHistory* history)
  {
    auto newHit = new TrackerHit();
    G4Track* track = step->GetTrack();
    G4double edep = step->GetTotalEnergyDeposit();
    if (track->GetParentID() != 0) return true;
    if (edep < 3.6 *eV) return true;

    G4String volumeName = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume(3)->GetName();
    if (volumeName.compare("trackerFlexPhys"))
    {
      newHit->SetLayerID(step->GetPreStepPoint()->
        GetTouchableHandle()->GetReplicaNumber(6) + 1);
      newHit->SetPixels(
          step->GetPreStepPoint()->GetTouchableHandle()->GetReplicaNumber(4)*1024 +
          step->GetPreStepPoint()->GetTouchableHandle()->GetReplicaNumber(1),
          step->GetPreStepPoint()->GetTouchableHandle()->GetReplicaNumber(5)*512 +
          step->GetPreStepPoint()->GetTouchableHandle()->GetReplicaNumber(2)
      );
    }
    else
    {
      newHit->SetLayerID(0);
      newHit->SetPixels(
          step->GetPreStepPoint()->GetTouchableHandle()->GetReplicaNumber(3)*1024 +
          step->GetPreStepPoint()->GetTouchableHandle()->GetReplicaNumber(1),
          step->GetPreStepPoint()->GetTouchableHandle()->GetReplicaNumber(4)*512 +
          step->GetPreStepPoint()->GetTouchableHandle()->GetReplicaNumber(2)
      );

    }
    newHit->SetEdep(edep);
    newHit->SetK(step->GetPostStepPoint()->GetKineticEnergy());
    newHit->SetPDGEncoding(track->GetParticleDefinition()->GetPDGEncoding());
    newHit->SetParentID(track->GetParentID());
    newHit->SetTrackID(step->GetTrack()->GetTrackID());
    newHit->SetPosZ(step->GetPreStepPoint()->GetPosition().getZ());
    fHitsCollection->insert(newHit);

    return true;
  }

  void PctTrackerSD::EndOfEvent(G4HCofThisEvent* hce)
  {
    if (verboseLevel == 1)
    {
      G4int nofHits = fHitsCollection->GetSize();
      G4cout << G4endl
            << "-------->Hits Collection: in this event they are " << nofHits
            << " hits in the tracker chambers: " << G4endl;
      for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
      }
    }
  
} // namespace kcmh
