
Efficiency for dimuons

1. making efficiency
  * root -l 
  * .L dimuEff.C+
  * dimuEff(oniaMode,isPbPb) //oniaMode = 1 for 1S and 2 for 2S and isPbPb = true for PbPb and false for pp

2. making single ratios
  * root -l
  * .L singleRatioEff.C+
  * singleRatioEff(isPbPb) //isPbPb = true for PbPb and false for pp
  
3. making double ratios
  * root -l doubleRatioEff.C
