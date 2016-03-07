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

    protected:
        // Function to split strings
        std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
            std::stringstream ss(s);
            std::string item;
            while (std::getline(ss, item, delim)) {
                if (!item.empty()) elems.push_back(item);
            }
            return elems;
        }
        std::vector<std::string> split(const std::string &s, char delim) {
            std::vector<std::string> elems;
            split(s, delim, elems);
            return elems;
        }

    private:

        PhysicsList*          fPhysicsList;

        G4UIdirectory*             fPhysDir;
        G4UIcmdWithAString*        fListCmd;
        G4UIcmdWithADoubleAndUnit* fMinEnergyCmd;
        G4UIcmdWithAString*        fMakeFileCmd;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif // PHYSICS_LIST_MESSNGER_HH
