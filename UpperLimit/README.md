# Upper Limit Code

This code is to be used to estimate the upperlimit of the double ratio of the Y(3S) and Y(2S). It is derived code from the HIN-15001 git repo which can be found here: https://github.com/nfilipov/HIN-15-001/tree/master/upperlimit/upperlimit 

Code_2S and Code_3S:
It currently works on lxplus at CERN.
1. source setup.sh
    * This will set your computing environment.
    
2. source run_Ws.sh
    * Combines the pp and PbPb workspaces in for ex. directory Code_3S/WS FIT_PP.root and FIT_PbPb.root in one workspace.
  
    
2. source run_Limit.sh
    * Need to set it to be 68 or 95 in the script first. 
    * Neet to change in StandardHypoTestInvDemo.C and runLimit_RaaNS_Workspace.C you must set SetConfidenceLevel and CI to either 0.68 or 0.95 depending on your requirements. This can be all automated of course but yeah.


  

Results_XXXXXXXX:
There are directories for the 2S and 3S. The 09152016 ones are the numbers in the paper and AN. The 12142016 are done with allowing the DR value and left edge/right edge to fluctuate between [-2,2] instead of [0,1] as a cross check. 



