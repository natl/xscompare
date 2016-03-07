#ifndef PHYSICS_LIST_HH
#define PHYSICS_LIST_HH

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class PhysicsListMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PhysicsList: public G4VModularPhysicsList
{
    public:
          PhysicsList();
          ~PhysicsList();

          void RegisterConstructor(const G4String& name);
          void SetMinEnergyRange(const G4double);
          virtual void ConstructProcess();

          void SaveXS(G4String, G4String, G4double, G4double);

    private:
          G4String fEmName;

          PhysicsListMessenger* fMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif //PHYSICS_LIST_HH
