#ifndef EVENTACTION_HH
#define EVENTACTION_HH 1

#include "G4UserEventAction.hh"
#include "globals.hh"

namespace kcmh
{
  class RunAction;

  class EventAction: public G4UserEventAction
  {
  public:
    EventAction(const RunAction*);
    ~EventAction();

    void BeginOfEventAction(const G4Event*) override;
    void EndOfEventAction(const G4Event*) override;

  private:
    G4int dtcTrackerID = -1;
    const RunAction* fRunAction;
  };
  
} // namespace kcmh


#endif