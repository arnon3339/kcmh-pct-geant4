#ifndef RUNACTION_HH
#define RUNACTION_HH 1

#include "G4UserRunAction.hh"

class G4Run;

namespace kcmh
{
  class RunAction: public G4UserRunAction
  {
    public:
      RunAction();
      ~RunAction() override;

      void BeginOfRunAction(const G4Run*) override;
      void EndOfRunAction(const G4Run*) override;
  };
} // namespace kcmh


#endif