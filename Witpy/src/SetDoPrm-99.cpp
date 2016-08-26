// If you read this line, the file is a copy from cpplib/S8L/SetDoPrm-v01.1.cpp
// If you read this line, the file is a copy from cpplib/asource/RootDraw_byC 
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip> // USES cout << setw(9), etc.
#include <Python.h> //See Makefile for compiler and linker options
#include "SetDoPrm.h"
//#include "StrTable-v03.1w.h"
//
//#define WITPY_SRCDIR "/Users/schin/Data/notes/cpplib/Witpy/srcPy"

namespace witpy {
//using namespace std;
//template int &SetDoPrm::getValue(const std::string& vname);
//template double &SetDoPrm::getValue(const std::string& vname);
template int SetDoPrm::getValue(const std::string& vname) const;
template short SetDoPrm::getValue(const std::string& vname) const;
template float SetDoPrm::getValue(const std::string& vname) const;
template double SetDoPrm::getValue(const std::string& vname) const;
template unsigned int SetDoPrm::getValue(const std::string& vname) const;
template long SetDoPrm::getValue(const std::string& vname) const;
template long long SetDoPrm::getValue(const std::string& vname) const;
template void SetDoPrm::setValue(const std::string& vname,
                                 const int &value);
template void SetDoPrm::setValue(const std::string& vname,
                                     const float &value);
template void SetDoPrm::setValue(const std::string& vname,
                                     const double &value);
template void SetDoPrm::setValue(const std::string& vname,
                                     const bool &value);
template void SetDoPrm::setValue(const std::string& vname,
                                     const unsigned int &value);
template void SetDoPrm::setValue(const std::string& vname,
                                     const long &value);
template void SetDoPrm::setValue(const std::string& vname,
                                     const long long &value);

//class SetDoPrm;
SetDoPrm::SetDoPrm() {
	if (5<s_PrtLvl) std::cout << "D6"
		<<"...SetDoPrm(): instantiated."<<std::endl;
} //Default constructor
SetDoPrm::SetDoPrm(const std::string doprmfilename)
{
    if (5<s_PrtLvl) std::cout << "D6...SetDoPrm::ReadDoPrm(\""<<doprmfilename
        <<"\"): instantiated."<<std::endl;

    ReadDoPrm(doprmfilename, "NOLOG");
}
const int SetDoPrm::ReadDoPrm(const std::string doprmfilename, const std::string logFile_Name)
{
    std::string logfilename = logFile_Name;
    if (""==logfilename) logfilename = "NOLOG";

    if (5<s_PrtLvl) std::cout << "D6...SetDoPrm::ReadDoPrm(\""<<doprmfilename
        <<"\", \""<<logfilename<<"\"): instantiated."<<std::endl;
    
    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs,
    *pClass, *pInstance, *pValue, *pValue0;
    
    // Initialize the Python Interpreter
    Py_Initialize();

    //1242
    //wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    //Py_SetProgramName(program);
    
    // tell Python a directory to search for modules
    PyRun_SimpleString("import sys");
//    PyRun_SimpleString("sys.path.append('../srcPy')");
    if (6<s_PrtLvl) std::cout<<"D7...WITPY_SRCDIR = "<<WITPY_SRCDIR<<std::endl;
    std::string witpy_path_s ="sys.path.append('"WITPY_SRCDIR"')";
//    std::cout<<"DDD...witpy_path_s = "<<witpy_path_s<<std::endl;
    PyRun_SimpleString(witpy_path_s.c_str());
    
    // Build the name object (module name)
    if (6<s_PrtLvl) std::cout<<"D7...main::about to call PyUnicode_DecodeFSDefault..."<<std::endl;
    pName = PyUnicode_DecodeFSDefault("readDoPrm");
    
    // Load the module object
    if (6<s_PrtLvl) std::cout<<"D7...main::about to call PyImport_Import..."<<std::endl;
    pModule = PyImport_Import(pName);
    
    //pDict is a borrowed reference
    if (6<s_PrtLvl) std::cout<<"D7...main::about to call PyModule_GetDict..."<<std::endl;
    pDict = PyModule_GetDict(pModule);
    //................................................................................
    //................................................................................
    //---------------- Passing printLvl to python module ---------------------
    //pFunc is also a borrowed reference
    pFunc = PyDict_GetItemString(pDict, "set_printLvl");
    if (PyCallable_Check(pFunc)) {
        int arg1 = s_PrtLvl;
        //NG pArgs = PyTuple_Pack(1,PyInt_FromLong(arg1)); this is for python2
        pArgs = PyTuple_Pack(1,PyLong_FromLong(arg1));
        
        if (6<s_PrtLvl) std::cout<<"D7...main::about to call PyObject_CallObject..."<<std::endl;
        pValue0 = PyObject_CallObject(pFunc, pArgs);
        
        if (NULL != pArgs) {
            Py_DECREF(pArgs);
        }
        
        if (NULL != pValue0) {
        //NG if (6<s_PrtLvl) printf("      Return of call : %ld\n",PyInt_AsLong(pValue)); python2
            if (6<s_PrtLvl) printf("     Return of call : %ld\n",PyLong_AsLong(pValue0));
                Py_DECREF(pValue0);
        }
        else {
                PyErr_Print();
                std::cout<<"    pValue0 is a NULL"<<std::endl;
        }
        
    }
    else {
        PyErr_Print();
    }
    
    //................................................................................
    //---------------- Call python method readDoPrm.txt2dataLst ---------------------
    // Build the name of a callable class
    if (6<s_PrtLvl) std::cout<<"D7...main::about to call PyDict_GetItemString..."<<std::endl;
    pClass = PyDict_GetItemString(pDict, "ReadDoPrm");
    
    pInstance = NULL;
    //F if (PyCallable_Check(pFunc)) {
    if (PyCallable_Check(pClass)) {
        //F PyObject_CallObject(pFunc, NULL);
    if (6<s_PrtLvl) std::cout<<"D7...main::about to call PyObject_CallObject..."<<std::endl;
        pInstance = PyObject_CallObject(pClass, NULL);
    }
    else {
        PyErr_Print();
    }
    //preparing char* to call PyObject_CallMethod
    //std::string pyMethName_s = "txt2sdataPair";
    std::string pyMethName_s = "txt2dataLst";
    char *pyMethName = new char (pyMethName_s.length()+1);
    std::strcpy(pyMethName,pyMethName_s.c_str());
    //
    std::string pyMethArgTypes_s = "(ss)";
    char *pyMethArgTypes = new char (pyMethArgTypes_s.length()+1);
    std::strcpy(pyMethArgTypes,pyMethArgTypes_s.c_str());
    //
    if (6<s_PrtLvl) std::cout<<"D7...main::about to call PyObject_CallMethod..."<<std::endl;
    pValue = PyObject_CallMethod(pInstance,pyMethName,pyMethArgTypes,
                                 doprmfilename.c_str(),logfilename.c_str());
                                //"indata/blsimu.doprm","log.readDoPrm");

    delete pyMethName;
    delete pyMethArgTypes;
    
    //obtain values in C++
    //pValue is supposed to be a list of lists
    if(!PyList_Check(pValue)) {
        std::cout<<"E...pValue is not a list"<<std::endl;
    }
    
    //size of pValue as a list
    int lsize = PyList_Size(pValue);
    if (2<s_PrtLvl) std::cout<<"N3...pValue: List size = "<<lsize<<std::endl;
    
    
    PyObject *pElem;
    m_map_prm.clear();
    m_nmaps = 0;
    PyObject *pVal0, *pVal1, *pVal2;

    for (int i =0; i<lsize; i++) {
        pElem = PySequence_GetItem(pValue,i);
        if(!PyList_Check(pElem)) {
            std::cout<<"E...pElem is not a list"<<std::endl;
        }
        //PyObject *pVal0, *pVal1, *pVal2;
        pVal0 = PySequence_GetItem(pElem,0);
        pVal1 = PySequence_GetItem(pElem,1);
        pVal2 = PySequence_GetItem(pElem,2);
        //        if (!PyBytes_Check(pVal1)) {
        //            std::cout<<"E...pVal1 is not a bytes"<<std::endl;
        if (!PyUnicode_Check(pVal0)) {
            std::cout<<"E...pVal0 is not a unicode"<<std::endl;
        }
        char *vartype = PyUnicode_AsUTF8(pVal0);
        //        std::cout<<" "<<varname<<std::endl;
        
        if (!PyUnicode_Check(pVal1)) {
            std::cout<<"E...pVal1 is not a unicode"<<std::endl;
        }
        char *varname = PyUnicode_AsUTF8(pVal1);
        //        std::cout<<i<<" "<<vartype<<" "<<varname<<std::endl;
        
        std::string vname_s(varname);
        std::string vtype_s(vartype);

        if (!PyUnicode_Check(pVal2)) {
            std::cout<<"E...pVal2 is not a unicode"<<std::endl;
        }
        char *val = PyUnicode_AsUTF8(pVal2);
        
        std::string val_s(val);
        
#if(0)//**************************************************************************
        // Depending on readDoPrm versions, pVal2 in pElem may be either
        // a specified C type or always a string.
        // The following part corresponds to both cases.
        if ("int" == vtype_s) {
            if (!PyLong_Check(pVal2)) {
                std::cout<<"E..."<<i<<" pVal2 is not a long"<<std::endl;
                char *val_c = PyUnicode_AsUTF8(pVal2);
                std::string val(val_c);
                std::cout<<i<<" "<<vtype_s<<" "<<vname_s<<" "<<val<<std::endl;
            }
            else {
                int val(PyLong_AsLong(pVal2));
                std::cout<<i<<" "<<vtype_s<<" "<<vname_s<<" "<<val<<std::endl;
            }
        }
        else if ("double" == vtype_s) {
            if (!PyFloat_Check(pVal2)) {
                std::cout<<"E..."<<i<<" pVal2 is not a float"<<std::endl;
                char *val_c = PyUnicode_AsUTF8(pVal2);
                std::string val(val_c);
                std::cout<<i<<" "<<vtype_s<<" "<<vname_s<<" "<<val<<std::endl;
            }
            else {
                double val(PyFloat_AsDouble(pVal2));
                std::cout<<i<<" "<<vtype_s<<" "<<vname_s<<" "<<val<<std::endl;
            }
        }
        else if ("str" == vtype_s) {
            char *val_c = PyUnicode_AsUTF8(pVal2);
            std::string val(val_c);
            std::cout<<i<<" "<<vtype_s<<" "<<vname_s<<" \""<<val<<"\""<<std::endl;
        }
        else if ("bool" == vtype_s) {
            if (!PyBool_Check(pVal2)) {
                std::cout<<"E..."<<i<<" pVal2 is not a bool"<<std::endl;
                char *val_c = PyUnicode_AsUTF8(pVal2);
                std::string val(val_c);
                std::cout<<i<<" "<<vtype_s<<" "<<vname_s<<" "<<val<<std::endl;
            }
            else {
                bool val = true;
                if (Py_False == pVal2) {
                    val = false;
                }
                    std::cout<<i<<" "<<vtype_s<<" "<<vname_s<<" "<<val<<std::endl;
                }
        }
        else {
            std::cout<<"E...type "<<vtype_s<<" not prepared."<<std::endl;
            exit(9);
        }
        //        char * varname = PyUnicode_AsString(pVal1);
        //        std::string vname_s = varname;
        //        std::string vname_s(varname);
        //        std::string varname = PyBytes_AsString(pVal1);
        //        std::string varname = strdup(pVal1);
        //        std::cout<<i<<" "<<vname_s<<std::endl;

#endif//**************************************************************************
        
        
        //assign map
        m_nmaps++;
        //		m_map_prm.insert(make_pair(variable_name,variable_value));
        //m_map_prm.push_back(make_pair(variable_name,variable_value));
        m_map_prm.push_back(make_pair(vname_s,val_s));

        
        
    }//i<lsize
    
    Py_XDECREF(pVal0);
    Py_XDECREF(pVal1);
    Py_XDECREF(pVal2);

    Py_XDECREF(pElem);
    
    
    //pDoPrm_lst = pValue;
    
    //Clean up
    Py_XDECREF(pValue);
    if (NULL != pInstance)    Py_XDECREF(pInstance);
    Py_XDECREF(pClass);
    //Py_DECREF(pDict); << DECREF borrowed ref may cause segfault
    Py_XDECREF(pModule);
    Py_XDECREF(pName);

    
    
    // Finish the Python Interpreter
    Py_Finalize();
    
    //PyMem_RawFree(program);
    
    
    return 0;
    
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
SetDoPrm::~SetDoPrm() {
	if (5<s_PrtLvl) std::cout << "D6"
		<<"...SetDoPrm::Destructor:"<<std::endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//void SetDoPrm::setValue(const std::string &vtype, const std::string &vname,
template <typename T>
void SetDoPrm::setValue(const std::string &vname, const T &value){
	if (7<s_PrtLvl) std::cout << "D8"
		<<"...SetDoPrm::setValue("
        <<vname<<", "<<value<<")"
        <<std::endl;
    
    std::stringstream ss;
    ss.clear();
    ss.str("");
    ss<<value;
    std::string value_s=ss.str();
    
    int i_map=0;
    for (int i = 0; i<m_nmaps; i++) {
        if (vname!=m_map_prm[i].first) continue;
        m_map_prm[i].second = value_s;
        i_map++;
    }//i_map<m_nmaps
    
    if (0<i_map) return;
    
    m_map_prm.push_back(make_pair(vname,value_s));
    m_nmaps++;

    return;
}//template <typename T> setValue
template<> void SetDoPrm::setValue(const std::string &vname,
                                   const std::string &value){
        
	if (7<s_PrtLvl) std::cout << "D8"
		<<"...SetDoPrm::setValue("
        <<vname<<", "<<value<<")"
        <<std::endl;

    int i_found=0;
    int i_pos=0;
    for (int i = 0; i<m_nmaps; i++) {
        if (vname!=m_map_prm[i].first) continue;
        i_pos = i;
        i_found++;
    }//i_map<m_nmaps

    if (0==i_found) {
        m_map_prm.push_back(make_pair(vname,value));
        m_nmaps++;
    }
    else {
        m_map_prm[i_pos].second = value;
    }
    
    return;
}//setValue<std::string>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
const bool SetDoPrm::is_a_key(const std::string& vname) const {

	if (7<s_PrtLvl) std::cout << "D8"
		<<"...SetDoPrm::is_a_key: called."<<std::endl;
	
//	std::vector<std::pair<std::string,std::string>>::iterator itr;
//	for (itr=m_map_prm.begin(); m_map_prm.end()!=itr; itr++) {
//		if (itr.first==vname) return true;
//	}
	
	for (int i=0; i<m_nmaps; i++) {
		if(m_map_prm[i].first==vname) return true;
	}
	
	return false;
	
//	if (m_map_prm.end() == m_map_prm.find(vname)) {
//		return false;
//	}
//	
//	return true;

}	
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//template <typename T> T &SetDoPrm::getValue(const std::string& vname){
template <typename T> T SetDoPrm::getValue(const std::string& vname) const{

	if (9<s_PrtLvl) std::cout << "D10"
		<<"...SetDoPrm::getValue: called."<<std::endl;
    
	int ipos = 0;
    //for (ipos=0; ipos<m_nmaps; ipos++) {
    for (ipos=m_nmaps-1; ipos>-1; ipos--) {
		if(m_map_prm[ipos].first==vname) break;
        //if (m_nmaps-1==ipos) {
        if (0==ipos) {
			std::cout<<"E...SetDoPrm::getValue: vname "<<vname
			<<" not found in m_map_prm."<<std::endl;
			exit(1);
		}
	}
	
//	if (m_map_prm.end() == m_map_prm.find(vname)) {
//		std::cout<<"E...SetDoPrm::getValue: vname "<<vname
//		<<" not found in m_map_prm."<<std::endl;
//		exit(1);
//	}

//	std::string valstr = m_map_prm[vname];
	std::string valstr = m_map_prm[ipos].second;

	T value;
	std::stringstream ss;
	ss.clear();
	ss<<valstr;
	ss>>value;
	
	return value;
	
}
//template std::string &SetDoPrm::getValue<std::string>(const std::string& vname){
//std::string &SetDoPrm::getValue<std::string>(const std::string& vname){
template<> std::string SetDoPrm::getValue<std::string>(const std::string& vname)
const {
	
	if (9<s_PrtLvl) std::cout << "D10"
		<<"...SetDoPrm::getValue<std::string>: called."<<std::endl;
	
	int ipos = 0;
	//for (ipos=0; ipos<m_nmaps; ipos++) {
    for (ipos=m_nmaps-1; ipos>-1; ipos--) {
		if(m_map_prm[ipos].first==vname) break;
		//if (m_nmaps-1==ipos) {
        if (0==ipos) {
			std::cout<<"E...SetDoPrm::getValue: vname "<<vname
			<<" not found in m_map_prm."<<std::endl;
			exit(1);
		}
	}
//	if (m_map_prm.end() == m_map_prm.find(vname)) {
//		std::cout<<"E...SetDoPrm::getValue: vname "<<vname
//		<<" not found in m_map_prm."<<std::endl;
//		exit(1);
//	}

//	std::string valstr = m_map_prm[vname];
	std::string valstr = m_map_prm[ipos].second;
//remove double||single quotations
	if (('\"'==valstr.at(0)&&'\"'==valstr.at(valstr.size()-1))
		||('\''==valstr.at(0)&&'\''==valstr.at(valstr.size()-1))) {
		valstr = valstr.substr(1,valstr.size()-2);
	}
	return valstr;
	
}
//template bool &SetDoPrm::getValue(const std::string& vname){
template<> bool SetDoPrm::getValue(const std::string& vname) const {
	
	if (9<s_PrtLvl) std::cout << "D10"
		<<"...SetDoPrm::getValue<bool>: called."<<std::endl;
	
	int ipos = 0;
    //for (ipos=0; ipos<m_nmaps; ipos++) {
    for (ipos=m_nmaps-1; ipos>-1; ipos--) {
		if(m_map_prm[ipos].first==vname) break;
        //if (m_nmaps-1==ipos) {
        if (0==ipos) {
			std::cout<<"E...SetDoPrm::getValue: vname "<<vname
			<<" not found in m_map_prm."<<std::endl;
			exit(1);
		}
	}
//	if (m_map_prm.end() == m_map_prm.find(vname)) {
//		std::cout<<"E...SetDoPrm::getValue: vname "<<vname
//		<<" not found in m_map_prm."<<std::endl;
//		exit(1);
//	}

//	std::string valstr = m_map_prm[vname];
	std::string valstr = m_map_prm[ipos].second;
	bool valbool=false;
    if ("true"==valstr||"True"==valstr||"1"==valstr) { //"True" added for python expression 160721
		valbool=true;
	}
	return valbool;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
int SetDoPrm::s_PrtLvl=10;
}//namespace
