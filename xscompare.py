"""
xscompare.py

Compare the cross sections generated via the Geant4 application
in this project
"""
from __future__ import division, print_function, unicode_literals

import numpy as np
import matplotlib.pyplot as plt

# G4DATADIR = "/path/to/geant4/shared/data"
# WATER = {"1": 2./18., "16": 16./18.}
# #DAVIS = materials.addChemicals([(WATER, 1.00)])
#
# PARTICLES_IMPL = ["neutron"]
# INVALID_PARTICLE = "Particle must be in " + " ".join(PARTICLES_IMPL)
#
# DATA_PREFIX = {"neutron": [G4DATADIR+"/G4NEUTRONXS1.4/cap",
#                            G4DATADIR+"/G4NEUTRONXS1.4/elast",
#                            G4DATADIR+"/G4NEUTRONXS1.4/inelast"],
#                "e-": [G4DATADIR + "/G4EMLOW6.48/ioni/ion-ss-cs-",
#                       G4DATADIR + "/G4EMLOW6.48/brem/br-cs-"],
#                "photon": [G4DATADIR + "/G4EMLOW6.48/..."]}
#
# DATA_SUFFIX = {"neutron": ".dat",
#                "e-": ".dat",
#                "photon": ".p08"}
#
#
# def getxs(material, particle, min_en, max_en):
#     """
#     """
#     assert particle in PARTICLES_IMPL, INVALID_PARTICLE
#
#     ens = np.arange(min_en, max_en + 1e-4, 1e-4)
#     xs = []
#     for (el, mf) in material.materials:
#         files = [file(prefix + str(el) + DATA_SUFFIX[particle], 'r')
#                  for prefix in DATA_PREFIX[particle]]
#      # TODO
#
#     return None

def comparison_plot(files1, files2, label1, label2, xvals):
    """
    fig = comparison_plot(files1, files2, label1, label2)

    compare two sets of cross section data
    all files in files1 are added, same with files2.
    This gives total cross section
    Then the comparison of the plot sections is plotted
    """
    # condense first set of files
    yvals1 = np.zeros(len(xvals))
    for f in files1:
        xp, yp = np.loadtxt(f, unpack=True)
        yvals1 += np.interp(xvals, xp, yp, left=0, right=0)

    # condense second set of files
    yvals2 = np.zeros(len(xvals))
    for f in files2:
        xp, yp = np.loadtxt(f, unpack=True)
        yvals2 += np.interp(xvals, xp, yp, left=0, right=0)

    diff = 100*(yvals1 - yvals2)/yvals1

    fig = plt.figure()
    ax1 = fig.add_subplot(211)
    ax1.set_ylabel(r"$\sigma_{\mathrm{tot}}$ ($\mathrm{mm}^{-1}$)")
    ax1.set_yscale('log')
    ax1.set_xscale('log')
    ax1.plot(xvals*1000., yvals1, "r-", label=label1)
    ax1.plot(xvals*1000., yvals2, "b--", label=label2)
    plt.legend()
    plt.setp(ax1.get_xticklabels(), visible=False)

    ax2 = fig.add_subplot(212, sharex=ax1)
    ax2.set_ylabel(r"Difference (%)")
    ax2.set_xlabel("Energy (keV)")
    maxabsdiff = max(abs(100*(yvals1 - yvals2)/yvals1))
    ax2.set_ylim([-1.1*maxabsdiff, 1.1*maxabsdiff])
    ax2.plot(xvals*1000., diff, "r-")
    ax2.plot(xvals*1000., np.zeros(len(xvals)), 'b:')
    ax2.set_yticks(ax.get_yticks()[:-1])

    fig.subplots_adjust(hspace=0)

    return fig


def compare(xvals, particle, processes, medium1, medium2, label1, label2, emin,
            emax):
    """
    fig = compare(xvals, particle, processes, medium1, medium2, label1, label2,
                  emin, emax)

    emin and emax are strings
    """
    files1 = ["_".join([particle, medium1, process, emin, emax]) + ".dat" for process in processes]
    files2 = ["_".join([particle, medium2, process, emin, emax]) + ".dat" for process in processes]
    fig = comparison_plot(files1, files2, label1, label2, xvals)
    return fig

