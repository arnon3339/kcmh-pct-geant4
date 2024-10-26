#ifndef RUNACTION_HH
#define RUNACTION_HH 1

#include "G4UserRunAction.hh"

class G4Run;
class G4String;

namespace kcmh
{
  class RunActionMessenger;

  class RunAction: public G4UserRunAction
  {
    private:
      const RunActionMessenger* fRunActionMessenger;
    public:
      RunAction();
      ~RunAction() override;

      void BeginOfRunAction(const G4Run*) override;
      void EndOfRunAction(const G4Run*) override;

      void OpenOutputFile(const G4String&);
  };
} // namespace kcmh


#endif