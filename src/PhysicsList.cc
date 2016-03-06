#include "PhysicsList.hh"

// Physics Lists available
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmDNAPhysics.hh"
#include "G4EmDNAPhysics_option1.hh"
//#include "G4EmDNAPhysics_option2.hh"

// Necessary Hadron Physics
#include "G4EmExtraPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"

// Geant 4

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
// #include "G4UserSpecialCuts.hh"

// particles

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4DNAGenericIonsManager.hh"
#include "G4Electron.hh"

// Managers

#include "G4ProcessManager.hh"
#include "G4EmProcessOptions.hh"
#include "G4PhysicsConstructorRegistry.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MacroPhysicsList::MacroPhysicsList() :
    G4VModularPhysicsList(),
    fMessenger(0)
{
    fMessenger = new MacroPhysicsListMessenger(this);
    SetVerboseLevel(1);


    // Synchrotron Radiation and GN Physics
    RegisterPhysics(new G4EmExtraPhysics(verboseLevel));
    // Register the Hadronic Physics
    RegisterPhysics(new G4DecayPhysics(verboseLevel));
    RegisterPhysics(new G4HadronElasticPhysicsHP(verboseLevel));
    RegisterPhysics(new G4HadronPhysicsQGSP_BIC_HP(verboseLevel));
    RegisterPhysics(new G4StoppingPhysics(verboseLevel) );
    RegisterPhysics(new G4IonPhysics(verboseLevel));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MacroPhysicsList::~MacroPhysicsList()
{
    delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MacroPhysicsList::ConstructProcess()
{
    // physics constructors
    //
    if(G4VModularPhysicsList::GetPhysics(0) == 0) return;

    // G4ProcessManager* pman = G4Electron::Electron()->GetProcessManager();
    // pman->AddProcess(new G4UserSpecialCuts(), -1, -1, -1);

    G4VModularPhysicsList::ConstructProcess();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MacroPhysicsList::RegisterConstructor(const G4String& name)
{
    if (verboseLevel > -1)
    {
        G4cout << "MacroPhysicsList::RegisterConstructor: <" << name << ">"
               << G4endl;
    }

    if (name == fEmName) return;

    fEmName = name;
    G4VPhysicsConstructor* physics = G4PhysicsConstructorRegistry::Instance()->
        GetPhysicsConstructor(name);

    RegisterPhysics(physics);

    if(physics)
    {
        physics->ConstructParticle();
    }

    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MacroPhysicsList::SetMinEnergyRange(const G4double minEnergy)
{
    G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(minEnergy,
        1*GeV);
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
}
