/*    main for SP8ReadAData-v02.8, A2Rt-v02.9
 ============================================================
 If you read this line, the file is a copy from cpplib/asource/rootData/asc2rt.cpp
 121106
 120605-11
 121204 modified for SP8acc/optics
*/
#include <iostream>
#include <iomanip>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
//#include "CLHEP_SystemOfUnits.h"
//#include "CLHEP_PhysicalConstants.h"
#include "plnck.h"

//using namespace galeo;

int main(int argn,char* argv[]) {
	using namespace std;
	string doprmfilename = "mainSetDoPrm.doprm";//the default doprmfile
	string maincntl="3";
	
	if (1<argn) {
		doprmfilename = *(argv+1);
	}
	if (3==argn) {
		maincntl = *(argv+2);
	}
	
//    Gaus64();
//    std::cout<<"  xInteg[5] = "<<Gaus64::xInteg[5]<<endl;
//    std::cout<<"  wInteg[31] = "<<Gaus64::wInteg[31]<<endl;
  
    using namespace plnck;
    
    double energy = 8.0 *GeV;
    double emass = 0.511 * MeV;
    double gamma = energy / emass;
    
    std::cout.precision(11);
//    std::cout<<"  gamma = "<<gamma<<std::endl;
// PLKLGT
    std::cout<<"  hbarc = "<<hbarc/GeV/fm<<" GeV*fm"<<std::endl;
    //    std::cout<<"  WL2E = "<<WL2E<<std::endl;
    std::cout<<"  hc = "<<hc/eV/nm<<" eV*nm"<<std::endl;
    std::cout<<"  EMASS = "<<EMASS/MeV<<" MeV"<<std::endl;
    std::cout<<"  ECHARG = "<<ECHARG/coulomb<<" C"<<std::endl;
    std::cout<<"  fine_structure_const = "<<fine_structure_const
    <<"  alpha_E = "<<1./fine_structure_const<<std::endl;
//    std::cout<<"  ERDIUS = "<<ERDIUS<<std::endl;
    std::cout<<"  classic_electr_radius = "<<classic_electr_radius/m<<" m"<<std::endl;
    std::cout<<"  barn = "<<barn/m/m<<" m^2"<<std::endl;
    std::cout<<"  THOMSN = "<<THOMSN/barn<<" b"<<std::endl;
    
	return(0);
	
}

