#!/bin/bash

# dir
dir=WS

# pt fits
dir2=WS/Asym
cl="95"

#for cat in 0cent20 ; do                         
for cat in 0cent20 20cent60 60cent100 100cent200 0cent200; do 
   nice root -l -b -q runLimit_RaaNS_Workspace.C'("'${dir}'/WS_combo3S_'${cat}'.root","DR31")'
   mv results.txt $dir2/results_3S_${cl}_${cat}.txt
   mv c1.pdf $dir2/c1_3S_${cl}_${cat}.pdf
   mv c1.root $dir2/c1_3S_${cl}_${cat}.root
   mv c2.pdf $dir2/c2_3S_${cl}_${cat}.pdf
   mv c2.root $dir2/c2_3S_${cl}_${cat}.root
done
