// If you read this line, the file is originally a copy from cpplib/S8L/include/A2Rt-v03.0.h
#ifndef __PLNCK_H
#define __PLNCK_H
#include "CLHEP_SystemOfUnits.h"
#include "CLHEP_PhysicalConstants.h"
namespace plnck {
//    static const double DBL_PRECISION  = 2.152e-16;//must < 1.e-15; 131119
    static const double DBL_PRECISION  = 2.764e-15;
    //131120 Electron::ele.px(0).py(0).pz(4);ele.complete_p4_("E");
}  // namespace plnck
#endif