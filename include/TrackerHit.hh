#ifndef TRACKERHIT_HH
#define TRACKERHIT_HH 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

namespace kcmh
{
class TrackerHit : public G4VHit
{
  public:
    TrackerHit();
    TrackerHit(const TrackerHit&) = default;
    ~TrackerHit() override;

    // operators
    TrackerHit& operator=(const TrackerHit&) = default;
    G4bool operator==(const TrackerHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    void SetTrackID  (G4int track)      { fTrackID = track; }
    void SetLayerID  (G4int layerID)      { fLayerID = layerID; }
    void SetEdep     (G4double de)      { fEdep = de; }
    void SetPDGEncoding (G4int PDGEncoding) { fPDGEncoding = PDGEncoding; }
    void SetPixels      (G4int, G4int);
    void SetParentID (G4int parentID) { fParentID = parentID; }
    void SetK (G4double K) { fK = K; };

    // Get methods
    G4int GetTrackID() const     { return fTrackID; };
    G4int GetLayerID() const   { return fLayerID; };
    G4double GetEdep() const     { return fEdep; };
    G4int* GetPixels() { return fPixels; };
    G4int GetPDGEncoding() { return fPDGEncoding; };
    G4int GetParentID() { return fParentID; };
    G4double GetK() { return fK; };

  private:
    G4int         fTrackID = -1;
    G4int         fLayerID = -1;
    G4double      fEdep = 0.;
    G4double      fK = 0.;
    G4int         fPixels[2]{-1, -1};
    G4int         fPDGEncoding = -1;
    G4int         fParentID = -1;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<TrackerHit> TrackerHitsCollection;

extern G4ThreadLocal G4Allocator<TrackerHit>* TrackerHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* TrackerHit::operator new(size_t)
{
  if(!TrackerHitAllocator)
      TrackerHitAllocator = new G4Allocator<TrackerHit>;
  return (void *) TrackerHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void TrackerHit::operator delete(void *hit)
{
  TrackerHitAllocator->FreeSingle((TrackerHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}

#endif
