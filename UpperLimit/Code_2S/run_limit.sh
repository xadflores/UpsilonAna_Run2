#!/bin/bash

# dir
dir=WS


dir2=WS/Asym
cl="68"

for cat in 0cent10 ; do                         
   nice root -l -b -q runLimit_RaaNS_Workspace.C'("'${dir}'/WS_combo2S_'${cat}'.root","DR21")'
   mv results.txt $dir2/results_2S_${cl}_${cat}.txt
   mv c1.pdf $dir2/c1_2S_${cl}_${cat}.pdf
   mv c1.root $dir2/c1_2S_${cl}_${cat}.root
   mv c2.pdf $dir2/c2_2S_${cl}_${cat}.pdf
   mv c2.root $dir2/c2_2S_${cl}_${cat}.root
done
