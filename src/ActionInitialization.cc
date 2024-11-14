#include "ActionInitialization.hh"

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"
#include "G4SystemOfUnits.hh"

namespace kcmh
{
  ActionInitialization::ActionInitialization(G4String mode)
  {
    if (!G4StrUtil::icompare(mode, "pct"))
      fSimMode = 0;
    else if ((!G4StrUtil::icompare(mode, "cal")) || 
      (!G4StrUtil::icompare(mode, "lynx")))
      fSimMode = 1;
  }

  ActionInitialization::~ActionInitialization()
  {}

  void ActionInitialization::Build() const
  {
    SetUserAction(new PrimaryGeneratorAction());

    auto runAction = new RunAction(fSimMode);
    SetUserAction(runAction);

    auto eventAction = new EventAction(runAction);
    SetUserAction(eventAction);
  }

  void ActionInitialization::BuildForMaster() const
  {
    SetUserAction(new RunAction(fSimMode));
  }
} // namespace kcmh
