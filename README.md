# xscompare

Simple tools to compare cross sections of various compounds with plotting

This tool contains two parts, a Geant4 macro that allows simple output
of cross section data, and a python plotting routine.
Geant4 is a simulation toolkit used to study particle transport through
matter, and can be downloaded [here](http://www.geant4.cern.ch/).

**Compiling the macro**

Ensure that you already have a version of Geant4 (preferably >10.2) installed
and that it is in the system path. Then:
```
mkdir /path/to/build/directory
cd /path/to/build/directory
cmake /path/to/this/source
make
```

**Using the macro**

The macro works much like most Geant4 macros, though notably it allows you to
choose the Em Physics list you want, and also to produce output cross sections

_Set the Em Model_
```
/phys/addPhysics G4EmStandardPhysics_option4
```
EM physics lists must first be defined as includes in `src/PhysicsList.cc`

_Record Cross Section Data_
```
/phys/printxs particle material process en_min en_max cut_energy
```
This command saves a data file with the name
`particle_material_process_en_min_en_max.dat`. It contains the cross section in
100eV intervals between `en_min` and `en_max`. Both `en_min` and `en_max` should be in units of MeV. `cut_energy` is in keV.

The `material` parameter can be any NIST material, or any material name defined
in the logic sequence inside the `SaveXS` method of `src/PhysicsList.cc`.
