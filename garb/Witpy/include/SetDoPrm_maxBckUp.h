// If you read this line, the file is a copy from cpplib/S8L/SetDoPrm-v01.1.h
// If you read this line, the file is originally a copy from cpplib/include/MyRoot-v02.4.h
#ifndef __SetDoPrm_V01_3_H
#define __SetDoPrm_V01_3_H
#include <string>
#include <vector>
#include <utility>
#include <map>
//#include "StrTable.h"

//using namespace std;
namespace witgn {
//class SetDoPrm:public virtual StrTable {
class SetDoPrm {
public:
//
	SetDoPrm(); //Default constructor
	SetDoPrm(const std::string doprmfilename);// A constructor with doprm file name
	virtual ~SetDoPrm();
	const int& getNmaps() const {return m_nmaps;}
	const bool is_a_key(const std::string& vname) const;
template <typename T>
	void setValue(const std::string &vname, const T &value);
	template <typename T> T getValue(const std::string& vname) const;
	const std::vector<std::pair<std::string,std::string> > getMap() const
        {return m_map_prm;}
	static void SetPrtLvl(const int& val) {s_PrtLvl = val;}
	static const int& GetPrtLvl() {return s_PrtLvl;}
	
protected:
    static int s_PrtLvl;
	
private:
    PyObject *pDoPrm_lst;
    
	int	m_nmaps;
	std::vector<std::pair<std::string,std::string> > m_map_prm;
};
}//namespace
#endif