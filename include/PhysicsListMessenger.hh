#ifndef PHYSICS_LIST_MESSNGER_HH
#define PHYSICS_LIST_MESSNGER_HH

#include "globals.hh"
#include "G4UImessenger.hh"

class PhysicsList;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PhysicsListMessenger: public G4UImessenger
{
  public:

    PhysicsListMessenger(PhysicsList* );
   ~PhysicsListMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

  private:

    PhysicsList*          fPhysicsList;

    G4UIdirectory*             fPhysDir;
    G4UIcmdWithAString*        fListCmd;
    G4UIcmdWithADoubleAndUnit* fMinEnergyCmd;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif // PHYSICS_LIST_MESSNGER_HH
