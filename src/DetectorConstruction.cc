#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  G4NistManager* nist = G4NistManager::Instance();

  G4Material* water_mat = nist->FindOrBuildMaterial("G4_WATER");
  G4Material* air_mat = nist->FindOrBuildMaterial("G4_AIR");


  G4bool checkOverlaps = true;

  G4Box* solidWorld = new G4Box("World", 1*m, 1*m, 1*m);

  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, air_mat,
    "World");

  G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(),
    logicWorld, "World", 0, false, 0, checkOverlaps);

  G4Box* solidBox = new G4Box("box", 0.1*m, 0.1*m, 0.1*m);

  G4LogicalVolume* logicBox = new G4LogicalVolume(solidBox, water_mat, "box");

  new G4PVPlacement(0, G4ThreeVector(), logicBox, "Box", logicWorld, false, 0,
    checkOverlaps);

  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
