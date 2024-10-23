#ifndef TRACKERSD_HH
#define TRACKERSD_HH 1

#include "G4VSensitiveDetector.hh"
#include "TrackerHit.hh"

namespace kcmh
{
class TrackerSD : public G4VSensitiveDetector
{
  public:
    TrackerSD(const G4String& name,
                const G4String& hitsCollectionName);
    ~TrackerSD() override;

    // methods from base class
    void   Initialize(G4HCofThisEvent* hitCollection) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    void   EndOfEvent(G4HCofThisEvent* hitCollection) override;

  private:
    TrackerHitsCollection* fHitsCollection = nullptr;
};
} // namespace kcmh

#endif