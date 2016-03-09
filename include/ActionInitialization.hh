#ifndef ACTION_INITIALIZATION_HH
#define ACTION_INITIALIZATION_HH

//parent
#include "G4VUserActionInitialization.hh"

//Project
#include "PrimaryGeneratorAction.hh"

/// Action initialization class.

class ActionInitialization : public G4VUserActionInitialization
{
    public:
        ActionInitialization();
        virtual ~ActionInitialization();

        virtual void BuildForMaster() const;
        virtual void Build() const;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif // ACTION_INITIALIZATION_HH
