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

// SaveXS
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4EmCalculator.hh"
#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList() :
    G4VModularPhysicsList(),
    fMessenger(0)
{
    fMessenger = new PhysicsListMessenger(this);
    SetVerboseLevel(1);


    // // Synchrotron Radiation and GN Physics
    // RegisterPhysics(new G4EmExtraPhysics(verboseLevel));
    // // Register the Hadronic Physics
    // RegisterPhysics(new G4DecayPhysics(verboseLevel));
    // RegisterPhysics(new G4HadronElasticPhysicsHP(verboseLevel));
    // RegisterPhysics(new G4HadronPhysicsQGSP_BIC_HP(verboseLevel));
    // RegisterPhysics(new G4StoppingPhysics(verboseLevel) );
    // RegisterPhysics(new G4IonPhysics(verboseLevel));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList()
{
    delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructProcess()
{
    // physics constructors
    //
    if(G4VModularPhysicsList::GetPhysics(0) == 0) return;

    // G4ProcessManager* pman = G4Electron::Electron()->GetProcessManager();
    // pman->AddProcess(new G4UserSpecialCuts(), -1, -1, -1);

    G4VModularPhysicsList::ConstructProcess();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::RegisterConstructor(const G4String& name)
{
    if (verboseLevel > -1)
    {
        G4cout << "PhysicsList::RegisterConstructor: <" << name << ">"
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

void PhysicsList::SetMinEnergyRange(const G4double minEnergy)
{
    G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(minEnergy,
        1*GeV);
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SaveXS(G4String particle, G4String material, G4String process,
    G4double en_min, G4double en_max)
{
    G4NistManager* nist = G4NistManager::Instance();
    G4Material* mat;
    //Make custom materials here
    if (material == "cellular")
    {
        G4Element* elK  = new G4Element("Potassium", "K", 19., 39.1*g/mole);
        mat = new G4Material("custom", 1000*kg/m3, 2);
        mat->AddMaterial(fpWaterMaterial, 99.63*perCent);
        mat->AddElement (elK, 00.37*perCent);
    }
    else
    {
        mat = nist->FindOrBuildMaterial(material);
    }
    G4String filename = G4String(particle + "_" + material + "_" +
        G4UICommand::ConvertToString(process) + "_" +
        G4UICommand::ConvertToString(en_min) + "_" +
        G4UICommand::ConvertToString(en_max) + ".dat")

    G4double xs;
    G4double en = en_min;

    ofstream file;
    file << "# energy_MeV xsection\n";
    while (en < en_max)
    {
        xs = G4EmCalculator::ComputeCrossSectionPerVolume(en, particle,
            process, mat);
        file << en << " " << xs << "\n";
        en = en + 100*eV;
    }

    file.close();



    delete nist;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
