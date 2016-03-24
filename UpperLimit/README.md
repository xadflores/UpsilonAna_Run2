# Upper Limit Code

This code is to be used to estimate the upperlimit of the double ratio of the Y(3S). It is derived code from the HIN-15001 git repo which can be found here: https://github.com/nfilipov/HIN-15-001/tree/master/upperlimit/upperlimit 

1. Combine pp and PbPb workspaces obtained from Fit
    * use Raa3S_WorkSpace.C
    * This code includes test_Combine.C
    
2. Run the upper limit algorithm
    * use runLimit_RaaNS_Workspace.C on the new Workspace from 1.)
  
