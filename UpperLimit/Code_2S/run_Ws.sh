
#!/bin/bash

# dir
dir2=WS

#for cat in 0cent20 20cent60 60cent100 100cent160 0cent160; do 
for cat in 0cent10; do 
   ppfile=`ls $dir2/FIT*PP*root`
   pbpbfile=`ls $dir2/FIT*PbPb*$cat*root`
   nice root -l -b -q Raa2S_Workspace.C\(\"${pbpbfile}\"\,\"${ppfile}\"\,\"$dir2/WS_combo2S_${cat}.root\"\)
   mv c1.pdf $dir2/c1_${cat}.pdf
   mv c2.pdf $dir2/c2_${cat}.pdf
   mv cpoi.pdf $dir2/cpoi_${cat}.pdf
done
