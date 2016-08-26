/*    Read an asc data and convert it into a root tree file 
    =========================================================
 If you read this line, the file is a copy from cpplib/asource/rootData/asc2rt.cpp
 121106
 120605-11
 121204 modified for SP8acc/optics
*/
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "MyRoot-v02.3.h"
#include "StrTable-v02.2.h"

main(int argn,char* argv[]) {
	using namespace std;
	string parfilename = "dolist.dat";
	string maincntl="3";
	
	if (1<argn) {
		parfilename = *(argv+1);
	}
	if (3==argn) {
		maincntl = *(argv+2);
	}
	
	MyRoot *myroot = new MyRoot();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//                           to use MyRoot::functions()
	// Reading dolist
	//==================
	// (As the first time of calling, parameters will be read from parfilename.)
	std::string dolisttmpl="Variable \"value\"";//Provide a template line of the dolist data file
	myroot->SetFParam(parfilename.c_str(),dolisttmpl, "0");
	int PrtLvl = myroot->GetPrtLvl();
	cout<<"N...main:PrtLvl = "<<PrtLvl<<endl;
	cout << "N...main:assigned AscFileName = "<<myroot->GetAscFileName()<<endl;
	cout << "N...main:assigned RootFileName = "<<myroot->GetRootFileName()<<endl;
	
	
	// Read data file
	//===================	
	ifstream ifs(myroot->GetAscFileName());
	if (ifs.fail()) {
		std::cerr <<"E...main: Asc2File open failuer: "
		<<myroot->GetAscFileName()<<std::endl;
		exit;
	}	
	ofstream ofs(myroot->GetLogFileName());
	if (ofs.fail()) {
		std::cerr <<"E...main: LogFile open failuer: "
		<<myroot->GetLogFileName()<<std::endl;
		exit;
	}	
	
	//Define a StrTable object with a data line template
	//Get the data line template
	std::string datatmpl = myroot->GetAscDataTempl();
	cout << "N...main:assigned asc data templ = "<<datatmpl<<endl;
	//Define a StrTable obj 
	StrTable datatbl;
	datatbl.clear_table();	//<<<<<  StrTable object is constructed in MyRoot->SetFParam
	datatbl.put_line(datatmpl);	
//	StrTable datatbl(datatmpl);	//<<<<< recover this line after improvement
	string datalinestruct = 	datatbl.get_line_format();
	std::cout<<"N...main: .get_line_format() = '"
	<< datalinestruct
	<<"' "<<datalinestruct.length()<<" columns"  <<std::endl;
	
// reading lines in the input file
//	int nskip = 44; // number of header lines //<<<< depends on asc file: remove later
	int nskip = 0; // number of header lines //<<<< depends on asc file: remove later
	int nline; // storage to get the number
	int ncomment;
	StrTable::eatTable(ifs, nskip, datalinestruct, nline);
 	cout<<"N...main: eatTable passed...nline = "<<nline<<", nskip = "<<nskip<<endl;
	ifs.close();
//-----------------------------------------------------------------	
// analyse comment lines
	for (int icom=0; icom<nskip; icom++) {
//		cout << "c"<<icom<<": "<<datatbl.get_comment(icom)<<endl;
		ofs <<datatbl.get_comment(icom)<<endl;
//		string::size_type index = datatbl.get_comment(icom).find("tune");
//		if (string::npos == index) {
//			continue;
//		}
//		else {
//			cout<<datatbl.get_comment(icom).substr(index)<<endl;
//		}
	}
	
	
	
	
//prepare for the output	
	TTree *tree;
	TFile *outf;
	
	string dfname = myroot->GetAscFileName();
	string ofname = myroot->GetRootFileName();
	
	tree = new TTree("T","data from ascii file");	
	outf = new TFile(ofname.c_str(),"RECREATE");

//	int itime0, itime;
//	double d_currmA;
//	double scoord;
//	double betax, alphax, gammax, nux, etax, etapx;
//	double betay, alphay, gammay, nuy, etay, etapy;
	double scoord;
	double betax, alphax, etax, etapx, sigx, sigxp;
	double betay, alphay, etay, etapy, sigy, sigyp;
	enum DataTag {s_scoord, s_betax, s_alphax, s_betay, s_alphay, 
		       s_etax, s_etapx, s_sigx, s_sigxp, s_sigy, s_sigyp};
	tree->Branch("scoord",&scoord,"scoord/D");
	tree->Branch("betax",&betax,"betax/D");
	tree->Branch("alphax",&alphax,"alphax/D");
	tree->Branch("betay",&betay,"betay/D");
	tree->Branch("alphay",&alphay,"alphay/D");
	tree->Branch("etax",&etax,"etax/D");
	tree->Branch("etapx",&etapx,"etapx/D");
	tree->Branch("sigx",&sigx,"sigx/D");
	tree->Branch("sigxp",&sigxp,"sigxp/D");
	tree->Branch("sigy",&sigy,"sigy/D");
	tree->Branch("sigyp",&sigyp,"sigyp/D");
		
	int iline;
	for (iline=0; iline<nline; iline++) {
//		double d_value[13];
//		for(int item=0; item<13; item++){
		double d_value[datalinestruct.length()];
		stringstream ss;
		for(int item=1; item<datalinestruct.length(); item++){
		ss.clear();
		ss<<datatbl.get_table(iline,item);
		//	ss<<datatbl.get_table(iline,static_cast<int>(s_x0s));
		ss>>d_value[item];
		}
		int i=1;
		scoord = d_value[i++];
		betax = d_value[i++];
		alphax = d_value[i++];
		betay = d_value[i++];
		alphay = d_value[i++];
		etax = d_value[i++];
		etapx = d_value[i++];
		sigx = d_value[i++];
		sigxp = d_value[i++];
		sigy = d_value[i++];
		sigyp = d_value[i++];
		
		tree->Fill();
		
		
/**/	
	}

	tree->Write();  
	cout <<"N...main: # data lines converted= "<< iline<<endl;
	delete tree;
	delete outf;
	
	return(0);
	
}

