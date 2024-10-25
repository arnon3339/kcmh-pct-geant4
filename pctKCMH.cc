#include "G4UIExecutive.hh"
#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "argparse.hh"

#include <iostream>
#include <string>
#include <filesystem>

using namespace kcmh;

int main(int argc, char **argv)
{
  argparse::ArgumentParser program("pctKCMH"); 

  program.add_argument("--macro", "-m")
  .help("The macro file")
  .default_value("init_vis.mac");

  program.add_argument("--phantom", "-p")
  .help("The phantom name")
  .default_value("catphan404");

  program.add_argument("--beam", "-b")
  .help("The number of beam")
  .default_value(1000)
  .action([](const std::string& value) { return std::stoi(value); });

  program.add_argument("-r", "--rotate")
  .help("Rotation array of a phantom")
  .nargs('+')
  .default_value(std::vector<float>{})
  .action([](const std::string& value) { return std::stof(value); });

  program.add_argument("--thread", "-t")
  .help("The number of threads")
  .default_value(1)
  .action([](const std::string& value) { return std::stoi(value); });

  program.parse_args(argc, argv);

  auto macroFile = program.get("--macro");
  auto phName = program.get("--phantom");
  auto numOfThreads = program.get<int>("--thread");
  auto numOfBeam = program.get<int>("--beam");
  auto rotationArray = program.get<std::vector<float>>("--rotate");
  
  const char* outputDir = "./output";
  try {
    // Create the directory
    if (std::filesystem::create_directory(outputDir)) {
        std::cout << "Directory created: " << outputDir << std::endl;
    } else {
      std::filesystem::remove_all(outputDir);
      std::filesystem::create_directory(outputDir);
    }
  } 
  catch (const std::filesystem::filesystem_error& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  G4UIExecutive* ui = nullptr;
  if (argc == 1) ui = new G4UIExecutive(argc, argv);

  auto runManager = 
    G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
  runManager->SetNumberOfThreads(numOfThreads);

  runManager->SetUserInitialization(new PhysicsList());
  runManager->SetUserInitialization(new ActionInitialization());
  runManager->SetUserInitialization(new DetectorConstruction(phName));

  auto UImanager = G4UImanager::GetUIpointer();
  G4VisManager* vis = new G4VisExecutive(argc, argv);
  vis->Initialize();

  G4String execCommand = "/control/execute ";
  if (!ui && macroFile.compare("init_vis.mac"))
  {
    UImanager->ApplyCommand(execCommand + macroFile);
    float newRotationArray[] = {0., 0., 1.};

    if (rotationArray.size())
      if (rotationArray.size() < 2) newRotationArray[1] = rotationArray[0];
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

    for (float angle = newRotationArray[0]; angle <= newRotationArray[1];
     angle += newRotationArray[2])
    {
      auto rotatePhCmd = "/det/phantom/rotate/angle " + 
        std::to_string(angle) + " deg";
      auto beamRunCmd = "/run/beamOn " + std::to_string(numOfBeam);
      UImanager->ApplyCommand(rotatePhCmd);
      UImanager->ApplyCommand(beamRunCmd);
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