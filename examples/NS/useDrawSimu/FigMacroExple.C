#include <iostream>
//#include <memory>
//#include <TLatex>
//#include <TPolyLine.h>
//#include "TPolyLine.h"
//#include "FigPtInteraction.h"
//#include "galeo.h"

void FigPtLongMacro(double &xwmin,double &xwmax,double &ywmin,double &ywmax,
                        double &gxscale, double &gyscale,double & anglstep,
                        double &Ylattice)
{
    if (6<galeo::SetDoPrm::GetPrtLvl()) std::cout << "D7"
        <<"...FigPtInteractionMacro("
        <<"...): called."<<std::endl;

    if (8<galeo::SetDoPrm::GetPrtLvl()) std::cout << "D9"
		<< "...FigPtInteractionMacro.C: params. passed:"
        <<" xwmin = "<<xwmin<<" xwmax = "<<xwmax
        <<" ywmin = "<<ywmin<<" ywmax = "<<ywmax<<std::endl;
        std::cout<<"  ..... gxscale = "<<gxscale<<" gyscale = "<<gyscale
        <<" anglstep = " <<anglstep
        <<" Ylattice = "<<Ylattice
        << std::endl;
#if(0)
    
    double lencell = CintCLSLatticePrm::lencell;
    int nelmcell = CintCLSLatticePrm::nelmcell;
    int ncellring = CintCLSLatticePrm::ncellring;
    //lengths of elements
    double D1=CintCLSLatticePrm::D1;
    double DF=CintCLSLatticePrm::DF;
    double DE=CintCLSLatticePrm::DE;
    double D2=CintCLSLatticePrm::D2;
    double D3=CintCLSLatticePrm::D3;
    double D4=CintCLSLatticePrm::D4;
    double D5=CintCLSLatticePrm::D5;
    double D6=CintCLSLatticePrm::D6;
    double D7=CintCLSLatticePrm::D7;
    double D8=CintCLSLatticePrm::D8;
    //
    double Q1=CintCLSLatticePrm::Q1;
    double Q2=CintCLSLatticePrm::Q2;
    double Q3=CintCLSLatticePrm::Q3;
    double S1=CintCLSLatticePrm::S1;
    double S2=CintCLSLatticePrm::S2;
    //
    double HC=CintCLSLatticePrm::HC;
    
    
    CintCLSLatticePrm::fillLattice();
    std::vector<std::string> vecElements = CintCLSLatticePrm::GetVecElm();

    //    gROOT->ProcessLine(".g");
    
    if (8<galeo::SetDoPrm::GetPrtLvl()){
        std::cout << "D9"
        <<"...Macro::passed from CintCLSLatticePrm:"
        <<" lencell = "<<lencell
        <<" nelmcell = "<<nelmcell
        <<" ncellring = "<<ncellring
        <<"\n   elements[28,92] = ";
        for (int i=28; i<=92; i++) {
            std::cout<<vecElements[i]<<",";
        }
        std::cout<<std::endl;
    }
    

    // define drawing components
    //-----------------------------
    Double_t xp[5],yp[5];
	double angl=0;
	
    TPolyLine *box = new TPolyLine(5);
	box->SetLineWidth(1);
	box->SetLineColor(2);//Red
	double lboxside=0.15;
    TPolyLine *line = new TPolyLine(2);
	line->SetLineColor(1);//Black

    int icell0, icell1;
    if (fabs(anglstep)<1.e-9) {
		icell0 = xwmin / lencell;   // can be negative
		if (0>xwmin) {	icell0=icell0-1;}
		icell1 = xwmax / lencell;
		if (0>xwmax) {	icell1=icell1-1;}
		cout << "D...lencell = "<<lencell
        <<" icell0 = "<<icell0<<" icell1 = "<<icell1<<endl;
    }
    else {	icell1 = icell0; }
    
    double xlast;
    double ylast;
    double scd;
    
    
    //-------------------------------------------------//loop of cell
    for (int icell=icell0; icell<icell1+1; icell++) {
        double soffset=0.;
        if (fabs(anglstep)<1.e-9)
            soffset = icell*lencell;
        //			if (icell==icell0) 	double remleft = xwmin - soffset;
        //			if (icell==icell1) 	double remright = xwmax - soffset;
        cout << "D...loop of cells: icell = "<<icell<<endl;
        
        xlast=soffset;
        ylast=Ylattice;
        scd=soffset;
        cout << "D...soffset="<<soffset<<endl;
        
        //-------------------------------------------------//loop of elements
        //		for (idel=0; idel<nelmcell*ncellring; idel++) {
        //	for (int idel=0; idel<nelmcell; idel++) {
        for (int idel=idel0; idel<idel1; idel++) {
            double elmlen=0.;
            int nparts=0;
            int npoint1=0;
            int npoint2=0;
            int npoint3=0;
            double cth, sth;
            cth = cos(angl); sth = sin(angl);
            if ("D2"==vecElements[idel]) {
                elmlen = D2;
                nparts=1;
                npoint1=2;
            }
            if ("D3"==vecElements[idel]) {
                elmlen = D3;
                nparts=1;
                npoint1=2;
            }
            if ("D4"==vecElements[idel]) {
                elmlen = D4;
                nparts=1;
                npoint1=2;
            }
            if ("D5"==vecElements[idel]) {
                elmlen = D5;
                nparts=1;
                npoint1=2;
            }
            if ("D6"==vecElements[idel]) {
                elmlen = D6;
                nparts=1;
                npoint1=2;
            }
            if ("D7"==vecElements[idel]) {
                elmlen = D7;
                nparts=1;
                npoint1=2;
            }
            if ("D8"==vecElements[idel]) {
                elmlen = D8;
                nparts=1;
                npoint1=2;
            }
            if ("DE"==vecElements[idel]) {
                elmlen = DE;
                nparts=1;
                npoint1=2;
            }
            if ("HC"==vecElements[idel]) {
                elmlen = HC;
                //			angl+=
                nparts=2;
                npoint1=2;
                npoint2=4;
            }
            if ("Q1"==vecElements[idel]) {
                elmlen = Q1;
                nparts=3;
                npoint1=2;
                npoint2=2;
                npoint3=4;
            }
            if ("Q2"==vecElements[idel]) {
                elmlen = Q2;
                nparts=3;
                npoint1=2;
                npoint2=2;
                npoint3=4;
            }
            if ("Q3"==vecElements[idel]) {
                elmlen = Q3;
                nparts=3;
                npoint1=2;
                npoint2=2;
                npoint3=4;
            }
            if ("S1"==vecElements[idel]) {
                elmlen = S1;
                nparts=2;
                npoint1=2;
                npoint2=4;
            }
            if ("S2"==vecElements[idel]) {
                elmlen = S2;
                nparts=2;
                npoint1=2;
                npoint2=4;
            }
            
            
            
            //		if (std::string::npos==vecElements[idel].find('D') ){
            if (std::string::npos!=vecElements[idel].find('D') ){
                
                xp[0]=xlast; yp[0]=ylast;
                xp[1]=xlast+cth*gxscale*elmlen; yp[1]= ylast + sth*gyscale*elmlen;
                //			xlast = xp[1]; ylast = yp[1];
                
                if (xwmin>xp[0]) xp[0] = xwmin;
                if (xwmax<xp[1]) xp[1] = xwmax;
                //			cout << "D...idel = "<<idel<<" xp0 = "<<xp[0]<<" xp1 = "<<xp[1]<<endl;
                if (xwmax>=xp[0]&&xwmin<=xp[1])
                    line->DrawPolyLine(2,xp,yp);
                
                
                //			std::cout<<vecElements[idel]<<" ";
                
            }//End case D
            else {
                if (std::string::npos!=vecElements[idel].find('H') ){
                    angl+=anglstep;
                    cth = cos(angl); sth = sin(angl);
                    double xc0=xlast-sth*gxscale*(lboxside/2.);
                    double yc0=ylast+cth*gyscale*(lboxside/2.);
                    double xc1=xc0+cth*gxscale*elmlen;
                    double yc1=yc0+sth*gyscale*elmlen;
                    double xc2=xc1+sth*gxscale*lboxside;
                    double yc2=yc1-cth*gyscale*lboxside;
                    double xc3=xc2-cth*gxscale*elmlen;
                    double yc3=yc2-sth*gyscale*elmlen;
                    double xc4=xc0;
                    double yc4=yc0;
                    
                    //we are assuming the lattice directs from the left to the right.
                    xp[0]=xc0;	yp[0]=yc0;
                    xp[1]=xc1;	yp[1]=yc1;
                    if (xwmin>xc0) xp[0] = xwmin;
                    if (xwmax<xc1) xp[1] = xwmax;
                    if (xwmax>=xp[0]&&xwmin<=xp[1])
                        line->DrawPolyLine(2,xp,yp);
                    xp[0]=xc1;	yp[0]=yc1;
                    xp[1]=xc2;	yp[1]=yc2;
                    if (xwmax>=xp[0]&&xwmax>=xp[1]&&xwmin<=xp[0]&&xwmin<=xp[1])
                        line->DrawPolyLine(2,xp,yp);
                    xp[0]=xc3;	yp[0]=yc3;
                    xp[1]=xc2;	yp[1]=yc2;
                    if (xwmin>xc3) xp[0] = xwmin;
                    if (xwmax<xc2) xp[1] = xwmax;
                    if (xwmax>=xp[0]&&xwmin<=xp[1])
                        line->DrawPolyLine(2,xp,yp);
                    xp[0]=xc3;	yp[0]=yc3;
                    xp[1]=xc4;	yp[1]=yc4;
                    if (xwmax>=xp[0]&&xwmax>=xp[1]&&xwmin<=xp[0]&&xwmin<=xp[1])
                        line->DrawPolyLine(2,xp,yp);
                    //				if (xwmin<scd&&xwmax>scd)
                    //					box->DrawPolyLine(5,xp,yp);
                    angl+=anglstep;
                }//End case HC
                if (std::string::npos!=vecElements[idel].find('Q') ){
                    xp[0]=xlast-sth*gxscale*(lboxside/2.);
                    yp[0]=ylast+cth*gyscale*(lboxside/2.); ;
                    xp[1]=xp[0]+cth*gxscale*elmlen;
                    yp[1]=yp[0]+sth*gyscale*elmlen;
                    xp[2]=xp[1]+sth*gxscale*lboxside;
                    yp[2]=yp[1]-cth*gyscale*lboxside;
                    xp[3]=xp[2]-cth*gxscale*elmlen;
                    yp[3]=yp[2]-sth*gyscale*elmlen;
                    xp[4]=xp[0];
                    yp[4]=yp[0];
                    if (xwmin<scd&&xwmax>scd)
                        box->DrawPolyLine(5,xp,yp);
                    xp[4]=xp[1];
                    yp[4]=yp[1];
                    xp[1]=xp[2];
                    yp[1]=yp[2];
                    if (xwmin<scd&&xwmax>scd)
                        box->DrawPolyLine(2,xp,yp);
                    xp[0]=xp[4];
                    yp[0]=yp[4];
                    xp[1]=xp[3];
                    yp[1]=yp[3];
                    if (xwmin<scd&&xwmax>scd)
                        box->DrawPolyLine(2,xp,yp);
                }//End case Q
                if (std::string::npos!=vecElements[idel].find('S') ){
                    xp[0]=xlast-sth*gxscale*(lboxside/2.);
                    yp[0]=ylast+cth*gyscale*(lboxside/2.); ;
                    xp[1]=xp[0]+cth*gxscale*elmlen;
                    yp[1]=yp[0]+sth*gyscale*elmlen;
                    xp[2]=xp[1]+sth*gxscale*lboxside;
                    yp[2]=yp[1]-cth*gyscale*lboxside;
                    xp[3]=xp[2]-cth*gxscale*elmlen;
                    yp[3]=yp[2]-sth*gyscale*elmlen;
                    xp[4]=xp[0];
                    yp[4]=yp[0];
                    if (xwmin<scd&&xwmax>scd)
                        box->DrawPolyLine(5,xp,yp);
                    
                    
                }//End case S
            }//End case not D
            
            
            xlast += cth*gxscale*elmlen;
            ylast += sth*gyscale*elmlen;
            
            
            scd+=elmlen;
            
            
            if (5<galeo::SetDoPrm::GetPrtLvl())
               if (idel<23)
                    std::cout << vecElements[idel]<<" "<<scd<<std::endl;
            
            
            
            
        }//                          End loop of elements
	}//               End loop of cell



    delete line;
    delete box;
#endif
}
