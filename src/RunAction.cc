#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>
#include <fstream>

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
    analysisManager->CreateH1("Entries", "Number of entries in layers", numOfDtcLayer, 0, numOfDtcLayer);
    analysisManager->CreateH2("Layer0", "XY Histogram in the Tracker", 
      numAlpideCol*numOfPixelRow/100, 0, numAlpideCol*numOfPixelRow,
      numAlpideRow*numOfPixelCol/100, 0, numAlpideRow*numOfPixelCol);
    analysisManager->CreateNtuple("pCT", "pCT Hits");
    analysisManager->CreateNtupleIColumn("eventID");
    analysisManager->CreateNtupleIColumn("pixelX");
    analysisManager->CreateNtupleIColumn("pixelY");
    analysisManager->CreateNtupleIColumn("layerID");
    analysisManager->CreateNtupleDColumn("edep");
    analysisManager->CreateNtupleIColumn("trackID");
    analysisManager->CreateNtupleIColumn("parentID");
    analysisManager->CreateNtupleIColumn("PDGe");
    analysisManager->FinishNtuple();
  }

  RunAction::~RunAction()
  {}

  void RunAction::BeginOfRunAction(const G4Run*)
  {
    auto analysisManager = G4AnalysisManager::Instance();
    auto det = (DetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction();

    int angleDegree = (int)(det->GetPHangle()/CLHEP::degree);
    std::ostringstream oss;
    oss << std::setw(3) << std::setfill('0') << angleDegree;
    auto outFileName = "projection_" + oss.str() + ".root";
    auto outputDir = "./output/projection_" + oss.str();
    auto outFilePath = outputDir + "/" + outFileName;
    G4cout << outputDir << G4endl;
    try {
      // Create the directory
      if (std::filesystem::create_directory(outputDir)) {
          std::cout << "Directory created: " << outputDir << std::endl;
      } else {
          std::cout << "Directory already exists or couldn't be created: " << outputDir << std::endl;
      }
    } 
    catch (const std::filesystem::filesystem_error& e)
    {
      std::cerr << "Error: " << e.what() << std::endl;
    }

    analysisManager->OpenFile(outFilePath);
  }

  void RunAction::EndOfRunAction(const G4Run*)
  {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();  // Write all histograms to file
    analysisManager->CloseFile();  // Close the ROOT file

  }
} // namespace kcmh
