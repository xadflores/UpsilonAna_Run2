
#!/bin/bash

# dir
dir2=WS

#for cat in 0cent20 20cent60 60cent100 100cent160 ; do 
cat=('0cent20' '20cent60' '60cent100' '100cent200' '0cent200')
sys=(.306 1.51E5 0.1476 0.388 0.726)


for i in 0 1 2 3 4; do 
   ppfile=`ls $dir2/FIT*PP*root`
   pbpbfile=`ls $dir2/FIT*PbPb*_${cat[i]}_*root`
   nice root -l -b -q Raa3S_Workspace.C\(\"${pbpbfile}\"\,\"${ppfile}\"\,\"$dir2/WS_combo3S_${cat[i]}.root\"\,${sys[i]}\)
   mv c1.pdf $dir2/c1_${cat[i]}.pdf
   mv c2.pdf $dir2/c2_${cat[i]}.pdf
   mv cpoi.pdf $dir2/cpoi_${cat[i]}.pdf
done
