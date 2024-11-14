#ifndef ACTIONINITIALIZATION_HH
#define ACTIONINITIALIZATION_HH 1

#include "G4VUserActionInitialization.hh"

class G4String;

namespace kcmh
{
    class ActionInitialization: public G4VUserActionInitialization
    {
      public:
        ActionInitialization(G4String);
        ~ActionInitialization();

        void Build() const override;
        void BuildForMaster() const override;
      
      private:
        int fSimMode = 0;
    };
} // namespace kcmh
#endif