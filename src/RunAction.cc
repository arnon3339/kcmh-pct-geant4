#include "RunAction.hh"
#include "G4Run.hh"

namespace kcmh
{
  RunAction::RunAction()
  {}

  RunAction::~RunAction()
  {}

  void RunAction::BeginOfRunAction(const G4Run*)
  {}

  void RunAction::EndOfRunAction(const G4Run*)
  {}
} // namespace kcmh
