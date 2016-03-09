#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "PhysicsList.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif
// #include "G4RunManager.hh"


#include "G4UImanager.hh"
#include "Randomize.hh"

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    G4cout << "Usage is ./xscompare macroname";
    return 1;
  }

  G4Random::setTheEngine(new CLHEP::RanecuEngine);

  #ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
  runManager->SetNumberOfThreads(1);
  #else
  G4RunManager* runManager = new G4RunManager;
  #endif
  // G4RunManager* runManager = new G4RunManager();

  runManager->SetUserInitialization(new DetectorConstruction());
  runManager->SetUserInitialization(new PhysicsList());
  runManager->SetUserInitialization(new ActionInitialization());

  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  G4String command = "/control/execute ";
  G4String fileName = argv[1];
  UImanager->ApplyCommand(command+fileName);

  delete runManager;
}
