#include "PhysicsListMessenger.hh"

#include "PhysicsList.hh"
#include "G4SystemOfUnits.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsListMessenger::PhysicsListMessenger(PhysicsList* pPhys):
    G4UImessenger(),
    fPhysicsList(pPhys),
    fPhysDir(0),
    fListCmd(0),
    fMinEnergyCmd(0)
{
  fPhysDir = new G4UIdirectory("/phys/");
  fPhysDir->SetGuidance("physics list commands");

  fListCmd = new G4UIcmdWithAString("/phys/addPhysics",this);
  fListCmd->SetGuidance("Add modular physics list.");
  fListCmd->SetParameterName("PList",false);
  fListCmd->AvailableForStates(G4State_PreInit);
  fListCmd->SetToBeBroadcasted(false);

  fMinEnergyCmd = new G4UIcmdWithADoubleAndUnit("/phys/minEnergy",this);
  fMinEnergyCmd->SetGuidance("Set the minimum energy production cut");
  fMinEnergyCmd->SetParameterName("energy",250*eV);
  fMinEnergyCmd->AvailableForStates(G4State_PreInit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsListMessenger::~PhysicsListMessenger()
{
  delete fListCmd;
  delete fMinEnergyCmd;
  delete fPhysDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsListMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if( command == fListCmd )
        fPhysicsList->RegisterConstructor(newValue);
    else if (command == fMinEnergyCmd)
        fPhysicsList->SetMinEnergyRange(fMinEnergyCmd->GetNewDoubleValue(newValue));

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
