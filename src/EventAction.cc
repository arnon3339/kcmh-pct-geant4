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
      if ((*DTC)[i]->GetLayerID() == 0)
        analysisManager->FillH2(0, (*DTC)[i]->GetPixels()[0], 
          (*DTC)[i]->GetPixels()[1]);

      analysisManager->FillNtupleIColumn(0, event->GetEventID());
      analysisManager->FillNtupleIColumn(1, (*DTC)[i]->GetPixels()[0]);
      analysisManager->FillNtupleIColumn(2, (*DTC)[i]->GetPixels()[1]);
      analysisManager->FillNtupleIColumn(3, (*DTC)[i]->GetLayerID());
      analysisManager->FillNtupleDColumn(4, (*DTC)[i]->GetEdep());

      if ((i > 0) && ((*DTC)[i-1]->GetLayerID() != ((*DTC)[i]->GetLayerID())))
      {
        G4double currentXYZ[] = {
          (*DTC)[i]->GetPixels()[0] * pixelSizeX,
          (*DTC)[i]->GetPixels()[1] * pixelSizeY,
          (*DTC)[i]->GetLayerID() * dtcDistance
        };
        G4double prevXYZ[] = {
          (*DTC)[i-1]->GetPixels()[0] * pixelSizeX,
          (*DTC)[i-1]->GetPixels()[1] * pixelSizeY,
          (*DTC)[i-1]->GetLayerID() * dtcDistance
        };
        G4double totalThickness = std::abs(currentXYZ[2] - prevXYZ[2]);
        if (((*DTC)[i]->GetLayerID() == 0) || ((*DTC)[i-1]->GetLayerID() == 0))
          totalThickness -= dtcDistance;

        G4double refVector[] = {0., 0., dtcDistance};
        G4double theVector[] = {
          currentXYZ[0] - prevXYZ[0],
          currentXYZ[1] - prevXYZ[1],
          currentXYZ[2] - prevXYZ[2]
        };

        G4double dotProduct =
          refVector[0] * theVector[0] + refVector[1] * theVector[1] +
          refVector[2] * theVector[2];

        G4double normTheVector = std::hypot(theVector[0], theVector[1], theVector[2]);
        G4double normRefVector = std::hypot(refVector[0], refVector[1], refVector[2]);

        G4double productTwoNorms = normRefVector * normTheVector;

        G4double angle = std::acos(std::clamp(dotProduct / productTwoNorms, -1.0, 1.0));

        if (std::abs(std::cos(angle)) > 1e-6) 
          analysisManager->FillNtupleDColumn(5, totalThickness * (1 / std::cos(angle)));
        else analysisManager->FillNtupleDColumn(5, totalThickness);
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
