#include "G4UIExecutive.hh"
#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "argparse.hh"
#include "utils.hh"
#include "CLHEP/Random/Random.h"

#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>
#include <fstream>

using namespace kcmh;

int main(int argc, char **argv)
{
  CLHEP::HepRandom::setTheSeed(123456);
  try
  {
    if (std::filesystem::create_directory("./output")){}
  }
  catch(const std::filesystem::filesystem_error& e)
  {
    exit(EXIT_FAILURE);
  }
  
  argparse::ArgumentParser program("pctKCMH"); 

  program.add_argument("--macro", "-m")
  .help("The macro file")
  .nargs(1)
  .default_value("init_vis.mac");

  program.add_argument("--phantom", "-p")
  .help("The phantom name")
  .nargs(1)
  .default_value("catphan404");

  program.add_argument("--beam", "-b")
  .help("The number of beam")
  .nargs(1)
  .default_value("1000");

  program.add_argument("--energy", "-e")
  .help("The beam energy (MeV)")
  .nargs(3)
  .default_value(std::vector<float>{})
  .action([](const std::string& value) { return std::stof(value); });

  program.add_argument("-r", "--rotate")
  .help("Rotation array of a phantom (degree)")
  .nargs(3)
  .default_value(std::vector<float>{})
  .action([](const std::string& value) { return std::stof(value); });

  program.add_argument("--thread", "-t")
  .help("The number of threads")
  .nargs(1)
  .default_value(1)
  .action([](const std::string& value) { return std::stoi(value); });

  program.parse_args(argc, argv);

  auto macroFile = program.get("--macro");
  auto phName = program.get("--phantom");
  auto numOfThreads = program.get<int>("--thread");
  auto numOfBeam = program.get("--beam");
  auto beamEnergyArray = program.get<std::vector<float>>("--energy");
  auto rotationArray = program.get<std::vector<float>>("--rotate");

  G4UIExecutive* ui = nullptr;
  if (argc == 1) ui = new G4UIExecutive(argc, argv);

  auto runManager = 
    G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
  runManager->SetNumberOfThreads(numOfThreads);

  runManager->SetUserInitialization(new PhysicsList());
  runManager->SetUserInitialization(new ActionInitialization());
  if (!macroFile.compare("init_vis.mac"))
    runManager->SetUserInitialization(new DetectorConstruction(phName, true));
  else
    runManager->SetUserInitialization(new DetectorConstruction(phName));

  auto UImanager = G4UImanager::GetUIpointer();
  G4VisManager* vis = new G4VisExecutive(argc, argv);
  vis->Initialize();

  G4String execCommand = "/control/execute ";
  if (!ui && macroFile.compare("init_vis.mac"))
  {

    UImanager->ApplyCommand("/run/initialize");
    UImanager->ApplyCommand(execCommand + macroFile);
    float newRotationArray[] = {0., 0., 1.};
    float newEnergyArray[] = {200., 200., 1.};

    if (rotationArray.size())
      if (rotationArray.size() < 2)
      {
        newRotationArray[0] = rotationArray[0];
        newRotationArray[1] = rotationArray[0];
      }
      else if (rotationArray.size() < 3)
      {
        newRotationArray[0] = rotationArray[0];
        newRotationArray[1] = rotationArray[1];
      }
      else
      {
        newRotationArray[0] = rotationArray[0];
        newRotationArray[1] = rotationArray[1];
        newRotationArray[2] = rotationArray[2];
      }

    if (beamEnergyArray.size())
      if (beamEnergyArray.size() < 2) 
      {
        newEnergyArray[0] = beamEnergyArray[0];
        newEnergyArray[1] = beamEnergyArray[0];
      }
      else if (beamEnergyArray.size() < 3)
      {
        newEnergyArray[0] = beamEnergyArray[0];
        newEnergyArray[1] = beamEnergyArray[1];
      }
      else
      {
        newEnergyArray[0] = beamEnergyArray[0];
        newEnergyArray[1] = beamEnergyArray[1];
        newEnergyArray[2] = beamEnergyArray[2];
      }

    for (float energy = newEnergyArray[0]; energy <= newEnergyArray[1];
      energy += newEnergyArray[2])
    {
      for (float angle = newRotationArray[0]; angle <= newRotationArray[1];
        angle += newRotationArray[2])
      {
        auto outputPath = createOutputDirs(energy, angle);
        auto runOutputFileCmd = "/run/file/output " + outputPath;
        auto rotatePhCmd = "/det/phantom/rotate/angle " + 
          std::to_string(angle) + " deg";
        auto beamEnergyCmd = "/gps/ene/mono " + std::to_string(energy) + " MeV";
        auto beamRunCmd = "/run/beamOn " + numOfBeam;
        UImanager->ApplyCommand(runOutputFileCmd);
        UImanager->ApplyCommand(beamEnergyCmd);
        UImanager->ApplyCommand(rotatePhCmd);
        UImanager->ApplyCommand(beamRunCmd);
      }
    }
  }
  else
  {
    UImanager->ApplyCommand(execCommand + "init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  delete runManager;
  delete vis;

  return 0;
}