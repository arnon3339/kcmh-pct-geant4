#ifndef ACTIONINITIALIZATION_HH
#define ACTIONINITIALIZATION_HH 1
#include "G4VUserActionInitialization.hh"

namespace kcmh
{
    class ActionInitialization: public G4VUserActionInitialization
    {
      public:
        ActionInitialization(){};
        ~ActionInitialization() override{};

        void Build() const override;
        void BuildForMaster() const override;
    };
} // namespace kcmh
#endif