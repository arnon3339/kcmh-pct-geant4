#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "RunActionMessenger.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4String.hh"

#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>
#include <fstream>

namespace kcmh
{
  RunAction::RunAction()
  : fRunActionMessenger(new RunActionMessenger(this))
  {
    G4int numAlpideCol = 9;
    G4int numAlpideRow = 12;
    G4int numOfDtcLayer = 42;
    G4int numOfPixelRow = 1024;
    G4int numOfPixelCol = 512;

    auto analysisManager = G4AnalysisManager::Instance(); 
    analysisManager->CreateH1("Entries", "Number of entries in layers", numOfDtcLayer, 0, numOfDtcLayer);
    analysisManager->CreateH2("Layer0", "XY Histogram in the Tracker", 
      numAlpideCol*numOfPixelRow/100, 0, numAlpideCol*numOfPixelRow,
      numAlpideRow*numOfPixelCol/100, 0, numAlpideRow*numOfPixelCol);
    analysisManager->CreateNtuple("pCT", "pCT Hits");
    analysisManager->CreateNtupleIColumn("eventID");
    analysisManager->CreateNtupleIColumn("pixelX");
    analysisManager->CreateNtupleIColumn("pixelY");
    analysisManager->CreateNtupleIColumn("layerID");
    analysisManager->CreateNtupleDColumn("K");
    analysisManager->CreateNtupleDColumn("edep");
    analysisManager->CreateNtupleDColumn("thickness");
    analysisManager->CreateNtupleIColumn("trackID");
    analysisManager->CreateNtupleIColumn("parentID");
    analysisManager->CreateNtupleIColumn("PDGe");
    analysisManager->FinishNtuple();
  }

  RunAction::~RunAction()
  {
    delete fRunActionMessenger;
  }

  void RunAction::BeginOfRunAction(const G4Run*){}

  void RunAction::OpenOutputFile(const G4String& outputFile)
  {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile(outputFile);
  }

  void RunAction::EndOfRunAction(const G4Run*)
  {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();  // Write all histograms to file
    analysisManager->CloseFile();  // Close the ROOT file
  }
} // namespace kcmh
