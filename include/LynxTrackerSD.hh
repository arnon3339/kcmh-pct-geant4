#ifndef LYNXTRACKERSD_HH
#define LYNXTRACKERSD_HH 1

#include "G4VSensitiveDetector.hh"
#include "TrackerHit.hh"

namespace kcmh
{
class LynxTrackerSD : public G4VSensitiveDetector
{
  public:
    LynxTrackerSD(const G4String& name,
                const G4String& hitsCollectionName);
    ~LynxTrackerSD() override;

    // methods from base class
    void   Initialize(G4HCofThisEvent* hitCollection) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    void   EndOfEvent(G4HCofThisEvent* hitCollection) override;

  private:
    TrackerHitsCollection* fHitsCollection = nullptr;
};
} // namespace kcmh

#endif