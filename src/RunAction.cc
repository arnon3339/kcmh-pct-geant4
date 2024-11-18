#include "RunAction.hh"
#include "PctDetectorConstruction.hh"
#include "RunActionMessenger.hh"
#include "G4Run.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4String.hh"
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/variance.hpp>

#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>
#include <fstream>
#include <memory>

using namespace boost::accumulators;

namespace kcmh
{
  RunAction::RunAction(const G4int simMode)
  : fRunActionMessenger(new RunActionMessenger(this)), fSimMode(simMode),
  beamSigma(0), beamSigmaR(0)
  {

    auto analysisManager = G4AnalysisManager::Instance(); 

    if (simMode == 0)
    {
      G4int numAlpideCol = 9;
      G4int numAlpideRow = 12;
      G4int numOfDtcLayer = 42;
      G4int numOfPixelRow = 1024;
      G4int numOfPixelCol = 512;
      analysisManager->CreateH1("Entries", "Number of entries in layers",
        numOfDtcLayer, 0, numOfDtcLayer);
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
    if (fSimMode == 0)
    {
      auto analysisManager = G4AnalysisManager::Instance();
      analysisManager->Write();  // Write all histograms to file
      analysisManager->CloseFile();  // Close the ROOT file
    }
  }

  void RunAction::InitLynxAcc()
  {
    for (int i = 0; i < 6; i++)
    {
      fAccX[i] = std::make_unique<accumulator_set<double,
        features<tag::mean, tag::variance>>>();
      fAccY[i] = std::make_unique<accumulator_set<double,
        features<tag::mean, tag::variance>>>();
    }
  }

  void RunAction::ResetLynxAcc()
  {
    for (int i = 0; i < 6; i++)
    {
      fAccX[i].reset();
      fAccY[i].reset();
    }
  }

  void RunAction::AddAccValues(G4int* xy)
  {
    (*fAccX[fLayerID])(xy[0]);
    (*fAccY[fLayerID])(xy[1]);
  }

  void RunAction::CalLynx()
  {

  }
} // namespace kcmh
