#include "EventAction.hh"

#include "TrackerHit.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "RunAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include "G4NistManager.hh"

#include <cmath>

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
    auto dtcDistance = 25. *mm;
    auto alpideSizeX = 30. *mm;
    auto alpideSizeY = 15. *mm;
    auto numPixelsXInAlpide = 1024;
    auto numPixelsYInAlpide = 512;
    auto pixelSizeX = alpideSizeX/numPixelsXInAlpide;
    auto pixelSizeY = alpideSizeY/numPixelsYInAlpide;

    auto analysisManager = G4AnalysisManager::Instance();
    if (dtcTrackerID < 0) return;

    auto hce = event->GetHCofThisEvent();
    TrackerHitsCollection* DTC = nullptr;

    if (hce) DTC = (TrackerHitsCollection*)hce->GetHC(dtcTrackerID);

    for (size_t i = 0; i < DTC->GetSize(); i++)
    {
      analysisManager->FillH1(0, (*DTC)[i]->GetLayerID());
      if (((*DTC)[i]->GetLayerID() != 0) && ((*DTC)[i]->GetLayerID() != 1) &&
        ((*DTC)[i-1]->GetLayerID() == ((*DTC)[i]->GetLayerID() - 1)))
      {
        G4double refVector[] = {0., 0., dtcDistance};
        G4double currentXY[] = {
          (*DTC)[i]->GetPixels()[0] * pixelSizeX,
          (*DTC)[i]->GetPixels()[1] * pixelSizeY
        };
        G4double prevXY[] = {
          (*DTC)[i-1]->GetPixels()[0] * pixelSizeX,
          (*DTC)[i-1]->GetPixels()[1] * pixelSizeY
        };

        G4double theVector[] = {
          currentXY[0] - prevXY[0],
          currentXY[1] - prevXY[1],
          dtcDistance
        };

        G4double dotProduct =
          refVector[0] * theVector[0] + refVector[1] * theVector[1] +
          refVector[2] * theVector[2];

        G4double normTheVector = std::hypot(theVector[0], theVector[1], theVector[2]);
        G4double normRefVector = std::hypot(refVector[0], refVector[1], refVector[2]);

        G4double angle = 0.;
        G4double productTwoNorms = normRefVector * normTheVector;

        if (dotProduct > productTwoNorms) angle = 0.;
        else angle = std::acos(std::clamp(dotProduct / productTwoNorms, -1.0, 1.0));

        if (std::abs(std::cos(angle)) > 1e-6) 
          analysisManager->FillNtupleDColumn(5, dtcDistance * (1 / std::cos(angle)));
        else analysisManager->FillNtupleDColumn(5, 0.);
      }
      else
        analysisManager->FillNtupleDColumn(5, 0.);
      analysisManager->FillNtupleIColumn(6, (*DTC)[i]->GetTrackID());
      analysisManager->FillNtupleIColumn(7, (*DTC)[i]->GetParentID());
      analysisManager->FillNtupleIColumn(8, (*DTC)[i]->GetPDGEncoding());
      analysisManager->AddNtupleRow();
    }
  }
} // namespace kcmh
