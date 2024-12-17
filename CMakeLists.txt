cmake_minimum_required(VERSION 3.14)
project(oomph-lib VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-O3 -Wall)

find_package(MPI REQUIRED)

add_library(oomph INTERFACE)

set(OOMPH_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/build)

target_include_directories(oomph
    INTERFACE
        $<BUILD_INTERFACE:${OOMPH_PATH}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_directories(oomph
    INTERFACE
        $<BUILD_INTERFACE:${OOMPH_PATH}/lib>
        $<INSTALL_INTERFACE:lib>
)

target_link_libraries(oomph
    INTERFACE
        # oomph-lib libraries
        advection_diffusion
        advection_diffusion_reaction
        axisym_advection_diffusion
        axisym_displ_based_foeppl_von_karman
        axisym_foeppl_von_karman
        axisym_linear_elasticity
        axisym_navier_stokes
        axisym_poroelasticity
        axisym_spherical_solid
        beam
        biharmonic
        constitutive
        darcy
        fluid_interface
        flux_transport
        foeppl_von_karman
        fourier_decomposed_helmholtz
        generalised_newtonian_axisym_navier_stokes
        generalised_newtonian_navier_stokes
        helmholtz
        jack_cool
        linear_elasticity
        linear_wave
        linearised_axisym_navier_stokes
        linearised_navier_stokes
        mesh_smoothing
        meshes
        multi_physics
        navier_stokes
        ode
        pml_fourier_decomposed_helmholtz
        pml_helmholtz
        pml_time_harmonic_linear_elasticity
        poisson
        polar_navier_stokes
        poroelasticity
        rigid_body
        shell
        solid
        space_time_block_preconditioner
        space_time_navier_stokes
        space_time_unsteady_heat_equal_order_galerkin
        space_time_unsteady_heat_equal_order_galerkin_petrov
        space_time_unsteady_heat_mixed_order_galerkin_petrov
        spherical_advection_diffusion
        spherical_navier_stokes
        steady_axisym_advection_diffusion
        time_harmonic_fourier_decomposed_linear_elasticity
        time_harmonic_linear_elasticity
        unsteady_heat
        womersley
        young_laplace

        # oomph-lib dependencies
        generic
        oomph_hsl
        oomph_arpack
        oomph_crbond_bessel
        oomph_triangle
        oomph_tetgen
        oomph_superlu_4.3
        oomph_parmetis_3.1.1
        oomph_superlu_dist_3.0
        oomph_metis_from_parmetis_3.1.1
        oomph_lapack
        oomph_flapack
        oomph_blas

        # additional dependencies
        MPI::MPI_CXX
        open-rte
        open-pal
        hwloc
        event_core
        event_pthreads
        gfortran
        m
        z
        quadmath
)

target_compile_definitions(oomph
    INTERFACE
        -DOOMPH_HAS_MPI
        -DOOMPH_HAS_STACKTRACE
        -DOOMPH_HAS_UNISTDH
        -DOOMPH_HAS_FPUCONTROLH
        -DOOMPH_HAS_MALLOCH
        -DOOMPH_HAS_TRIANGLE_LIB
        -DOOMPH_HAS_TETGEN_LIB
        -DUSING_OOMPH_SUPERLU
        -DUSING_OOMPH_SUPERLU_DIST
        -DgFortran
)

install(DIRECTORY ${OOMPH_PATH}/include/ DESTINATION include)
install(DIRECTORY ${OOMPH_PATH}/lib/ DESTINATION lib)

export(TARGETS oomph FILE oomphTargets.cmake)

install(TARGETS oomph EXPORT oomphTargets)

install(EXPORT oomphTargets
    FILE oomphTargets.cmake
    NAMESPACE oomph::
    DESTINATION lib/cmake/oomph
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/oomphConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/oomphConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/oomphConfig.cmake"
    INSTALL_DESTINATION lib/cmake/oomph
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/oomphConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/oomphConfigVersion.cmake"
    DESTINATION lib/cmake/oomph
)
