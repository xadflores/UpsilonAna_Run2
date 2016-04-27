#include "Macros/options.h"

void doFit(TString FileName = "OniaTree.root",bool isPbPb=true,bool doPt=false, bool doRap= false,bool doCent=false,bool doPeriph = false)
{

   int nbin = 0;
   int start = 0;

   bool doUL = true;

   //bunch of ugly stuff to make looping
   if(doPt){nbin = nPtBin;}

   if(doCent){nbin = nCentBin;}
   if(doCent && doPeriph){start = 4; nbin = nCentPerBin;}

   if(doCent && doUL){nbin = 2;}
   if(doCent && doPeriph && doUL){start = 2; nbin = nCentBin;}


   if(doRap){nbin = nRapBin;}

   if(!doPt && !doCent && !doRap){nbin = 1;}
   
   
    
   gROOT->LoadMacro("fit2015.C+");

   for (int j = start;j<nbin;j++){
   fit2015(  FileName,
             true,     // isData = false for MC, true for Data
             isPbPb,    // isPbPb = false for pp, true for PbPb
             true,    // doFit
             true,     // if inExcStat is true, then the excited states are fitted
             j,        //bin
             doPt,     
             doRap,
             doCent,
	     doPeriph,
	     doUL
             );
  }

}


