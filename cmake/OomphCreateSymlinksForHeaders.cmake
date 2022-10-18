# cmake-format: off
# =============================================================================
# Create symlinks for headers from the build/include directory to the *actual*
# location of the to-be-installed header files.
#
# NOTE: We have to be careful to use "install(...)" commands only as we can only
# create the actual links at install time.
#
# Usage:
#
#  include(OomphCreateSymlinksForHeaders)
#  oomph_create_symlinks_for_headers(REAL_DIR    <dir-to-symlink-to>
#                                    SYMLINK_DIR <dir-to-symlink-from>
#                                    HEADERS     <headers-to-be-symlinked-to>
# =============================================================================
# cmake-format: on
include_guard()

function(oomph_create_symlinks_for_headers)
  # Define the supported set of keywords
  set(PREFIX ARG)
  set(FLAGS "")
  set(SINGLE_VALUE_ARGS REAL_DIR SYMLINK_DIR)
  set(MULTI_VALUE_ARGS HEADERS)

  # Process the arguments passed in
  include(CMakeParseArguments)
  cmake_parse_arguments(PARSE_ARGV 0 ${PREFIX} "${FLAGS}"
                        "${SINGLE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}")

  # Symlink the headers
  foreach(HEADER ${${PREFIX}_HEADERS})
    get_filename_component(HEADER_PATH ${HEADER} DIRECTORY)

    # If the header is an absolute path (i.e. the DIRECTORY component is
    # non-empty), link it directly. Otherwise, use REAL_DIR as the base
    # directory
    if(HEADER_PATH)
      get_filename_component(HEADER_NAME ${HEADER} NAME)
      set(TARGET "${HEADER}")
      set(LINK "${${PREFIX}_SYMLINK_DIR}/${HEADER_NAME}")
    else()
      set(TARGET "${${PREFIX}_REAL_DIR}/${HEADER}")
      set(LINK "${${PREFIX}_SYMLINK_DIR}/${HEADER}")
    endif()

    # Symlink and print info
    install(
      CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${TARGET} ${LINK})"
    )
    if(DEFINED CMAKE_MESSAGE_LOG_LEVEL)
      if(${CMAKE_MESSAGE_LOG_LEVEL} STREQUAL VERBOSE)
        install(CODE "message(\"-- Created symlink: ${LINK} -> ${TARGET}\")")
      endif()
    endif()
  endforeach()
endfunction()