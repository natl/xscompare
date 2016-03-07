"""
materials.py

Utilities for dealing with materials
"""
from __future__ import division, print_function, unicode_literals

import unittest
import sys

class Material(object):
    """
    """
    def __init__(self, materials):
        """
        Material(materials)

        create a material from a dict of materials and their mass fractions.
        integers for atomic numbers may be substituted for materials

        e.g. water = Material([(16, 16./18.), (1, 2./18.)])
             salt = Material([(11, 23./58.5), (17, 35.5/58.5.)])
             saltwater = Material([(salt, 0.01), (water, 0.99)])
        """
        mats = []
        for material in materials:
            if type(material[0]) == int:
                mats.append(material)
            elif type(material[0]) == Material:
                for m in material[0].materials:
                    mats.append((m[0], m[1]*material[1]))

        atomic_numbers = []
        mass_fractions = []
        for m in mats:
            if m[0] not in atomic_numbers:
                atomic_numbers.append(m[0])
                mass_fractions.append(m[1])
            else:
                idx = atomic_numbers.index(m[0])
                mass_fractions[idx] += m[1]

        self.materials = zip(atomic_numbers, mass_fractions)

        return None


    def __len__(self):
        return self.materials.__len__()

class TestMaterial(unittest.TestCase):
    def test_material_creation(self):
        raw_water = [(16, 16./18.), (1, 2./18.)]
        water = Material(raw_water)
        self.compare_list_and_material(raw_water, water)

        raw_salt = [(11, 23./58.5), (17, 35.5/58.5)]
        salt = Material(raw_salt)
        self.compare_list_and_material(raw_salt, salt)

        salt_frac = 0.01
        water_frac = 1 - salt_frac
        saltwater = Material([(salt, salt_frac), (water, water_frac)])
        raw_saltwater =  [(16, 16./18.*water_frac), (1, 2./18.*water_frac),
                          (11, 23./58.5*salt_frac), (17, 35.5/58.5*salt_frac)]
        self.compare_list_and_material(raw_saltwater, saltwater)

    def compare_list_and_material(self, the_list, material):
        self.assertEqual(len(the_list), len(material),
                         "Different number of elements")
        [mat_els, mat_masses] = zip(*material.materials)
        for (el, mass) in the_list:
            self.assertIn(el, mat_els,
                "material does not contain element %i" % (el))
            idx = mat_els.index(el)
            self.assertAlmostEqual(mass, mat_masses[idx],
                "material has wrong mass for element %i" %(el))


def run_tests():
    unittest.main(verbosity=2)


if __name__ == '__main__':
    if '--test' in sys.argv:
        sys.argv.remove('--test')
        run_tests()
    else:
        pass
