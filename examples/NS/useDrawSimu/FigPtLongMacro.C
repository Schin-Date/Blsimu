#include <iostream>
//#include <memory>
//#include <TLatex>
//#include <TPolyLine.h>
//#include "TPolyLine.h"
//#include "FigPtInteraction.h"
//#include "galeo.h"

void FigPtLongMacro(double &xfrmin,double &xfrmax,
                    double &yfrmin,double &yfrmax)
{
    if (6<witgn::SetDoPrm::GetPrtLvl()) std::cout << "D7"
        <<"...FigPtLongMacro("
        <<"...): called."<<std::endl;

    if (8<witgn::SetDoPrm::GetPrtLvl()) std::cout << "D9"
		<< "...FigPtLongMacro.C: params. passed:"
        <<" xfrmin = "<<xfrmin<<" xfrmax = "<<xfrmax
        <<" yfrmin = "<<yfrmin<<" yfrmax = "<<yfrmax<<std::endl;

    TText* txt;
    TArrow arrow;

    txt = new TText(5.7,4500,"Number of Interactions");
    txt->SetTextColor(1);
    txt->SetTextSize(0.04);
    txt->Draw();
    txt = new TText(5.9,4230,"(30k in total)");
    txt->SetTextSize(0.04);
    txt->Draw();
    arrow.SetLineColor(1);
    arrow.DrawArrow(5.9,4300,5.5,4300,0.03,">");
    
    txt = new TText(7.3,800,"laser profile (mm)");
    txt->SetTextColor(3);
    txt->SetTextSize(0.04);
    txt->Draw();
    txt = new TText(7.5,600,"10.6 um");
    txt->SetTextColor(3);
    txt->SetTextSize(0.04);
    txt->Draw();
    arrow.SetLineColor(3);
    arrow.DrawArrow(8.2,650,8.5,650,0.03,">");
//    txt = new TText(7,110,"profile");
//    txt->SetTextColor(3);
//    txt->SetTextSize(0.04);
//    txt->Draw();
//    txt = new TText(7.6,110,"(mm)");
//    txt->SetTextColor(3);
//    txt->SetTextSize(0.04);
//    txt->Draw();
    
    TLatex *tex = new TLatex();
    tex->SetTextColor(2);
//    tex->SetTextSize(0.05);
    tex->DrawLatex(7.7,3500,"#scale[0.8]{#beta}_{x}(m)");
    arrow.SetLineColor(2);
    arrow.DrawArrow(8.3, 3520,8.7,3520,0.03,">");

}
