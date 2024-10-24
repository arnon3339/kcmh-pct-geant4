#include "G4UIExecutive.hh"
#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"

using namespace kcmh;

int main(int argc, char **argv)
{
  G4UIExecutive* ui = nullptr;
  if (argc == 1) ui = new G4UIExecutive(argc, argv);

  auto runManager = 
    G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
  runManager->SetNumberOfThreads(1);

  runManager->SetUserInitialization(new PhysicsList());
  runManager->SetUserInitialization(new ActionInitialization());
  runManager->SetUserInitialization(new DetectorConstruction("catphan404"));

  auto UImanager = G4UImanager::GetUIpointer();
  G4VisManager* vis = new G4VisExecutive(argc, argv);
  vis->Initialize();

  G4String execCommand = "/control/execute ";
  if (!ui)
  {
    UImanager->ApplyCommand(execCommand + argv[1]);
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