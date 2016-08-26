// -*- C++ -*-
// $Id: PhysicalConstants.h,v 1.4 2010/06/16 17:12:28 garren Exp $
// ----------------------------------------------------------------------
// HEP coherent Physical Constants
//
// This file has been provided by Geant4 (simulation toolkit for HEP).
//
// The basic units are :
//  		millimeter  
// 		nanosecond  
// 		Mega electron Volt  
// 		positon charge 
// 		degree Kelvin
//              amount of substance (mole)
//              luminous intensity (candela)
// 		radian  
//              steradian 
//
// Below is a non exhaustive list of Physical CONSTANTS,
// computed in the Internal HEP System Of Units.
//
// Most of them are extracted from the Particle Data Book :
//        Phys. Rev. D  volume 50 3-1 (1994) page 1233
// 
//        ...with a meaningful (?) name ...
//
// You can add your own constants.
//
// Author: M.Maire modified by S. D.
//
// History:
//
// 23.02.96 Created
// 26.03.96 Added constants for standard conditions of temperature
//          and pressure; also added Gas threshold.
// 29.04.08   use PDG 2006 values
// 03.11.08   use PDG 2008 values
// 131022 copied from opt/local/include/CLHEP/Units to use as plnck
// 131022 added from Particle Data Booklet 2012
//    PLKLGT WL2E ECHARG EMASS ERDIUS THOMSN
// 131022 updated for Particle Data Booklet 2012
//    Avogadro h_Planck electron_mass_c2

#ifndef CLHEP_PHYSICAL_CONSTANTS_H
#define CLHEP_PHYSICAL_CONSTANTS_H

//#include "CLHEP/Units/defs.h"
//#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP_SystemOfUnits.h"

//namespace CLHEP {
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
//131022 static const double Avogadro = 6.02214179e+23/mole;
static const double Avogadro = 6.02214129e+23/mole;

//
// c   = 299.792458 mm/ns
// c^2 = 898.7404 (mm/ns)^2 
//
static const double c_light   = 2.99792458e+8 * m/s;
static const double c_squared = c_light * c_light;

//
// h     = 4.13566e-12 MeV*ns
// hbar  = 6.58212e-13 MeV*ns
// hbarc = 197.32705e-12 MeV*mm
//
//131022 static const double h_Planck      = 6.62606896e-34 * joule*s;
static const double h_Planck      = 6.62606957e-34 * joule*s;
static const double hbar_Planck   = h_Planck/twopi;
static const double hbarc         = hbar_Planck * c_light;
static const double hc = h_Planck * c_light;
static const double hbarc_squared = hbarc * hbarc;
//Refer to basics/LBCS/lib/cpp/testKlein for the usage
//static const double PLKLGT = .1973269718;  /* hbar*c [GeV fm] */
//static const double PLKLGT = hbarc*(1.e+9)*GeV*fm;  /* hbar*c [GeV fm] */
//static const double PLKLGT = hbarc;  /* hbar*c [GeV fm] */
//static const double WL2E = twopi*PLKLGT*(1.e+3)*eV*nm; /* eV*nm */
//static const double WL2E = twopi*PLKLGT; /* eV*nm */
//
//
//
static const double electron_charge = - eplus; // see SystemOfUnits.h
static const double e_squared = eplus * eplus;
static const double ECHARG = e_SI*coulomb;
//
// amu_c2 - atomic equivalent mass unit
//        - AKA, unified atomic mass unit (u)
// amu    - atomic mass unit
//
//131022 static const double electron_mass_c2 = 0.510998910 * MeV;
static const double electron_mass_c2 = 0.510998928 * MeV;
static const double   proton_mass_c2 = 938.272013 * MeV;
static const double  neutron_mass_c2 = 939.56536 * MeV;
static const double           amu_c2 = 931.494028 * MeV;
static const double              amu = amu_c2/c_squared;
static const double EMASS = 0.510998928 * MeV;
//
// permeability of free space mu0    = 2.01334e-16 Mev*(ns*eplus)^2/mm
// permittivity of free space epsil0 = 5.52636e+10 eplus^2/(MeV*mm)
//
static const double mu0      = 4*pi*1.e-7 * henry/m;
static const double epsilon0 = 1./(c_squared*mu0);

//
// electromagnetic coupling = 1.43996e-12 MeV*mm/(eplus^2)
//
static const double elm_coupling           = e_squared/(4*pi*epsilon0);
//static const double fine_structure_const   = elm_coupling/hbarc; Logical
static const double fine_structure_const   = 0.0072973525698;//PDG2014
static const double classic_electr_radius  = elm_coupling/electron_mass_c2;
static const double electron_Compton_length = hbarc/electron_mass_c2;
static const double Bohr_radius = electron_Compton_length/fine_structure_const;
//static const double ERDIUS = classic_electr_radius * (mm/m); /* in m */
//static const double THOMSN = 4.*twopi/3.* ERDIUS * ERDIUS * (meter2/barn); // b
static const double THOMSN = 4.*twopi/3.*
                    classic_electr_radius * classic_electr_radius;
    
static const double alpha_rcl2 = fine_structure_const
                                   *classic_electr_radius
                                   *classic_electr_radius;

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

}  // namespace plnck
//}  // namespace CLHEP

//#ifdef ENABLE_BACKWARDS_COMPATIBILITY
////  backwards compatibility will be enabled ONLY in CLHEP 1.9
//using namespace CLHEP;
//#endif

#endif /* CLHEP_PHYSICAL_CONSTANTS_H */





