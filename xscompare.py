"""
xscompare.py

Compare the cross sections generated via the Geant4 application
in this project
"""
from __future__ import division, print_function, unicode_literals

import numpy as np
import matplotlib.pyplot as plt
import os
import re
import matplotlib.gridspec as gridspec


from matplotlib import rcParams

rcParams['font.family'] = 'serif'
rcParams['font.serif'] = ['Times']
rcParams['text.usetex'] = True
rcParams['axes.labelsize'] = 8
rcParams['xtick.labelsize'] = 7
rcParams['ytick.labelsize'] = 7
rcParams['legend.fontsize'] = 8


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

    if 0 in yvals1 or 0 in yvals2:
        ok = np.where(yvals1 != 0)[0]
        yvals1 = yvals1[ok]
        yvals2 = yvals2[ok]
        xvals = xvals[ok]


    diff = 100*(yvals1 - yvals2)/yvals1
    gs = gridspec.GridSpec(3,1)

    fig = plt.figure(figsize=[2.5, 3])
    ax1 = fig.add_subplot(gs[:2, :])
    ax1.set_ylabel(r"$\sigma_{\mathrm{tot}}$ ($\mathrm{mm}^{-1}$)")
    ax1.set_yscale('log')
    ax1.set_xscale('log')
    ax1.plot(xvals*1000., yvals1, "y-", label=label1)
    ax1.plot(xvals*1000., yvals2, "r:", label=label2)
    plt.legend()
    plt.setp(ax1.get_xticklabels(), visible=False)

    ax2 = fig.add_subplot(gs[2, :], sharex=ax1)
    ax2.set_ylabel(r"Difference ($\%$)")
    ax2.set_xlabel("Energy (keV)")
    ax2.set_yscale("linear")
    maxabsdiff = max(abs(100*(yvals1 - yvals2)/yvals1))
    ax2.set_ylim([-1.1*maxabsdiff, 1.1*maxabsdiff])
    ax2.plot(xvals*1000., diff, "r-")
    ax2.plot(xvals*1000., np.zeros(len(xvals)), 'k:')
    fig.subplots_adjust(hspace=0)
    fig.show()
    if ax2.get_yticklabels()[-1].get_text() == "":
        ax2.get_yticklabels()[-2].set_visible(False)
    else:
        ax2.get_yticklabels()[-1].set_visible(False)

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


def compare_directories(xvals, dir1, dir2):
    """
    fig = compare(xvals, particle, processes, medium1, medium2, label1, label2,
                  emin, emax)

    emin and emax are strings
    """
    processes = re.compile("eBrem|msc|eIoni|compt|phot|Rayl|conv")
    particles = re.compile("e-|proton|gamma")
    files1 = os.listdir(dir1)
    files2 = os.listdir(dir2)
    files = set.intersection(set(files1), set(files2))
    for fname in files:
        if fname[-4:] == ".dat":
            process = processes.search(fname).group()
            particle = particles.search(fname).group()
            f1 = dir1 + '/' + fname
            f2 = dir2 + '/' + fname
            l1 = " ".join([dir1, particle, process])
            l2 = " ".join([dir2, particle, process])
            figname = "_".join([dir1, dir2, particle, process]) + ".pdf"
            print("Making {}".format(figname))
            fig = comparison_plot([f1], [f2], l1, l2, xvals)
            fig.savefig(figname, bbox_inches="tight")
    files1 = ["_".join([particle, medium1, process, emin, emax]) + ".dat" for process in processes]
    files2 = ["_".join([particle, medium2, process, emin, emax]) + ".dat" for process in processes]
    fig = comparison_plot(files1, files2, label1, label2, xvals)
    return fig

if __name__ == "__main__":
    xvals = np.arange(0.0001, 10.000, 0.0001)
    compare_directories(xvals, "pen", "liv")
    compare_directories(xvals, "opt4", "liv")
    compare_directories(xvals, "opt4", "pen")

    e_processes = ["eBrem", "msc", "eIoni"]
    proton_processes = ["msc"]
    gamma_processes = ["compt", "phot", "conv", "Rayl"]
    process_dict = {"e-": e_processes, "proton": proton_processes, "gamma": gamma_processes}
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
        fig.savefig("_".join(["xs", p, m1, m2])+".pdf", bbox_inches="tight")