if __name__ == "__main__":
    xvals = np.arange(0.0001, 10.000, 0.0001)
    e_processes = ["eBrem", "msc", "eIoni"]
    gamma_processes = ["compt", "phot", "conv", "Rayl"]
    process_dict = {"e-": e_processes, "proton": e_processes, "gamma": gamma_processes}
    label_dict = {"davis": "Davis Broth", "water": "Water", "cellular": "Cell"}

    runs = [{"particle": "e-", "m1": "davis", "m2": "water"},
            {"particle": "e-", "m1": "davis", "m2": "cellular"},
            {"particle": "e-", "m1": "cellular", "m2": "water"},
            {"particle": "gamma", "m1": "davis", "m2": "water"},
            {"particle": "gamma", "m1": "davis", "m2": "cellular"},
            {"particle": "gamma", "m1": "cellular", "m2": "water"},
            {"particle": "proton", "m1": "davis", "m2": "water"},
            {"particle": "proton", "m1": "davis", "m2": "cellular"},
            {"particle": "proton", "m1": "cellular", "m2": "water"}]

    for run in runs:
        p = run["particle"]
        m1 = run["m1"]
        l1 = label_dict[m1]
        m2 = run["m2"]
        l2 = label_dict[m2]
        fig = compare(xvals, p, process_dict[p], m1, m2, l1, l2, "0.0001", "10")
        fig.savefig("-".join([p, m1, m2])+".pdf", bbox_inches="tight")

    #
    # files1 = ["proton_davis_eBrem_0.0001_10.dat",
    #           "proton_davis_msc_0.0001_10.dat",
    #           "proton_davis_eIoni_0.0001_10.dat"]
    # files2 = ["proton_G4_WATER_eBrem_0.0001_10.dat",
    #           "proton_G4_WATER_msc_0.0001_10.dat",
    #           "proton_G4_WATER_eIoni_0.0001_10.dat"]
    # fig_p = comparison_plot(files1, files2, "Davis Medium", "Water", xvals)
    # fig_p.savefig("xs-proton-davis-water.pdf", bbox_inches="tight")
    #
    # files1 = ["gamma_davis_compt_0.0001_10.dat",
    #           "gamma_davis_phot_0.0001_10.dat",
    #           "gamma_davis_conv_0.0001_10.dat",
    #           "gamma_davis_Rayl_0.0001_10.dat"]
    # files2 = ["gamma_G4_WATER_compt_0.0001_10.dat",
    #           "gamma_G4_WATER_phot_0.0001_10.dat",
    #           "gamma_G4_WATER_conv_0.0001_10.dat",
    #           "gamma_G4_WATER_Rayl_0.0001_10.dat"]
    # fig_g = comparison_plot(files1, files2, "Davis Medium", "Water", xvals)
    # fig_g.savefig("xs-gamma-davis-water.pdf", bbox_inches="tight")
    #
    # files1 = ["e-_davis_eBrem_0.0001_10.dat",
    #           "e-_davis_msc_0.0001_10.dat",
    #           "e-_davis_eIoni_0.0001_10.dat"]
    # files2 = ["e-_cellular_eBrem_0.0001_10.dat",
    #           "e-_cellular_msc_0.0001_10.dat",
    #           "e-_cellular_eIoni_0.0001_10.dat"]
    # fig_e = comparison_plot(files1, files2, "Davis Medium", "Cell", xvals)
    # fig_e.savefig("xs-electron-davis-cellular.pdf", bbox_inches="tight")
    #
    # files1 = ["proton_davis_eBrem_0.0001_10.dat",
    #           "proton_davis_msc_0.0001_10.dat",
    #           "proton_davis_eIoni_0.0001_10.dat"]
    # files2 = ["proton_cellular_eBrem_0.0001_10.dat",
    #           "proton_cellular_msc_0.0001_10.dat",
    #           "proton_cellular_eIoni_0.0001_10.dat"]
    # fig_p = comparison_plot(files1, files2, "Davis Medium", "Cell", xvals)
    # fig_p.savefig("xs-proton-davis-cellular.pdf", bbox_inches="tight")
    #
    # files1 = ["gamma_davis_compt_0.0001_10.dat",
    #           "gamma_davis_phot_0.0001_10.dat",
    #           "gamma_davis_conv_0.0001_10.dat",
    #           "gamma_davis_Rayl_0.0001_10.dat"]
    # files2 = ["gamma_cellular_compt_0.0001_10.dat",
    #           "gamma_cellular_phot_0.0001_10.dat",
    #           "gamma_cellular_conv_0.0001_10.dat",
    #           "gamma_cellular_Rayl_0.0001_10.dat"]
    # fig_g = comparison_plot(files1, files2, "Davis Medium", "Cell", xvals)
    # fig_g.savefig("xs-gamma-davis-cellular.pdf", bbox_inches="tight")
    #
    #
    # files1 = ["e-_cellular_eBrem_0.0001_10.dat",
    #           "e-_cellular_msc_0.0001_10.dat",
    #           "e-_cellular_eIoni_0.0001_10.dat"]
    # files2 = ["e-_G4_WATER_eBrem_0.0001_10.dat",
    #           "e-_G4_WATER_msc_0.0001_10.dat",
    #           "e-_G4_WATER_eIoni_0.0001_10.dat"]
    # fig_e = comparison_plot(files1, files2, "Cell", "Water", xvals)
    # fig_e.savefig("xs-electron-cellular-water.pdf", bbox_inches="tight")
    #
    # files1 = ["proton_cellular_eBrem_0.0001_10.dat",
    #           "proton_cellular_msc_0.0001_10.dat",
    #           "proton_cellular_eIoni_0.0001_10.dat"]
    # files2 = ["proton_G4_WATER_eBrem_0.0001_10.dat",
    #           "proton_G4_WATER_msc_0.0001_10.dat",
    #           "proton_G4_WATER_eIoni_0.0001_10.dat"]
    # fig_p = comparison_plot(files1, files2, "Cell", "Water", xvals)
    # fig_p.savefig("xs-proton-cellular-water.pdf", bbox_inches="tight")
    #
    # files1 = ["gamma_cellular_compt_0.0001_10.dat",
    #           "gamma_cellular_phot_0.0001_10.dat",
    #           "gamma_cellular_conv_0.0001_10.dat",
    #           "gamma_cellular_Rayl_0.0001_10.dat"]
    # files2 = ["gamma_G4_WATER_compt_0.0001_10.dat",
    #           "gamma_G4_WATER_phot_0.0001_10.dat",
    #           "gamma_G4_WATER_conv_0.0001_10.dat",
    #           "gamma_G4_WATER_Rayl_0.0001_10.dat"]
    # fig_g = comparison_plot(files1, files2, "Cell", "Water", xvals)
    # fig_g.savefig("xs-gamma-cellular-water.pdf", bbox_inches="tight")
