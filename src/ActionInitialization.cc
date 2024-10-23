#include "ActionInitialization.hh"
#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"

namespace kcmh
{
  void ActionInitialization::Build() const
  {
    SetUserAction(new PrimaryGeneratorAction());
    auto runAction = new RunAction();
    SetUserAction(runAction);
  }

  void ActionInitialization::BuildForMaster() const
  {
    SetUserAction(new RunAction());
  }
} // namespace kcmh
