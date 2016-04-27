
#include "Macros/makeWorkspace2015.C"
#include "Macros/buildModel.C"

#include "Macros/drawPlot.C"
#include "Macros/options.h"

void SetOptions(InputOpt* opt, bool isData = true, bool isPbPb = false,bool inExcStat= false,int bin=0,bool doPt = false, bool doRap=false,bool doCent=false,bool doUL =false) {

  opt->isData    = isData;
  opt->isPbPb    = isPbPb;

  time_t t = time(0);
  struct tm * now = localtime( & t );
  if(doPt){opt->plotDir   = Form("Plots_%d_%d_%d/PT",1+now->tm_mon,now->tm_mday,1900+now->tm_year);}
  if(doCent){opt->plotDir   = Form("Plots_%d_%d_%d/Cent",1+now->tm_mon,now->tm_mday,1900+now->tm_year);}
  if(doRap){opt->plotDir   = Form("Plots_%d_%d_%d/Rap",1+now->tm_mon,now->tm_mday,1900+now->tm_year);}
  if(!doPt && !doRap && !doCent){opt->plotDir   = Form("Plots_%d_%d_%d/Integrated",1+now->tm_mon,now->tm_mday,1900+now->tm_year);}


  opt->lumi      = opt->isPbPb ? "PbPb" : "pp" ;

  opt->dMuon.M.Min = 8.0;
  opt->dMuon.M.Max = 14.; 
  opt->sMuon.Pt.Min  = 4;

  if(doPt && !doRap && !doCent){
  opt->dMuon.Pt.Min = DPtMinArr[bin];
  opt->dMuon.Pt.Max = DPtMaxArr[bin];
  }
  else{
  opt->dMuon.Pt.Min = 0;
  opt->dMuon.Pt.Max = 30;
  }
  if(doRap && !doPt && !doCent){
  opt->dMuon.AbsRap.Min = DRapMinArr[bin];
  opt->dMuon.AbsRap.Max = DRapMaxArr[bin];
  }
  else{
  opt->dMuon.AbsRap.Min = 0;
  opt->dMuon.AbsRap.Max = 2.4;
  }
  if(doCent && !doRap && !doPt){ 
  opt->Centrality.Start = !doUL ? CentMinArr[bin] : CentMinArrUL[bin];
  opt->Centrality.End   = !doUL ? CentMaxArr[bin] : CentMaxArrUL[bin];
  }
  else{
  opt->Centrality.Start = opt->isPbPb ? 0 : -1;
  opt->Centrality.End   = opt->isPbPb ? 160 : 1;
  }
  
  //Single Muon Parameters
  opt->sMuon.Pt.Max  = 100;
  opt->sMuon.Eta.Min = -2.4;
  opt->sMuon.Eta.Max = 2.4;
  
  opt->RunNb.Start      = opt->isPbPb ? 262620 : 262157;
  opt->RunNb.End        = opt->isPbPb ? 263757 : 262328;
  if(!isData){ 
  opt->RunNb.Start      = 0;
  opt->RunNb.End        = 4;
  }
  return;
};

