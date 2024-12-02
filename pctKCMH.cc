#include "G4UIExecutive.hh"
#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "ActionInitialization.hh"
#include "PctDetectorConstruction.hh"
#include "LynxDetectorConstruction.hh"
#include "PhysicsList.hh"
#include "argparse.hh"
#include "utils.hh"
#include "CLHEP/Random/Random.h"

#include <iostream>
#include <filesystem>

using namespace kcmh;

int main(int argc, char **argv)
{
  std::filesystem::path exePath = std::filesystem::current_path();
  
  argparse::ArgumentParser program("pctKCMH"); 

  program.add_argument("--mode")
  .help("Simulation mode")
  .nargs(1)
  .default_value("pct");

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

  program.add_argument("-bt", "--translate")
  .help("beam translation (cm)")
  .nargs(3)
  .default_value(std::vector<float>{})
  .action([](const std::string& value) { return std::stof(value); });

  program.add_argument("--condorid")
  .help("using condor clusters and process")
  .nargs(2)
  .default_value(std::vector<int>{0, 0})
  .action([](const std::string& value) { return std::stoi(value); });

  program.add_argument("--thread", "-t")
  .help("The number of threads")
  .nargs(1)
  .default_value(1)
  .action([](const std::string& value) { return std::stoi(value); });

  program.add_argument("--optimized")
  .help("optimized beam")
  .default_value(false)
  .implicit_value(true);

  program.add_argument("--condor")
  .help("enabling condor")
  .default_value(false)
  .implicit_value(true);

  program.parse_args(argc, argv);

  auto simMode = program.get("--mode");
  auto macroFile = program.get("--macro");
  auto phName = program.get("--phantom");
  auto numOfThreads = program.get<int>("--thread");
  auto numOfBeam = program.get("--beam");
  auto beamEnergyArray = program.get<std::vector<float>>("--energy");
  auto rotationArray = program.get<std::vector<float>>("--rotate");
  auto beamTransArray = program.get<std::vector<float>>("--translate");
  auto condorID = program.get<std::vector<int>>("--condorid");

  G4UIExecutive* ui = nullptr;
  if (!simMode.compare("pct") && !macroFile.compare("init_vis.mac")) 
    ui = new G4UIExecutive(argc, argv);

  auto runManager = 
    G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
  runManager->SetNumberOfThreads(numOfThreads);

  runManager->SetUserInitialization(new PhysicsList());
  runManager->SetUserInitialization(new ActionInitialization(simMode));

  if (!G4StrUtil::icompare(simMode, "pct"))
  {
    if (!macroFile.compare("init_vis.mac"))
      runManager->SetUserInitialization(new PctDetectorConstruction(phName, true));
    else
      runManager->SetUserInitialization(new PctDetectorConstruction(phName));
  }
  else if (!G4StrUtil::icompare(simMode, "lynx"))
  {
    if (!macroFile.compare("init_vis.mac"))
      runManager->SetUserInitialization(new LynxDetectorConstruction(true));
    else
      runManager->SetUserInitialization(new LynxDetectorConstruction());
  }
  else
      runManager->SetUserInitialization(new PctDetectorConstruction(phName, true));

  auto UImanager = G4UImanager::GetUIpointer();
  G4VisManager* vis = new G4VisExecutive(argc, argv);
  vis->Initialize();

  G4String execCommand = "/control/execute ";

  if (macroFile.compare("init_vis.mac") || !simMode.compare("lynx"))
  {
    UImanager->ApplyCommand("/run/initialize");
    float newRotationArray[] = {0., 0., 1.};
    float newEnergyArray[] = {200., 200., 1.};
    float newBeamTransArray[] = {0., 0., 1.};

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

      if (beamTransArray.size())
        if (beamTransArray.size() < 2) 
        {
          newBeamTransArray[0] = beamTransArray[0];
          newBeamTransArray[1] = beamTransArray[0];
        }
        else if (beamTransArray.size() < 3)
        {
          newBeamTransArray[0] = beamTransArray[0];
          newBeamTransArray[1] = beamTransArray[1];
        }
        else
        {
          newBeamTransArray[0] = beamTransArray[0];
          newBeamTransArray[1] = beamTransArray[1];
          newBeamTransArray[2] = beamTransArray[2];
        }
    if (!G4StrUtil::icompare(simMode, "pct"))
    {
      UImanager->ApplyCommand(execCommand + macroFile);

      for (float energy = newEnergyArray[0]; energy <= newEnergyArray[1];
        energy += newEnergyArray[2])
      {
        auto beamEnergy = std::to_string(energy) + G4String(" MeV");
        UImanager->ApplyCommand("/gps/ene/mono " + beamEnergy);

        std::string outputPath;
        if (program["--condor"] == true)
        {
          std::ostringstream ossClusterID;
          std::ostringstream ossProcessID;
          ossClusterID << std::setw(3) << std::setfill('0') << std::to_string(condorID[0]);
          ossProcessID << std::setw(3) << std::setfill('0') << std::to_string(condorID[1]);
          outputPath = createSingleOutputDirs(
            exePath.string() + std::string("/dataout/") +
            ossClusterID.str() + std::string("/") + 
            ossProcessID.str(),
            energy
            );
        }
        else
        {
          outputPath = createSingleOutputDirs(exePath.string() +
            std::string("/dataout"), energy);
        }
        UImanager->ApplyCommand("/run/file/output " + outputPath);

        for (float angle = newRotationArray[0]; angle <= newRotationArray[1];
          angle += newRotationArray[2])
        {
          auto phAngle = std::to_string(angle) + G4String(" deg");
          UImanager->ApplyCommand("/det/phantom/rotate/angle " + phAngle);
          UImanager->ApplyCommand("/run/pct/det/ph/angle " + phAngle);

          for (float beamPosX = newBeamTransArray[0]; beamPosX <= newBeamTransArray[1];
            beamPosX += newBeamTransArray[2])
          {
            auto beamTransPosX = std::to_string(beamPosX) + G4String(" 0 -42.1 cm");
            UImanager->ApplyCommand("/gps/pos/centre " + beamTransPosX);
            UImanager->ApplyCommand("/run/pct/beam/posx " + 
              std::to_string(beamPosX) + G4String(" cm"));
            UImanager->ApplyCommand("/run/beamOn " + numOfBeam);
          }
        }
        UImanager->ApplyCommand("/run/file/close");
      }
    }
    else if (!G4StrUtil::icompare(simMode, "lynx"))
    {
      UImanager->ApplyCommand(execCommand + "beam_kcmh.mac");
      auto beamEnergy = std::to_string(newEnergyArray[0]) + G4String(" MeV");
      UImanager->ApplyCommand("/gps/ene/mono " + beamEnergy);
      std::string outputPath;
      if (program["--condor"] == true)
      {
        std::ostringstream ossClusterID;
        std::ostringstream ossProcessID;
        ossClusterID << std::setw(3) << std::setfill('0') << std::to_string(condorID[0]);
        ossProcessID << std::setw(3) << std::setfill('0') << std::to_string(condorID[1]);
        outputPath = createSingleOutputDirs(
          exePath.string() + std::string("/dataout/") +
          ossClusterID.str() + std::string("/") + 
          ossProcessID.str(),
          newEnergyArray[0]
          );
      }
      else
      {
        outputPath = createSingleOutputDirs(exePath.string() +
          std::string("/dataout"), newEnergyArray[0]);
      }
      UImanager->ApplyCommand("/run/file/output " + outputPath);
      for (int layerID = 0; layerID < 6; layerID++)
      {
        UImanager->ApplyCommand("/det/lynx/posz "+ std::to_string((layerID - 2.)*10.)
          + G4String(" cm"));
        UImanager->ApplyCommand("/run/lynx/det/layerID " + std::to_string(layerID));

        if (program["--optimized"] == true)
        {
            // UImanager->ApplyCommand("/gps/pos/sigma_r 3.6 mm");
            UImanager->ApplyCommand("/run/lynx/beam/sigma_r 3.6 mm");
            // UImanager->ApplyCommand("/gps/ang/sigma_r 0.057 deg");
            UImanager->ApplyCommand("/run/lynx/beam/sigma_a 0.004 deg");
            // UImanager->ApplyCommand("/gps/ene/sigma 0.1 MeV");
            UImanager->ApplyCommand("/run/lynx/beam/sigma_e 0.1 MeV");
            auto beamRunCmd = "/run/beamOn " + numOfBeam;
            UImanager->ApplyCommand(beamRunCmd);
        }
        else
        {
          for (int sigma_i = 0; sigma_i < 10; sigma_i++)
          {
            auto sigma = std::to_string(2. + (2./10.)*sigma_i) + G4String(" mm");
            UImanager->ApplyCommand("/gps/pos/sigma_r " + sigma);
            UImanager->ApplyCommand("/run/lynx/beam/sigma_r " + sigma);
            for (int sigma_ai = 0; sigma_ai < 10; sigma_ai++)
            {
              auto angleSigma = std::to_string(0.001 + 0.001*sigma_ai/10) +
                G4String(" deg");
              UImanager->ApplyCommand("/gps/ang/sigma_r " + angleSigma);
              UImanager->ApplyCommand("/run/lynx/beam/sigma_a " + angleSigma);
              UImanager->ApplyCommand("/gps/ene/sigma 0.1 MeV");
              UImanager->ApplyCommand("/run/lynx/beam/sigma_e 0.1 MeV");
              
              auto beamRunCmd = "/run/beamOn " + numOfBeam;
              UImanager->ApplyCommand(beamRunCmd);

              G4cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << G4endl;
              G4cout << "Sigma step: " << sigma_i << ", Sigma angle step: " << sigma_ai <<
                ", Sigma energy step: " << "0" << G4endl;
              G4cout << "Sigma: " << sigma << ", Sigma angle: " << angleSigma <<
                ", Sigma energy: " << "0.1 MeV" << G4endl;
              G4cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << G4endl;
            }
          }
        }
      }
      UImanager->ApplyCommand("/run/file/close");
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