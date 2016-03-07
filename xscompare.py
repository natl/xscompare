"""
xscompare.py

Simple tool for taking two chemicals and comparing their cross sections
"""
from __future__ import division, print_function, unicode_literals

import numpy as np
import matplotlib.pyplot as plt
import materials


G4DATADIR = "/path/to/geant4/shared/data"
WATER = {"1": 2./18., "16": 16./18.}
DAVIS = materials.addChemicals([(WATER, 1.00)])

PARTICLES_IMPL = ["proton", "e-", "neutron", "photon"]
INVALID_PARTICLE = "Particle must be in " + " ".join(PARTICLES_IMPL)

DATA_PREFIX = {"neutron": [G4DATADIR+"/G4NEUTRONXS1.4/cap",
                           G4DATADIR+"/G4NEUTRONXS1.4/elast",
                           G4DATADIR+"/G4NEUTRONXS1.4/inelast"],
               "e-": [G4DATADIR + "/G4EMLOW6.48/ioni/ion-ss-cs-",
                      G4DATADIR + "/G4EMLOW6.48/brem/br-cs-"],
               "photon": [G4DATADIR + "/G4EMLOW6.48/..."]}

DATA_SUFFIX = {"neutron": ".dat",
               "e-": ".dat",
               "photon": ".p08"}


def getxs(material, particle, min_en, max_en):
    """
    """
    assert particle in PARTICLES_IMPL, INVALID_PARTICLE

    return None
