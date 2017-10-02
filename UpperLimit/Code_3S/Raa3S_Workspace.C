#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "TCanvas.h"
#include "RooWorkspace.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"
#include "RooArgSet.h"
#include "RooArgList.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooStats/BayesianCalculator.h"
#include "RooStats/ModelConfig.h"
#include "RooStats/SimpleInterval.h"
#include "TAxis.h"
#include <iostream>
#include <TString.h>
#include <TH1F.h>
#include <TTree.h>
#include <TFile.h>
#include <TChain.h>
#include <TNtuple.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TROOT.h>
#include <fstream>
#include <TGraph.h>
#include "TMath.h"
#include "TF1.h"
#include <RooMinuit.h>

#include "test_combine.C"

using namespace RooFit;
using namespace RooStats;

void Raa3S_Workspace(const char* name_pbpb="fitresult.root", const char* name_pp="fitresult_pp.root", const char* name_out="fitresult_combo2.root",double sysNumPerc=0.0){


   RooWorkspace * ws = test_combine(name_pbpb, name_pp);
   RooAbsData * data = ws->data("data");
bool doSyst = true;

   RooRealVar* DR31 = new RooRealVar("DR31","DoubleRatio(#Upsilon (3S))",0.5,0.,1.);
   RooRealVar* frac3Sv1S_pp = ws->var("R_{#frac{3S}{1S}}_pp");
   RooRealVar* frac3Sv1S_hi = ws->var("R_{#frac{3S}{1S}}_hi");
   DR31->setVal(frac3Sv1S_hi->getVal()/frac3Sv1S_pp->getVal());
   cout<<"nsig_PbPb: "<<frac3Sv1S_hi->getVal()<<endl;
   cout<<"nsig_pp: "<<frac3Sv1S_pp->getVal()<<endl;
   cout<<"DR31: "<<DR31->getVal()<<endl;
   DR31->setError(sqrt(pow(frac3Sv1S_hi->getError()/frac3Sv1S_hi->getVal(),2) + pow(frac3Sv1S_pp->getError()/frac3Sv1S_pp->getVal(),2))*DR31->getVal());
   cout<<"DR31 Error: "<<DR31->getError()<<endl;

   double sysNumAbs = sysNumPerc*(DR31->getVal());
   //double sysNumAbs = sysNumPerc;
   cout<<"systematic: "<<sysNumAbs<<endl;
  


   RooRealVar* leftEdge = new RooRealVar("leftEdge","leftEdge",0.);
   RooRealVar* rightEdge = new RooRealVar("rightEdge","rightEdge",1.);
   RooGenericPdf step("step", "step", "(@0 >= @1) && (@0 < @2)", RooArgList(*DR31, *leftEdge, *rightEdge));
   ws->import(step);
   ws->factory( "Uniform::flat(DR31)" );
//-- From jpsi crew
//
   //ws->factory( Form("kappa_syst[%f]",0.03073) );//int
   //ws->factory( Form("kappa_syst[%f]",0.08956) );//0-10
   //ws->factory( Form("kappa_syst[%f]",.020239) );//10-30
   //ws->factory( Form("kappa_syst[%f]",0.03846) );//30-50
   //ws->factory( Form("kappa_syst[%f]",0.051807) );//50-100
   ws->factory( Form("kappa_syst[%f]",sysNumAbs) );//50-100
   ws->factory( "expr::alpha_syst('kappa_syst*beta_syst',kappa_syst,beta_syst[0,-5,5])" );
   ws->factory( "Gaussian::constr_syst(beta_syst,glob_syst[0,-5,5],1)" );
  
   //ws->factory( Form("kappa_syst_add[%f]",systValAdd) );
   //ws->factory( "Gaussian::constr_syst_add(beta_syst_add[0,-5,5],glob_syst_add[0,-5,5],kappa_syst_add)" );

   RooRealVar* alphaSyst = ws->function("alpha_syst");

   RooFormulaVar *nsig1_hi = (RooFormulaVar*)ws->function("N_{#varUpsilon(1S)}_hi");
     //cout << frac3Sv1S_pp << endl;
   
   RooFormulaVar nsig3_hi_modified("nsig3_hi_modified", "(@0+@1)*@2*@3", RooArgList(*DR31,*alphaSyst, *frac3Sv1S_pp,*nsig1_hi));
   //RooFormulaVar nsig2_hi_modified("nsig2_hi_modified", "@0*@1*@2", RooArgList(*DR21, *frac2Sv1S_pp,*nsig1_hi));
   ws->import(nsig3_hi_modified);

   RooAbsPdf* sig1S_hi = ws->pdf("sig1S_hi");
   RooAbsPdf* sig2S_hi = ws->pdf("sig2S_hi");
   RooAbsPdf* sig3S_hi = ws->pdf("sig3S_hi");
   RooAbsPdf* nbkg_hi = ws->pdf("bkgPdf_hi");
   
   RooFormulaVar *nsig2_hi = (RooFormulaVar*)ws->function("N_{#varUpsilon(2S)}_hi");
   RooFormulaVar *nsig3_hi = (RooFormulaVar*)ws->function("nsig3_hi_modified"); 
   RooRealVar* norm_nbkg_hi = ws->var("n_{Bkgd}_hi");

   RooArgList pdfs_hi( *sig1S_hi,*sig2S_hi,*sig3S_hi, *nbkg_hi);
   RooArgList norms_hi(*nsig1_hi,*nsig2_hi,*nsig3_hi, *norm_nbkg_hi);


   RooAddPdf model_num("model_num", "model_num", pdfs_hi,norms_hi); 
   ws->import(model_num);
   ws->factory("PROD::model_hi(model_num, constr_syst)");



   ws->factory("SIMUL::joint(dataCat,hi=model_hi,pp=pdf_pp)");

   ws->Print();

   ws->var("beta_syst")->setConstant(kFALSE);


/*
   //  // background yield with systematics
   ws->factory( "kappa_syst[1.051]" );
   ws->factory( "expr::alpha_nbkg_hi('pow(nbkg_hi_kappa,beta_nbkg_hi)',nbkg_hi_kappa,beta_nbkg_hi[0,-5,5])" );
   ws->factory( "SUM::nbkg_hi_nom(alpha_nbkg_hi*bkgPdf_hi)" );
   ws->factory( "Gaussian::constr_nbkg_hi(beta_nbkg_hi,glob_nbkg_hi[0,-5,5],1)" );
   RooAbsPdf* sig1S_hi = ws->pdf("sig1S_hi");
   RooAbsPdf* sig2S_hi = ws->pdf("sig2S_hi");
   RooAbsPdf* sig3S_hi = ws->pdf("sig3S_hi");
   RooAbsPdf* LSBackground_hi = ws->pdf("nbkg_hi_nom");
   
   RooFormulaVar *nsig3_hi = (RooFormulaVar*)ws->function("N_{#varUpsilon(3S)}_hi");
   RooFormulaVar *nsig2_hi = (RooFormulaVar*)ws->function("nsig2_hi_modified"); 
 

   //RooRealVar* nsig2_hi = ws->var("N_{#Upsilon(2S)}_hi");
   //RooFormulaVar* nsig3_hi = ws->function("nsig3_hi_modified");
   //cout << nsig1_hi << " " << nsig2_hi << " " << nsig3_pp << endl;
   RooRealVar* norm_nbkg_hi = ws->var("n_{Bkgd}_hi");

   RooArgList pdfs_hi( *sig1S_hi,*sig2S_hi,*sig3S_hi, *LSBackground_hi);
   RooArgList norms_hi(*nsig1_hi,*nsig2_hi,*nsig3_hi, *norm_nbkg_hi);

   ////////////////////////////////////////////////////////////////////////////////

   ws->factory( "nbkg_pp_kappa[1.03]" );
   ws->factory( "expr::alpha_nbkg_pp('pow(nbkg_pp_kappa,beta_nbkg_pp)',nbkg_pp_kappa,beta_nbkg_pp[0.,-5.,5.])" );
   ws->factory( "SUM::nbkg_pp_nom(alpha_nbkg_pp*bkgPdf_pp)" );
   ws->factory( "Gaussian::constr_nbkg_pp(beta_nbkg_pp,glob_nbkg_pp[0.,-5.,5.],1)" );
   RooAbsPdf* sig1S_pp = ws->pdf("sig1S_pp");
   RooAbsPdf* sig2S_pp = ws->pdf("sig2S_pp");
   RooAbsPdf* sig3S_pp = ws->pdf("sig3S_pp");
   RooAbsPdf* LSBackground_pp = ws->pdf("nbkg_pp_nom");
 
   RooRealVar* nsig1_pp = ws->var("N_{#varUpsilon(1S)}_pp");
   RooFormulaVar *nsig2_pp = (RooFormulaVar*)ws->function("N_{#varUpsilon(2S)}_pp"); //RooFormulaVar *nsig2_pp = (RooFormulaVar*)("N_{ #varUpsilon(2S)}_pp");
   RooFormulaVar *nsig3_pp = (RooFormulaVar*)ws->function("N_{#varUpsilon(3S)}_pp"); //RooFormulaVar *nsig3_pp = (RooFormulaVar*)("N_{ #varUpsilon(3S)}_pp");

   //RooRealVar* nsig2_pp = ws->var("N_{#Upsilon(2S)}_pp");
   // RooRealVar* nsig3_pp = ws->var("N_{#Upsilon(3S)}_pp");
   RooRealVar* norm_nbkg_pp = ws->var("n_{Bkgd}_pp");

   RooArgList pdfs_pp( *sig1S_pp,*sig2S_pp,*sig3S_pp, *LSBackground_pp);
   RooArgList norms_pp( *nsig1_pp,*nsig2_pp,*nsig3_pp,*norm_nbkg_pp);

   RooAddPdf model_num("model_num", "model_num", pdfs_hi,norms_hi); 
   ws->import(model_num);
   ws->factory("PROD::model_hi(model_num, constr_nbkg_hi)");

   RooAddPdf model_den("model_den", "model_den", pdfs_pp,norms_pp); 
   ws->import(model_den);
   ws->factory("PROD::model_pp(model_den, constr_nbkg_pp)");

   ws->factory("SIMUL::joint(dataCat,hi=model_hi,pp=model_pp)");

*/

   /////////////////////////////////////////////////////////////////////
   RooRealVar * pObs = ws->var("invariantMass"); // get the pointer to the observable
   RooArgSet obs("observables");
   obs.add(*pObs);
   obs.add( *ws->cat("dataCat"));    
   //  /////////////////////////////////////////////////////////////////////
   //ws->var("glob_lumipp")->setConstant(true);
   //ws->var("glob_Taa")->setConstant(true);
   //ws->var("glob_effRat")->setConstant(true);
   ws->var("glob_syst")->setConstant(true);
   RooArgSet globalObs("global_obs");
   //globalObs.add( *ws->var("glob_lumipp") );
   //globalObs.add( *ws->var("glob_Taa") );
   //globalObs.add( *ws->var("glob_effRat") );
   if(doSyst){globalObs.add( *ws->var("glob_syst") );}

   cout<<"00000000000000000000"<<endl;
   // ws->Print();

   RooArgSet poi("poi");
   cout<<"111111"<<endl;
   poi.add( *ws->var("DR31") );
   cout<<"222222"<<endl;



   // create set of nuisance parameters
   RooArgSet nuis("nuis");
   cout<<"333333"<<endl;
   //nuis.add( *ws->var("beta_lumipp") );
   //cout<<"444444"<<endl;
   if(doSyst){nuis.add( *ws->var("beta_syst") );}
   //cout<<"777777"<<endl;
   //nuis.add( *ws->var("beta_effRat") );
   //cout<<"888888"<<endl;

   ws->var("#alpha_{CB}_hi")->setConstant(true);
   ws->var("#alpha_{CB}_pp")->setConstant(true);
   ws->var("#sigma_{CB1}_hi")->setConstant(true);
   ws->var("#sigma_{CB1}_pp")->setConstant(true);
   ws->var("#sigma_{CB2}/#sigma_{CB1}_hi")->setConstant(true);
   ws->var("#sigma_{CB2}/#sigma_{CB1}_pp")->setConstant(true);
//   ws->var("Centrality")->setConstant(true);
   ws->var("N_{#varUpsilon(1S)}_hi")->setConstant(true);
   ws->var("N_{#varUpsilon(1S)}_pp")->setConstant(true);
   //ws->var("R_{#frac{2S}{1S}}_hi")->setConstant(true);
   ws->var("R_{#frac{2S}{1S}}_pp")->setConstant(true);
   ws->var("R_{#frac{3S}{1S}}_pp")->setConstant(true);
   ws->var("R_{#frac{2S}{1S}}_hi")->setConstant(true);
//   ws->var("N_{#Upsilon(2S)}_hi")->setConstant(true);
//   ws->var("N_{#Upsilon(2S)}_pp")->setConstant(true);
//   ws->var("N_{#Upsilon(3S)}_pp")->setConstant(true);
   //ws->var("Nmb_hi")->setConstant(true);
   // ws->var("QQsign")->setConstant(true);
   //ws->var("Taa_hi")->setConstant(true);
   //ws->var("Taa_kappa")->setConstant(true);
   // ws->var("beta_Taa")->setConstant(true);
   // ws->var("beta_effRat")->setConstant(true);
   // ws->var("beta_lumipp")->setConstant(true);
   // ws->var("beta_nbkg_hi")->setConstant(true);
   // ws->var("beta_nbkg_pp")->setConstant(true);
   // ws->var("dataCat")->setConstant(true);
   ws->var("decay_hi")->setConstant(true);
   ws->var("decay_pp")->setConstant(true);
   //ws->var("effRat3_hi")->setConstant(true);
   //ws->var("effRat_kappa")->setConstant(true);
   // ws->var("glob_Taa")->setConstant(true);
   // ws->var("glob_effRat")->setConstant(true);
   // ws->var("glob_lumipp")->setConstant(true);
   // ws->var("glob_nbkg_hi")->setConstant(true);
   // ws->var("glob_nbkg_pp")->setConstant(true);
   // ws->var("invariantMass")->setConstant(true);
   ws->var("leftEdge")->setConstant(true);
   //ws->var("lumipp_hi")->setConstant(true);
   //ws->var("lumipp_kappa")->setConstant(true);
   ws->var("m_{ #varUpsilon(1S)}_hi")->setConstant(true);
   ws->var("m_{ #varUpsilon(1S)}_pp")->setConstant(true);
   ws->var("muMinusPt")->setConstant(true);
   ws->var("muPlusPt")->setConstant(true);
   ws->var("n_{Bkgd}_hi")->setConstant(true);
   ws->var("n_{Bkgd}_pp")->setConstant(true);
   ws->var("kappa_syst")->setConstant(true);
   ws->var("n_{CB}_hi")->setConstant(true);
   ws->var("n_{CB}_pp")->setConstant(true);
   //ws->var("npow")->setConstant(true);
   // ws->var("raa3")->setConstant(true);
   ws->var("rightEdge")->setConstant(true);
   ws->var("sigmaFraction_hi")->setConstant(true);
   ws->var("sigmaFraction_pp")->setConstant(true);
   ws->var("turnOn_hi")->setConstant(true);
   ws->var("turnOn_pp")->setConstant(true);
   ws->var("dimuPt")->setConstant(true); //ws->var("upsPt")->setConstant(true);
   ws->var("dimuRapidity")->setConstant(true); //ws->var("upsRapidity")->setConstant(true);
   //ws->var("vProb")->setConstant(true);
   ws->var("width_hi")->setConstant(true);
   ws->var("width_pp")->setConstant(true);
   // ws->var("x3raw")->setConstant(true);
   //  RooArgSet fixed_again("fixed_again");
   //  fixed_again.add( *ws->var("leftEdge") );
   //  fixed_again.add( *ws->var("rightEdge") );
   //  fixed_again.add( *ws->var("Taa_hi") );
   //  fixed_again.add( *ws->var("Nmb_hi") );
   //  fixed_again.add( *ws->var("lumipp_hi") );
   //  fixed_again.add( *ws->var("effRat1_hi") );
   //  fixed_again.add( *ws->var("effRat2_hi") );
   //  fixed_again.add( *ws->var("effRat3_hi") );
   //  fixed_again.add( *ws->var("nsig3_pp") );
   //  fixed_again.add( *ws->var("nsig1_pp") );
   //  fixed_again.add( *ws->var("nbkg_hi") );
   //  fixed_again.add( *ws->var("alpha") );
   //  fixed_again.add( *ws->var("nbkg_kappa") );
   //  fixed_again.add( *ws->var("Taa_kappa") );
   //  fixed_again.add( *ws->var("lumipp_kappa") );
   // fixed_again.add( *ws->var("mean_hi") );
   // fixed_again.add( *ws->var("mean_pp") );
   // fixed_again.add( *ws->var("width_hi") );
   // fixed_again.add( *ws->var("turnOn_hi") );
   // fixed_again.add( *ws->var("bkg_a1_pp") );
   // fixed_again.add( *ws->var("bkg_a2_pp") );
   // fixed_again.add( *ws->var("decay_hi") );
   // fixed_again.add( *ws->var("raa1") );
   // fixed_again.add( *ws->var("raa2") );
   //  fixed_again.add( *ws->var("nsig2_pp") );
   // fixed_again.add( *ws->var("sigma1") );
   //  fixed_again.add( *ws->var("nbkg_pp") );
   // fixed_again.add( *ws->var("npow") );
   // fixed_again.add( *ws->var("muPlusPt") );
   // fixed_again.add( *ws->var("muMinusPt") );
   // fixed_again.add( *ws->var("mscale_hi") );
   // fixed_again.add( *ws->var("mscale_pp") );
   //  
   // ws->Print();

   // create signal+background Model Config
   RooStats::ModelConfig sbHypo("SbHypo");
   sbHypo.SetWorkspace( *ws );
   sbHypo.SetPdf( *ws->pdf("joint") );
   sbHypo.SetObservables( obs );
   sbHypo.SetGlobalObservables( globalObs );
   sbHypo.SetParametersOfInterest( poi );
   sbHypo.SetNuisanceParameters( nuis );
   sbHypo.SetPriorPdf( *ws->pdf("step") ); // this is optional

   // ws->Print();
   /////////////////////////////////////////////////////////////////////
   RooAbsReal * pNll = sbHypo.GetPdf()->createNLL( *data,NumCPU(25) );
   RooMinuit(*pNll).migrad(); // minimize likelihood wrt all parameters before making plots
   RooPlot *framepoi = ((RooRealVar *)poi.first())->frame(Bins(10),Range(0.,.2),Title("LL and profileLL in DR31"));
   pNll->plotOn(framepoi,ShiftToZero());
   
   RooAbsReal * pProfile = pNll->createProfile( globalObs ); // do not profile global observables
   pProfile->getVal(); // this will do fit and set POI and nuisance parameters to fitted values
   pProfile->plotOn(framepoi,LineColor(kRed));
   framepoi->SetMinimum(0);
   framepoi->SetMaximum(3);
   TCanvas *cpoi = new TCanvas();
   cpoi->cd(); framepoi->Draw();
   cpoi->SaveAs("cpoi.pdf");

   ((RooRealVar *)poi.first())->setMin(0.);
   RooArgSet * pPoiAndNuisance = new RooArgSet("poiAndNuisance");
   // pPoiAndNuisance->add(*sbHypo.GetNuisanceParameters());
   // pPoiAndNuisance->add(*sbHypo.GetParametersOfInterest());
   pPoiAndNuisance->add( nuis );
   pPoiAndNuisance->add( poi );
   sbHypo.SetSnapshot(*pPoiAndNuisance);

   RooPlot* xframeSB = pObs->frame(Title("SBhypo"));
   data->plotOn(xframeSB,Cut("dataCat==dataCat::hi"));
   RooAbsPdf *pdfSB = sbHypo.GetPdf();
   RooCategory *dataCat = ws->cat("dataCat");
   pdfSB->plotOn(xframeSB,Slice(*dataCat,"hi"),ProjWData(*dataCat,*data));
   TCanvas *c1 = new TCanvas();
   c1->cd(); xframeSB->Draw();
   c1->SaveAs("c1.pdf");

   delete pProfile;
   delete pNll;
   delete pPoiAndNuisance;
   ws->import( sbHypo );
   /////////////////////////////////////////////////////////////////////
   RooStats::ModelConfig bHypo = sbHypo;
   bHypo.SetName("BHypo");
   bHypo.SetWorkspace(*ws);
   pNll = bHypo.GetPdf()->createNLL( *data,NumCPU(4) );
   RooArgSet poiAndGlobalObs("poiAndGlobalObs");
   poiAndGlobalObs.add( poi );
   poiAndGlobalObs.add( globalObs );
   pProfile = pNll->createProfile( poiAndGlobalObs ); // do not profile POI and global observables
   ((RooRealVar *)poi.first())->setVal( 0 );  // set raa3=0 here
   pProfile->getVal(); // this will do fit and set nuisance parameters to profiled values
   pPoiAndNuisance = new RooArgSet( "poiAndNuisance" );
   pPoiAndNuisance->add( nuis );
   pPoiAndNuisance->add( poi );
   bHypo.SetSnapshot(*pPoiAndNuisance);

   RooPlot* xframeB = pObs->frame(Title("Bhypo"));
   data->plotOn(xframeB,Cut("dataCat==dataCat::hi"));
   RooAbsPdf *pdfB = bHypo.GetPdf();
   pdfB->plotOn(xframeB,Slice(*dataCat,"hi"),ProjWData(*dataCat,*data));
   TCanvas *c2 = new TCanvas();
   c2->cd(); xframeB->Draw();
   c2->SaveAs("c2.pdf");

   delete pProfile;
   delete pNll;
   delete pPoiAndNuisance;

   // import model config into workspace
   bHypo.SetWorkspace(*ws);
   ws->import( bHypo );
   /////////////////////////////////////////////////////////////////////
   ws->Print();
   bHypo.Print();
   sbHypo.Print();

   // save workspace to file
   ws -> SaveAs(name_out);

   return;
}




