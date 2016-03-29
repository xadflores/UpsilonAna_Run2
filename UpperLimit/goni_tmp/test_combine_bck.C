#include "functions.C"
#include "RooGlobalFunc.h"
#include "RooWorkspace.h"

using namespace RooFit; //new

//int test_combine_bck(const char* name_pbpb="chad_ws_fits/centFits/ws_PbPbData_262548_263757_0cent10_0.0pt50.0_0.0y2.4.root", const char* name_pp="chad_ws_fits/centFits/ws_PPData_262157_262328_-1cent1_0.0pt50.0_0.0y2.4.root")
RooWorkspace* test_combine_bck(const char* name_pbpb="chad_ws_fits/centFits/ws_PbPbData_262548_263757_10cent20_0.0pt50.0_0.0y2.4.root", const char* name_pp="chad_ws_fits/centFits/ws_PPData_262157_262328_-1cent1_0.0pt50.0_0.0y2.4.root")
{
  // const char *poiname="N_{#Upsilon(3S)}";
  TFile *f = new TFile(name_pbpb) ;
  TFile *f_pp = new TFile(name_pp) ;

  // Retrieve workspace from file
  RooWorkspace* ws = (RooWorkspace*) f->Get("myws"); //change workspace name
  RooWorkspace* ws_pp = (RooWorkspace*) f_pp->Get("myws"); //change workspace name


  // RooRealVar *theVar; 
  RooDataSet *data; RooAbsPdf *pdf;
  RooRealVar *theVar_pp; RooDataSet *data_pp; RooAbsPdf *pdf_pp;

  // theVar = ws->var(poiname);
  // pdf = ws->pdf("pdf");
  data = (RooDataSet *) ws->data("dataOS"); //dataOS, dataSS
  pdf_pp = ws_pp->pdf("pdf");
  data_pp = (RooDataSet *) ws_pp->data("dataOS"); //dataOS, dataSS

  RooCategory dataCat("dataCat", "dataCat");
  dataCat.defineType("hi");
  dataCat.defineType("pp");

  RooRealVar muppt("muPlusPt" ,"#mu+ pt",2,20,"GeV/c"); 
  RooRealVar mumpt("muMinusPt","#mu- pt",2,20,"GeV/c"); 
  RooRealVar dimuPt("dimuPt","p_{T}(#DiMuon)",0.,"GeV/c"); //RooRealVar upsPt("upsPt","p_{T}(#Upsilon)",0.,"GeV/c");
  RooRealVar vProb("vProb","vProb",0.001,1);
  //RooRealVar upsEta("upsEta","#eta(#Upsilon)",0.,"");
  RooRealVar dimuRapidity("dimuRapidity", "dimuRapidity", 0.); //RooRealVar upsRapidity("upsRapidity", "upsRapidity", 0.);
  RooCategory QQsign("QQsign", "QQsign");
  QQsign.defineType("PlusMinus", 0);
  QQsign.defineType("PlusPlus", 1);
  QQsign.defineType("MinusMinus", 2);
  RooRealVar Centrality("Centrality", "Centrality", 0., 200);
  RooRealVar * mass = ws->var("invariantMass");
  if (!mass) {
    mass = new RooRealVar("invariantMass", "#mu#mu mass", mmin, mmax, "GeV/c^{2}");
  }

  RooArgSet cols(*mass, muppt, mumpt, dimuPt, vProb, dimuRapidity, QQsign, Centrality);

  RooDataSet data_combo("data", "data", cols, RooFit::Index(dataCat), RooFit::Import("hi", *data), RooFit::Import("pp", *data_pp));
  RooWorkspace *wcombo = new RooWorkspace("wcombo","workspace for PbPb + pp");
  wcombo->import(data_combo);
  wcombo->import(*pdf_pp, RooFit::RenameAllNodes("pp"),
      RooFit::RenameAllVariablesExcept("pp", 
//        "n_{CB},invariantMass,"//npow->n_{CB}
//        "f23,f3o2,"
        "x23,x3o2,"
//        "alpha,"
//        "sigma1"
        ), 
      RooFit::RecycleConflictNodes());

  RooAbsPdf *sig1S = ws->pdf("sig1S"); //RooAbsPdf *sig1S = ws->pdf("cbcb");
  RooAbsPdf *sig2S = ws->pdf("sig2S");
  RooAbsPdf *sig3S = ws->pdf("sig3S");
  RooAbsPdf *pdf_combinedbkgd = ws->pdf("bkgPdf");
  RooRealVar *nsig1f = ws->var("N_{#varUpsilon(1S)}"); //RooRealVar *nsig1f = ws->var("N_{#Upsilon(1S)}");
  RooRealVar *nsig2f = ws->var("R_{#frac{2S}{1S}}"); //RooRealVar *nsig2f = ws->var("N_{#Upsilon(2S)}");
  RooRealVar *nsig3f = ws->var("R_{#frac{3S}{1S}}"); //RooRealVar *nsig3f = ws->var("N_{#Upsilon(3S)}");
  RooRealVar *nbkgd = ws->var("n_{Bkgd}");

  RooAbsPdf *pdf_new = new RooAddPdf ("pdf","new total p.d.f.",
      RooArgList(*sig1S,*sig2S,*sig3S,*pdf_combinedbkgd),
      RooArgList(*nsig1f,*nsig2f,*nsig3f,*nbkgd));
  wcombo->import(*pdf_new, RooFit::RenameAllNodes("hi"),
      RooFit::RenameAllVariablesExcept("hi", 
//       "n_{CB},invariantMass,"
        "x23,x3o2,"
//        "alpha,"
//        "sigma1,"
//        "x3raw,N_{#Upsilon(3S)}_pp"
        ), 
      RooFit::RecycleConflictNodes());


  RooSimultaneous* simPdf = buildSimPdf(*wcombo,dataCat);
  
  RooFitResult* fit_2nd;// fit results
  fit_2nd = simPdf->fitTo(data_combo,
      RooFit::Save(kTRUE),
      RooFit::Extended(kFALSE),
      RooFit::Minos(kTRUE),
      RooFit::NumCPU(30));

      
  wcombo->var("#alpha_{CB}_hi")->setConstant(true);
  wcombo->var("#alpha_{CB}_pp")->setConstant(true);
  wcombo->var("#sigma_{CB1}_hi")->setConstant(true);
  wcombo->var("#sigma_{CB1}_pp")->setConstant(true);
  wcombo->var("#sigma_{CB2}/#sigma_{CB1}_hi")->setConstant(true);
  wcombo->var("#sigma_{CB2}/#sigma_{CB1}_pp")->setConstant(true);
  wcombo->var("N_{#varUpsilon(1S)}_hi")->setConstant(true);
  wcombo->var("N_{#varUpsilon(1S)}_pp")->setConstant(true);
  wcombo->var("R_{#frac{2S}{1S}}_hi")->setConstant(true); //new
  wcombo->var("R_{#frac{2S}{1S}}_pp")->setConstant(true); //new
  wcombo->var("R_{#frac{3S}{1S}}_hi")->setConstant(true); //new
  wcombo->var("R_{#frac{3S}{1S}}_pp")->setConstant(true); //new
  wcombo->var("decay_hi")->setConstant(true);
  wcombo->var("decay_pp")->setConstant(true);
 // wcombo->var("mass1S_hi")->setConstant(true);
 // wcombo->var("mass1S_pp")->setConstant(true);
  wcombo->var("n_{Bkgd}_hi")->setConstant(true);
  wcombo->var("n_{Bkgd}_pp")->setConstant(true);
//  wcombo->var("n_{CB}")->setConstant(true); //wcombo->var("npow")->setConstant(true);
  wcombo->var("n_{CB}_hi")->setConstant(true); //wcombo->var("npow")->setConstant(true);
  wcombo->var("n_{CB}_pp")->setConstant(true); //wcombo->var("npow")->setConstant(true);
  wcombo->var("sigmaFraction_hi")->setConstant(true);
  wcombo->var("sigmaFraction_pp")->setConstant(true);
  wcombo->var("turnOn_hi")->setConstant(true);
  wcombo->var("turnOn_pp")->setConstant(true);
  wcombo->var("width_hi")->setConstant(true);
  wcombo->var("width_pp")->setConstant(true);

  wcombo->writeToFile("fitresult_combo_bck.root");

  return wcombo;
}
