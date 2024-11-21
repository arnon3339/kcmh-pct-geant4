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
      G4int fLynxLayerID;
      G4double fLynxBeamSigma;
      G4double fLynxBeamSigmaA;
      G4double fLynxBeamSigmaE;

      G4double fPctBeamPosX;
      G4double fPctPhAngle;

    public:
      RunAction(const G4int);
      ~RunAction() override;

      void BeginOfRunAction(const G4Run*) override;
      void EndOfRunAction(const G4Run*) override;

      void OpenOutputFile(const G4String&);
      void CloseOutFile();

      void SetLynxLabeledBeamSigma(G4double sigma) { fLynxBeamSigma = sigma; };
      void SetLynxLabeledBeamSigmaR(G4double sigmaR) { fLynxBeamSigmaA = sigmaR; };
      void SetLynxLabeledBeamSigmaE(G4double sigmaE) { fLynxBeamSigmaE = sigmaE; };
      void SetLynxLabeledDetLayer(G4int layerID) { fLynxLayerID = layerID; };

      void SetPctLabeledBeamPosX(G4double posX) { fPctBeamPosX = posX; };
      void SetPctLabeledPhAngle(G4double angle) { fPctPhAngle = angle; };

      G4double GetLynxLabeledBeamSigma() { return fLynxBeamSigma; };
      G4double GetLynxLabeledBeamSigmaR() { return fLynxBeamSigmaA; };
      G4double GetLynxLabeledBeamSigmaE() { return fLynxBeamSigmaE; };
      G4int GetLynxLabeledDetLayerID() { return fLynxLayerID; };
      G4double GetPctLabeledBeamPosX() { return fPctBeamPosX; };
      G4double GetPctLabeledPhAngle() { return fPctPhAngle; };
  };
} // namespace kcmh


#endif