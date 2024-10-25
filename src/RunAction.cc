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
    G4int numAlpideCol = 9;
    G4int numAlpideRow = 12;
    G4int numOfDtcLayer = 42;
    G4int numOfPixelRow = 1024;
    G4int numOfPixelCol = 512;

    auto analysisManager = G4AnalysisManager::Instance(); 
    analysisManager->CreateH1("Entries", "Number of entries in layers", 42, 0, 42);
    analysisManager->CreateH2("Layer0", "XY Histogram in the Tracker", 
      numAlpideCol*numOfPixelRow/100, 0, numAlpideCol*numOfPixelRow,
      numAlpideRow*numOfPixelCol/100, 0, numAlpideRow*numOfPixelCol);
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
