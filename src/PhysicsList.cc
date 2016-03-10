#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"

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
#include "G4UIcommand.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
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
        G4cout << "Building " << material << " material." << G4endl;
        G4Material* water = nist->FindOrBuildMaterial("G4_WATER");
        G4Element* elN  = new G4Element("Nitrogen", "N", 14., 14.007*g/mole);
        G4Element* elH  = new G4Element("Hydrogen", "H", 1., 1.008*g/mole);
        G4Element* elS  = new G4Element("Sulfur", "S", 16., 32.06*g/mole);
        G4Element* elO  = new G4Element("Oxygen", "O", 8., 15.999*g/mole);
        G4Element* elK  = new G4Element("Potassium", "K", 19., 39.098*g/mole);
        G4Element* elP  = new G4Element("Phosphorous", "P", 15., 30.973*g/mole);
        G4Element* elMg  = new G4Element("Magnesium", "Mg", 12., 24.305*g/mole);
        G4Element* elNa  = new G4Element("Sodium", "Na", 11., 22.989*g/mole);
        G4Element* elC  = new G4Element("Carbon", "C", 12., 12.011*g/mole);
        mat = new G4Material("cellular", 1110*kg/m3, 10);
        mat->AddMaterial(water, (100-1.0853)*perCent);
        mat->AddElement (elN, 00.0212*perCent);
        mat->AddElement (elH, 00.0163*perCent);
        mat->AddElement (elS, 00.0269*perCent);
        mat->AddElement (elO, 00.443*perCent);
        mat->AddElement (elK, 00.372*perCent);
        mat->AddElement (elP, 00.170*perCent);
        mat->AddElement (elMg, 00.0020*perCent);
        mat->AddElement (elNa, 00.0117*perCent);
        mat->AddElement (elC, 00.0222*perCent);
    }
    else if (material == "davis")
    {
        G4cout << "Building " << material << " material." << G4endl;
        G4Material* water = nist->FindOrBuildMaterial("G4_WATER");
        G4Element* elN  = new G4Element("Nitrogen", "N", 14., 14.007*g/mole);
        G4Element* elH  = new G4Element("Hydrogen", "H", 1., 1.008*g/mole);
        G4Element* elS  = new G4Element("Sulfur", "S", 16., 32.06*g/mole);
        G4Element* elO  = new G4Element("Oxygen", "O", 8., 15.999*g/mole);
        G4Element* elK  = new G4Element("Potassium", "K", 19., 39.098*g/mole);
        G4Element* elP  = new G4Element("Phosphorous", "P", 15., 30.973*g/mole);
        G4Element* elMg  = new G4Element("Magnesium", "Mg", 12., 24.305*g/mole);
        G4Element* elNa  = new G4Element("Sodium", "Na", 11., 22.989*g/mole);
        G4Element* elC  = new G4Element("Carbon", "C", 12., 12.011*g/mole);
        mat = new G4Material("davis", 1000*kg/m3, 10);
        mat->AddMaterial(water, (100-1.0853)*perCent);
        mat->AddElement (elN, 00.0212*perCent);
        mat->AddElement (elH, 00.0163*perCent);
        mat->AddElement (elS, 00.0269*perCent);
        mat->AddElement (elO, 00.443*perCent);
        mat->AddElement (elK, 00.372*perCent);
        mat->AddElement (elP, 00.170*perCent);
        mat->AddElement (elMg, 00.0020*perCent);
        mat->AddElement (elNa, 00.0117*perCent);
        mat->AddElement (elC, 00.0222*perCent);
    }
    else if (material == "water")
    {
        G4cout << "Building " << material << " material." << G4endl;
        mat = nist->FindOrBuildMaterial("G4_WATER");
    }
    else
    {
        G4cout << "Finding material " << material << " in NIST database"
               << G4endl;
        mat = nist->FindOrBuildMaterial(material);
        if (!mat)
        {
            G4cerr << "Material not found"<< G4endl;
            return;
        }
    }

    G4String filename = G4String(particle + "_" + material + "_" +
        process + "_" +
        G4UIcommand::ConvertToString(en_min) + "_" +
        G4UIcommand::ConvertToString(en_max) + ".dat");

    G4cout << "saving filename: " << filename << G4endl;
    G4double xs;
    G4double en = en_min;

    G4ParticleDefinition* thisParticleDefn =
        G4ParticleTable::GetParticleTable()->FindParticle(particle);

    G4cout << "Saving xs for " << thisParticleDefn->GetParticleName()
           << " in material " << mat->GetName() << ". Process: " << process
           << G4endl;

    std::ofstream file(filename, std::ofstream::out);
    file << "# energy_MeV xsection_mm^-1\n";
    G4EmCalculator* calculator = new G4EmCalculator();
    calculator->SetVerbose(2);
    while (en < en_max)
    {
        xs = calculator->ComputeCrossSectionPerVolume(en, thisParticleDefn,
            process, mat)*mm;
        file << en << " " << xs << "\n";
        en = en + 100*eV;
    }

    file.close();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
