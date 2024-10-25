#include "EventAction.hh"

#include "TrackerHit.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "RunAction.hh"
#include "G4AnalysisManager.hh"

namespace kcmh
{
  EventAction::EventAction(const RunAction* runAction)
  :fRunAction(runAction){}

  EventAction::~EventAction(){}

  void EventAction::BeginOfEventAction(const G4Event*)
  {
    dtcTrackerID = G4SDManager::GetSDMpointer()->
      GetCollectionID("dtcHitsCollection");
  }

  void EventAction::EndOfEventAction(const G4Event* event)
  {
    auto analysisManager = G4AnalysisManager::Instance();
    G4cout << "Event: " << event->GetEventID() << G4endl;
    if (dtcTrackerID < 0) return;

    auto hce = event->GetHCofThisEvent();
    TrackerHitsCollection* DTC = nullptr;

    if (hce) DTC = (TrackerHitsCollection*)hce->GetHC(dtcTrackerID);

    for (size_t i = 0; i < DTC->GetSize(); i++)
    {
      analysisManager->FillH1(0, (*DTC)[i]->GetLayerID());
      if ((*DTC)[i]->GetLayerID() == 0)
        analysisManager->FillH2(0, (*DTC)[i]->GetPixels()[0], 
          (*DTC)[i]->GetPixels()[0]);
    }
  }
} // namespace kcmh
