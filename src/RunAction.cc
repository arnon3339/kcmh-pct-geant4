#include "RunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"
#include "TFile.h"
#include "G4SystemOfUnits.hh"
#include <string>

namespace kcmh
{
  RunAction::RunAction()
  {
    auto analysisManager = G4AnalysisManager::Instance(); 
    analysisManager->CreateH1("Entries", "Number of entries in layers", 43, 0, 43);
    analysisManager->CreateNtuple("pCT", "pCT Hits");
    analysisManager->CreateNtupleIColumn("eventID");
    analysisManager->CreateNtupleDColumn("pixelX");
    analysisManager->CreateNtupleDColumn("pixelY");
    analysisManager->CreateNtupleDColumn("layerID");
    analysisManager->CreateNtupleDColumn("edep");
    analysisManager->CreateNtupleDColumn("parentID");
    analysisManager->CreateNtupleDColumn("parentID");
    analysisManager->CreateNtupleDColumn("trackID");
    analysisManager->FinishNtuple();
  }

  RunAction::~RunAction()
  {}

  void RunAction::BeginOfRunAction(const G4Run*)
  {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile("./output.root");
  }

  void RunAction::EndOfRunAction(const G4Run*)
  {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();  // Write all histograms to file
    analysisManager->CloseFile();  // Close the ROOT file
  }
} // namespace kcmh
