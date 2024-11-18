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
      const G4int fSimMode;
      G4int fLayerID;
      G4double fBeamSigma;
      G4double fBeamSigmaA;
      G4double fBeamSigmaE;

    public:
      RunAction(const G4int);
      ~RunAction() override;

      void BeginOfRunAction(const G4Run*) override;
      void EndOfRunAction(const G4Run*) override;

      void OpenOutputFile(const G4String&);
      void CloseOutFile();

      void SetLabeledBeamSigma(G4double sigma) { fBeamSigma = sigma; };
      void SetLabeledBeamSigmaR(G4double sigmaR) { fBeamSigmaA = sigmaR; };
      void SetLabeledBeamSigmaE(G4double sigmaE) { fBeamSigmaE = sigmaE; };
      void SetLabeledDetLayer(G4int layerID) { fLayerID = layerID; };

      G4double GetLabeledBeamSigma() { return fBeamSigma; };
      G4double GetLabeledBeamSigmaR() { return fBeamSigmaA; };
      G4double GetLabeledBeamSigmaE() { return fBeamSigmaE; };
      G4int GetLabeledDetLayerID() { return fLayerID; };
  };
} // namespace kcmh


#endif