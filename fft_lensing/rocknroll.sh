make clean 
make 


NCC=1024
#BSZ=20 #Mpc/h
#BSZ=791 #arcsec # 3.0 Mpc/h
#BSZ=663.31 #arcsec # 2.5 Mpc/h
BSZ=530.65 #arcsec # 2.5 Mpc/h
#BSZ=8437.83 #arcsec # 32 Mpc/h
#BSZ=263.68 #arcsec # 3.0 Mpc/h
ALX=0.0
ALY=0.0
ALZ=0.0
ZZL=0.396 # g = shear/(1-kappa)
ZZS=1.0

./cal_alphas ../../lenses/data/${1}.bin ${NCC} ${BSZ} ${ZZL} ${ZZS} \
      ../../lenses/output/${1}_posx1.bin \
      ../../lenses/output/${1}_posx2.bin \
      ../../lenses/output/${1}_alpha1.bin \
      ../../lenses/output/${1}_alpha2.bin \
      ../../lenses/output/${1}_shear1.bin \
      ../../lenses/output/${1}_shear2.bin \
      ../../lenses/output/${1}_kappa.bin \
      ../../lenses/output/${1}_mu.bin 

