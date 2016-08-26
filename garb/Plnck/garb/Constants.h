// -*- C++ -*-
// $Id: Constants.h,v 1.0 2013/10/21 19:34 S.D. $
// ----------------------------------------------------------------------
// Refered::
// HEP coherent Physical Constants in opt/local/include/CLHEP/Units/
//
// This file has been originally provided by Geant4 (simulation toolkit for HEP).
//
// You can add your own constants.
//
// Author: M.Maire
//
// History:
//
#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace plnck {

//
//
//
static const double     pi  = 3.14159265358979323846;
static const double  twopi  = 2*pi;
static const double halfpi  = pi/2;
static const double     pi2 = pi*pi;

//
// 
//
static const double Avogadro = 6.02214179e+23// /mole;

//
// c   = 299.792458 mm/ns
// c^2 = 898.7404 (mm/ns)^2 
//
static const double c_light   = 2.99792458e+8 //* m/s;

    static const double hbarc  = .1973269602  /* [GeV fm] = eV um */;
    static const double hbar_Planck   = hbarc / c_light;
    static const double h_Planck   = hbar_Planck * twopi;/* 10^-6 eV s */
    static const double hc_Planck = hbarc * twopi * 1000 /*= 1240 ev nm */
// h     = 4.13566e-12 MeV*ns
// hbar  = 6.58212e-13 MeV*ns
// hbarc = 197.32705e-12 MeV*mm
//
//static const double h_Planck      = 6.62606896e-34 * joule*s;
//static const double hbar_Planck   = h_Planck/twopi;
//static const double hbarc         = hbar_Planck * c_light;
//static const double hbarc_squared = hbarc * hbarc;

//
//
//
static const double echarge = 1.602176462E-19 /* electron charge [C] */
//static const double electron_charge = - eplus; // see SystemOfUnits.h
//static const double e_squared = eplus * eplus;

//
// amu_c2 - atomic equivalent mass unit
//        - AKA, unified atomic mass unit (u)
// amu    - atomic mass unit
//
static const double emass = .510998928 /* m_electron [MeV/c/c] */
//static const double electron_mass_c2 = 0.510998910 * MeV;
static const double   proton_mass_c2 = 938.272013 //* MeV;
static const double  neutron_mass_c2 = 939.56536 //* MeV;
static const double           amu_c2 = 931.494028 ///* MeV;
//static const double              amu = amu_c2/c_squared;

//
// permeability of free space mu0    = 2.01334e-16 Mev*(ns*eplus)^2/mm
// permittivity of free space epsil0 = 5.52636e+10 eplus^2/(MeV*mm)
//
//static const double mu0      = 4*pi*1.e-7 * henry/m;
//static const double epsilon0 = 1./(c_squared*mu0);

//
// electromagnetic coupling = 1.43996e-12 MeV*mm/(eplus^2)
//
//static const double elm_coupling           = e_squared/(4*pi*epsilon0);
//static const double fine_structure_const   = elm_coupling/hbarc;
//static const double classic_electr_radius  = elm_coupling/electron_mass_c2;
//static const double electron_Compton_length = hbarc/electron_mass_c2;
//static const double Bohr_radius = electron_Compton_length/fine_structure_const;

//static const double alpha_rcl2 = fine_structure_const
//                                   *classic_electr_radius
//                                   *classic_electr_radius;

static const double twopi_mc2_rcl2 = twopi*electron_mass_c2
                                             *classic_electr_radius
                                             *classic_electr_radius;
//
//
//
static const double k_Boltzmann = 8.617343e-11 * MeV/kelvin;

//
//
//
static const double STP_Temperature = 273.15*kelvin;
static const double STP_Pressure    = 1.*atmosphere;
static const double kGasThreshold   = 10.*mg/cm3;

//
//
//
static const double universe_mean_density = 1.e-25*g/cm3;

}  // namespace CLHEP

#ifdef ENABLE_BACKWARDS_COMPATIBILITY
//  backwards compatibility will be enabled ONLY in CLHEP 1.9
using namespace CLHEP;
#endif

#endif /* HEP_PHYSICAL_CONSTANTS_H */





