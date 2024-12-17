#! /bin/sh

# Get the OOPMH-LIB root directory from a makefile
OOMPH_ROOT_DIR=$(make -s --no-print-directory print-top_builddir)


#Set the number of tests to be checked
NUM_TESTS=2


# Setup validation directory
#---------------------------
touch Validation
rm -r -f Validation
mkdir Validation

# Validation for spherical Couette flow
#--------------------------------
cd Validation

echo "Running Steady Spherical Rigid-Body Rotation validation "
mkdir RESLT
cd RESLT
../../steady_rot > ../OUTPUT_steady_rot
cd ..
echo "done"
echo " " >> validation.log
echo "Steady spherical rigid-body rotation validation" >> validation.log
echo "---------------------------------------------" >> validation.log
echo " " >> validation.log
echo "Validation directory: " >> validation.log
echo " " >> validation.log
echo "  " `pwd` >> validation.log
echo " " >> validation.log
cat  RESLT/soln_CR_8x8_10.dat RESLT/trace_CR.dat > sph_rot_CR.dat
cat  RESLT/soln_TH_8x8_10.dat RESLT/trace_TH.dat > sph_rot_TH.dat

if test "$1" = "no_fpdiff"; then
  echo "dummy [OK] -- Can't run fpdiff.py because we don't have python or validata" >> validation.log
else
echo "Crouzeix-Raviart elements" >> validation.log
echo " " >> validation.log
../../../../bin/fpdiff.py ../validata/sph_rot_CR.dat.gz  \
         sph_rot_CR.dat 0.1 1.0e-12 >> validation.log
echo "Taylor-Hood elements" >> validation.log
echo " " >> validation.log
../../../../bin/fpdiff.py ../validata/sph_rot_TH.dat.gz  \
         sph_rot_TH.dat 0.1 1.0e-12 >> validation.log

fi


# Append log to main validation log
cat validation.log >> ../../../../validation.log

cd ..



#######################################################################


#Check that we get the correct number of OKs
# validate_ok_count will exit with status
# 0 if all tests has passed.
# 1 if some tests failed.
# 2 if there are more 'OK' than expected.
. $OOMPH_ROOT_DIR/bin/validate_ok_count

# Never get here
exit 10
