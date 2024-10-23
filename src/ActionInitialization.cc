#include "ActionInitialization.hh"
#include "RunAction.hh"

namespace kcmh
{
  void ActionInitialization::Build() const
  {
    SetUserAction(new RunAction());
  }

  void ActionInitialization::BuildForMaster() const
  {
    auto runAction = new RunAction();
    SetUserAction(runAction);
  }
} // namespace kcmh
