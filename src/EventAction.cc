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
  EventAction::EventAction(RunAction* runAction)
  :fRunAction(runAction){}

  EventAction::~EventAction(){}

  void EventAction::BeginOfEventAction(const G4Event*)
  {
    auto sdManager = G4SDManager::GetSDMpointer();
    const auto numOfSds = sdManager->GetHCtable()->entries();
    
    for (int i = 0; i < numOfSds; i++)
    {
      if (!sdManager->GetHCtable()->GetHCname(i).compare("dtcHitsCollection"))
        dtcTrackerID = sdManager->GetCollectionID("dtcHitsCollection");
      else if (!sdManager->GetHCtable()->GetHCname(i).compare("LynxHitsCollection"))
        lynxTrackerID = G4SDManager::GetSDMpointer()->
          GetCollectionID("LynxHitsCollection");
    }
  }

  void EventAction::EndOfEventAction(const G4Event* event)
  {
    if (dtcTrackerID < 0 && lynxTrackerID < 0) return;

    if (dtcTrackerID >= 0)
      collectDtcHits(event);
    else if (lynxTrackerID >= 0)
      collectLynxHits(event);
  }

  void EventAction::collectDtcHits(const G4Event* event)
  {
    auto dtcDistance = 25. *mm;
    auto alpideSizeX = 30. *mm;
    auto alpideSizeY = 15. *mm;
    auto numPixelsXInAlpide = 1024;
    auto numPixelsYInAlpide = 512;
    auto pixelSizeX = alpideSizeX/numPixelsXInAlpide;
    auto pixelSizeY = alpideSizeY/numPixelsYInAlpide;

    auto analysisManager = G4AnalysisManager::Instance();
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
      analysisManager->FillNtupleDColumn(3, (*DTC)[i]->GetPosZ());
      analysisManager->FillNtupleIColumn(4, (*DTC)[i]->GetLayerID());
      analysisManager->FillNtupleDColumn(5, fRunAction->GetPctLabeledPhAngle()/deg);
      analysisManager->FillNtupleDColumn(6, fRunAction->GetPctLabeledBeamPosX());
      analysisManager->FillNtupleDColumn(7, (*DTC)[i]->GetK());
      analysisManager->FillNtupleDColumn(8, (*DTC)[i]->GetEdep());

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
          analysisManager->FillNtupleDColumn(9, totalThickness * (1 / std::cos(angle)));
        else analysisManager->FillNtupleDColumn(9, totalThickness);
      }
      else
        analysisManager->FillNtupleDColumn(9, 0.);
      analysisManager->FillNtupleIColumn(10, (*DTC)[i]->GetTrackID());
      analysisManager->FillNtupleIColumn(11, (*DTC)[i]->GetParentID());
      analysisManager->FillNtupleIColumn(12, (*DTC)[i]->GetPDGEncoding());
      analysisManager->AddNtupleRow();
    }
  }

  void EventAction::collectLynxHits(const G4Event* event)
  {
    auto analysisManager = G4AnalysisManager::Instance();
    auto hce = event->GetHCofThisEvent();
    TrackerHitsCollection* LYNX = nullptr;

    if (hce) LYNX = (TrackerHitsCollection*)hce->GetHC(lynxTrackerID);

    for (size_t i = 0; i < LYNX->GetSize(); i++)
    {
      analysisManager->FillNtupleIColumn(0, event->GetEventID());
      analysisManager->FillNtupleIColumn(1, (*LYNX)[i]->GetPixels()[0]);
      analysisManager->FillNtupleIColumn(2, (*LYNX)[i]->GetPixels()[1]);
      analysisManager->FillNtupleDColumn(3, (*LYNX)[i]->GetPosZ());
      analysisManager->FillNtupleIColumn(4, fRunAction->GetLynxLabeledDetLayerID());
      analysisManager->FillNtupleDColumn(5, (*LYNX)[i]->GetK());
      analysisManager->FillNtupleDColumn(6, (*LYNX)[i]->GetEdep());
      analysisManager->FillNtupleDColumn(7, fRunAction->GetLynxLabeledBeamSigma());
      analysisManager->FillNtupleDColumn(8, fRunAction->GetLynxLabeledBeamSigmaR());
      analysisManager->FillNtupleDColumn(9, fRunAction->GetLynxLabeledBeamSigmaE());
      analysisManager->FillNtupleIColumn(10, (*LYNX)[i]->GetTrackID());
      analysisManager->FillNtupleIColumn(11, (*LYNX)[i]->GetParentID());
      analysisManager->FillNtupleIColumn(12, (*LYNX)[i]->GetPDGEncoding());
      analysisManager->AddNtupleRow();
    }
  }
} // namespace kcmh
