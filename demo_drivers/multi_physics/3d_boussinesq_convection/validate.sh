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

# Validation for demo poisson
#----------------------------
cd Validation

echo "Running 3D Boussinesq Convection validation"
mkdir RESLT
../3d_ref_b_convect > OUTPUT_3d_boussinesq_convection
echo "done"
echo " " >> validation.log
echo "3D Boussinesq Convection validation" >> validation.log
echo "-----------------------------------------------------------" >> validation.log
echo " " >> validation.log
echo "Validation directory: " >> validation.log
echo " " >> validation.log
echo "  " `pwd` >> validation.log
echo " " >> validation.log
cat RESLT/soln0.dat > 3d_conv.dat


if test "$1" = "no_fpdiff"; then
  echo "dummy [OK] -- Can't run fpdiff.py because we don't have python or validata" >> validation.log
else
../../../../bin/fpdiff.py ../validata/3d_conv.dat.gz   \
    3d_conv.dat 0.1 1.0e-10  >> validation.log
fi

mv RESLT RESLT_3d_conv



echo "Running 3D Boussinesq Convection (multimesh) validation"
mkdir RESLT
../3d_multimesh_ref_b_convect > OUTPUT_3d_multimesh_boussinesq_convection
echo "done"
echo " " >> validation.log
echo "3D Boussinesq Convection validation (multimesh)" >> validation.log
echo "------------------------------------------------------------" >> validation.log
echo " " >> validation.log
echo "Validation directory: " >> validation.log
echo " " >> validation.log
echo "  " `pwd` >> validation.log
echo " " >> validation.log
cat RESLT/soln0.dat  > 3d_multi_conv.dat



if test "$1" = "no_fpdiff"; then
  echo "dummy [OK] -- Can't run fpdiff.py because we don't have python or validata" >> validation.log
else
../../../../bin/fpdiff.py ../validata/3d_multi_conv.dat.gz   \
    3d_multi_conv.dat 0.1 1.0e-10  >> validation.log
fi

mv RESLT RESLT_3d_multi_conv

# Append output to global validation log file
#--------------------------------------------
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
