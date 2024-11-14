#ifndef PCTTRACKERSD_HH
#define PCTTRACKERSD_HH 1

#include "G4VSensitiveDetector.hh"
#include "TrackerHit.hh"

namespace kcmh
{
class PctTrackerSD : public G4VSensitiveDetector
{
  public:
    PctTrackerSD(const G4String& name,
                const G4String& hitsCollectionName);
    ~PctTrackerSD() override;

    // methods from base class
    void   Initialize(G4HCofThisEvent* hitCollection) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    void   EndOfEvent(G4HCofThisEvent* hitCollection) override;

  private:
    TrackerHitsCollection* fHitsCollection = nullptr;
};
} // namespace kcmh

#endif