#ifndef ACTION_INITIALISATION_HH
#define ACTION_INITIALISATION_HH

//parent
#include "G4VUserActionInitialization.hh"

//Project
#include "PrimaryGeneratorAction.hh"

/// Action initialization class.

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    MacroActionInitialization();
    virtual ~MacroActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif // ACTION_INITIALISATION_HH