void fit2015(
           TString FileName ="OniaTree_262163_262328.root", 
           bool isData    = true,     // isData = false for MC, true for Data
           bool isPbPb    = true,    // isPbPb = false for pp, true for PbPb
           bool doFit = true,
           bool inExcStat = true,     // if inExcStat is true, then the excited states are fitted
           int bin = 0,
           bool doPt    = false,    
           bool doRap    = false,   
           bool doCent    = false,  
           bool doPeriph = false,
	   bool doUL = false
           ) {


gROOT->Macro("~/logon.C+");

InputOpt opt;
SetOptions(&opt, isData, isPbPb,inExcStat,bin,doPt,doRap,doCent,doUL);

 
int nbins = 60; //ceil((opt.dMuon->M->Max - opt.dMuon->M->Min)/binw);

RooWorkspace myws;
myws.SetName("myws");
TH1F* hDataOS =  new TH1F("hDataOS","hDataOS", nbins, opt.dMuon.M.Min, opt.dMuon.M.Max);


makeWorkspace2015(myws, FileName, opt, hDataOS,doPeriph);

RooRealVar* mass      = (RooRealVar*) myws.var("invariantMass"); 
RooDataSet* dataOS_fit = (RooDataSet*) myws.data("dataOS");
RooDataSet* dataSS_fit = (RooDataSet*) myws.data("dataSS");
RooAbsPdf*  pdf = NULL;

int sigModel, bkgModel;      
sigModel = inExcStat ? 2 : 2; //2 nominal
bkgModel = 1; //1 nominal, 2 + poly, 3+ poly2,4+poly3,5 +offset, 6exp 7 exp+poly1
if(doPt && bin!=0)bkgModel = 6; //1 nominal, 2 + poly, 3+ poly2,4+poly3,5 +offset, 6exp 7 exp+poly1
         
buildModel(myws, sigModel, bkgModel,inExcStat,bin,doPt,doRap,doCent,doPeriph,opt);

pdf       =(RooAbsPdf*)  myws.pdf("pdf");
RooFitResult* fitObject = pdf->fitTo(*dataOS_fit,Save(),NumCPU(4),Timer(kTRUE),Hesse(kTRUE),Extended(kTRUE)); // Fit
//RooFitResult* fitObject = pdf->fitTo(*dataSS_fit,Save(),NumCPU(4),Timer(kTRUE),Hesse(kTRUE),Extended(kTRUE)); // Fit
//cout<<fitObject<<endl;
//cout << "correlation between 1S and 2S"<< fitObject->correlation("N_{#varUpsilon(1S)}","N_{#varUpsilon(2S)}") << endl ;

RooPlot* frame = mass->frame(Bins(nbins),Range(opt.dMuon.M.Min, opt.dMuon.M.Max));  
RooPlot* frame2 = NULL;
//dataSS_fit->plotOn(frame, Name("dataSS_FIT"), MarkerColor(kRed), LineColor(kRed), MarkerSize(1.2)); 
dataOS_fit->plotOn(frame, Name("dataOS_FIT"), MarkerColor(kBlack), LineColor(kBlue), MarkerSize(1.2));


pdf->plotOn(frame,Components("bkgPdf"),LineStyle(kDashed));
pdf->plotOn(frame,Name("thePdf"));
//pdf->plotOn(frame,Name("thePdf"),Normalization(dataOS_fit->sumEntries(),RooAbsReal::NumEvent));


//ChiSquare and Pulls!   
double chi2FromRoo = frame->chiSquare(fitObject->floatParsFinal().getSize());
cout<<"chi2 from simple pull= "<<frame->chiSquare()<<"\t chi2 from RooFit= "<<chi2FromRoo <<endl;
RooHist *hPull = frame->pullHist(0,0,true); // this calcualtes the pulls taking the integral of the fit in each bin, instead of the value in the middle of the bid
hPull->SetName("hPull");
double *ypull     = hPull->GetY();

TH1 *phData      = dataSS_fit->createHistogram("invariantMass",nbins);
int nFullBinsPull = 0;
for (int i=0; i < nbins; i++)
{
   if (phData->GetBinContent(i) == 0) continue;
   nFullBinsPull++;
}
int nFitParam     = fitObject->floatParsFinal().getSize();
int Dof           = nFullBinsPull - nFitParam;
double myChi2 = chi2FromRoo*Dof;


hPull->SetMarkerSize(1.2);
frame2 = mass->frame(Title("Pull Distribution"),Bins(nbins),Range(opt.dMuon.M.Min,opt.dMuon.M.Max));
frame2 -> addPlotable(hPull,"PX");  
   


drawPlot(frame,frame2, pdf, opt, doFit,inExcStat,myChi2,Dof,sigModel,bkgModel);



string myParams = Form("%s_DATA_sig_%d_Bkg_ErfExp%d_%d_%d_%dcent%d_%.1fpt%.1f_%.1fy%.1f", (opt.isPbPb ? "PbPb" : "PP"), sigModel,bkgModel,opt.RunNb.Start, opt.RunNb.End,opt.Centrality.Start, opt.Centrality.End,opt.dMuon.Pt.Min,opt.dMuon.Pt.Max,opt.dMuon.AbsRap.Min,opt.dMuon.AbsRap.Max);

std::replace(myParams.begin(),myParams.end(),'.','p');
myws.SaveAs(opt.plotDir+"/FIT_"+myParams+".root");


}



