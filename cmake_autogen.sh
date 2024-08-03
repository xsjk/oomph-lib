#! /bin/bash


# Crash if any sub command crashes
set -o errexit

# Crash if any unset variables are used
set -o nounset


# Load helper functions
source scripts/autogen_helpers.sh

# hierher read in configure options
mpi_flag="OFF"
mpi_flag="ON"

# Paranoia
paranoia_flag="ON"
paranoia_flag="OFF"

# Range checking
range_checking_flag="ON"
range_checking_flag="OFF"

# build type
build_type="Debug"
build_type="Release"

# hierher check that we're in the right directory


# Check if build directories exist and offer to wipe them
build_dir_list="external_distributions/build build demo_drivers/build"
for build_dir in `echo $build_dir_list`; do
    if [ -e  $build_dir ]; then
        absolute_build_dir=`pwd`"/"$build_dir
        echo "Build directory: "
        echo " "
        echo "    "$absolute_build_dir
        echo " "
        echo "already exists."
        if YesNoRead "Do you want to wipe it" "n"; then
            echo " "
            echo "Sorry to be over-cautious here, but we'd better double-check"
            echo "before we delete some of your precious files..."
            echo " "
            echo "The contents of " $absolute_build_dir " are:"
            echo " "
            ls -l  $absolute_build_dir
            echo " "
            if YesNoRead "Are you still sure you want to wipe it" "n"; then
                echo " "
                echo "Wiping it..."
                rm -f -r $absolute_build_dir
                echo "Done"
            fi
        fi
    fi
done


# Build external distributions (all of them)
cd external_distributions/
cmake -G Ninja -B build -DOOMPH_ENABLE_MPI=$mpi_flag # Add other flags
cmake --build build

# Build oomph-lib itself
cd ..
cmake -G Ninja $(cat external_distributions/build/cmake_flags_for_oomph_lib.txt) -DOOMPH_ENABLE_PARANOID=$paranoia_flag -DOOMPH_ENABLE_RANGE_CHECKING=$range_checking_flag -DCMAKE_BUILD_TYPE=$build_type -B build 
cmake --build build
cmake --install build

# Build/run self tests
cd demo_drivers
cmake -G Ninja -B build -DOOMPH_ENABLE_PARANOID=$paranoia_flag -DOOMPH_ENABLE_RANGE_CHECKING=$range_checking_flag -DCMAKE_BUILD_TYPE=$build_type 
cd build/
ctest -j8

