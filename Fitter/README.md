# Current Fitter Code

This code is to be used to fit OniaTrees for Upsilon fits in 8-14 mass range.  

1. How to run!
    * .L doFit.C
    * doFit(TString FileName = "OniaTree.root", bool isPbPb = true, bool doPt = false, bool doRap = false, bool doCent = false, bool doPeriph = false)
    * For example if you want integrated doPt && doRap && doCent && doPeriph all need to be false
    * To do the Centrality 0-30% fitting you need to only set doCent = true
    * To do the Centrality 30-80% fitting you need to set doCent = true && doPeriph = true
    
    
    This code will save the plots and workspaces in diretories as follows:
     * Plots_date
          * /PT
          * /Integrated
          * /Cent
          * /Rap
  
