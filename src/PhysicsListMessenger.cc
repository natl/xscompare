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
    fMinEnergyCmd(0),
    fMakeFileCmd(0)
{
    fPhysDir = new G4UIdirectory("/phys/");
    fPhysDir->SetGuidance("physics list commands");

    fListCmd = new G4UIcmdWithAString("/phys/addPhysics", this);
    fListCmd->SetGuidance("Add modular physics list.");
    fListCmd->SetParameterName("PList",false);
    fListCmd->AvailableForStates(G4State_PreInit);
    fListCmd->SetToBeBroadcasted(false);

    fMinEnergyCmd = new G4UIcmdWithADoubleAndUnit("/phys/minEnergy", this);
    fMinEnergyCmd->SetGuidance("Set the minimum energy production cut");
    fMinEnergyCmd->SetParameterName("energy",250*eV);
    fMinEnergyCmd->AvailableForStates(G4State_PreInit);

    fMakeFileCmd = new G4UIcmdWithAString("/phys/printxs", this);
    fMakeFileCmd->SetGuidance("Make a file containing cross sections");
    fMakeFileCmd->SetGuidance(
        "/phys/printxs particle material process en_min_MeV en_max_MeV");
    fMakeFileCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsListMessenger::~PhysicsListMessenger()
{
    delete fListCmd;
    delete fMinEnergyCmd;
    delete fMakeFileCmd;
    delete fPhysDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsListMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if( command == fListCmd )
        fPhysicsList->RegisterConstructor(newValue);
    else if (command == fMinEnergyCmd)
        fPhysicsList->SetMinEnergyRange(
            fMinEnergyCmd->GetNewDoubleValue(newValue));
    else if (command == fMakeFileCmd)
    {
        std::vector<std::string> strvec = split(newValue, ' ');
        G4String particle = strvec[0];
        G4String material = strvec[1];
        G4String process = strvec[2];
        G4double en_min = std::stod(strvec[3])*MeV;
        G4double en_max = std::stod(strvec[4])*MeV;
        fPhysicsList->SaveXS(particle, material, process, en_min, en_max);
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
