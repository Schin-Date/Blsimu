#include <iostream>
#include <iomanip>
#include <cstdlib> //exit()
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <ctime>
//#include "witgn.h"
#include "witpy.h"
//#include "plnck.h"
#include "Blsimu.h"
//#include "EleTracer.h"
//#include "main.h"



/*                 main for Blsimu
 =========================================================

 
 
 
 */
int main(int argn,char* argv[]) {
    using namespace std;

    //    string doprmfilename = "local.doprm";//the default doprmfile
    std::string  thisDir = getenv("blsimu_SRCDIR");//get an environment variable
    std::string doprmfilename = thisDir+"/main.doprm";//default doprm file for main

    if (1<argn) {
        std::string buff = *(argv+1);
        if (6<buff.length()) {
            doprmfilename = buff;
        }
    }
    
    
    std::cout<<"D...main:: about to call Blsimu..."<<std::endl;
    
    
    Blsimu blsimu(doprmfilename); //<< this does cause an instantiation
    //--------------------
    
    
    
}/* end of main*/
