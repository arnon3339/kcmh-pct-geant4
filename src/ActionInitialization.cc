#include "ActionInitialization.hh"
#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"

namespace kcmh
{
  void ActionInitialization::Build() const
  {
    SetUserAction(new PrimaryGeneratorAction());

    auto runAction = new RunAction();
    SetUserAction(runAction);

    auto eventAction = new EventAction(runAction);
    SetUserAction(eventAction);
  }

  void ActionInitialization::BuildForMaster() const
  {
    SetUserAction(new RunAction());
  }
} // namespace kcmh
