#ifndef RUNACTION_HH
#define RUNACTION_HH 1

#include "G4UserRunAction.hh"
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#include <memory>

using namespace boost::accumulators;

class G4Run;
class G4String;

namespace kcmh
{
  class RunActionMessenger;

  class RunAction: public G4UserRunAction
  {
    private:
      const RunActionMessenger* fRunActionMessenger;
      const G4int fSimMode;
      G4double beamSigma;
      G4double beamSigmaR;

      std::unique_ptr<boost::accumulators::accumulator_set<double,
      boost::accumulators::features<boost::accumulators::tag::mean,
      boost::accumulators::tag::variance>>> fAccX;

      std::unique_ptr<boost::accumulators::accumulator_set<double,
      boost::accumulators::features<boost::accumulators::tag::mean,
      boost::accumulators::tag::variance>>> fAccY;

    public:
      RunAction(const G4int);
      ~RunAction() override;

      void BeginOfRunAction(const G4Run*) override;
      void EndOfRunAction(const G4Run*) override;

      void OpenOutputFile(const G4String&);

      void ResetLynxAcc();
      void InitLynxAcc();

      void AddAccValues(G4int*);

      void SetLabeledBeamSigma(G4double sigma) { beamSigma = sigma; };
      void SetLabeledBeamSigmaR(G4double sigmaR) { beamSigmaR = sigmaR; };

      G4double GetLabeledBeamSigma() { return beamSigma; };
      G4double GetLabeledBeamSigmaR() { return beamSigmaR; };
  };
} // namespace kcmh


#endif