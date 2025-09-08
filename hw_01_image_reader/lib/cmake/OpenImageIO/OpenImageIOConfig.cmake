# Copyright Contributors to the OpenImageIO project.
# SPDX-License-Identifier: Apache-2.0
# https://github.com/AcademySoftwareFoundation/OpenImageIO


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was Config.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

include(CMakeFindDependencyMacro)

# add here all the find_dependency() whenever switching to config based dependencies
if (NOT OFF AND NOT OPENIMAGEIO_CONFIG_DO_NOT_FIND_IMATH)
    if (2.5.8 VERSION_GREATER_EQUAL 3.0)
        find_dependency(Imath 
                        HINTS Imath_DIR-NOTFOUND)
    elseif (2.5.8 VERSION_GREATER_EQUAL 2.4 AND 0)
        find_dependency(IlmBase 2.5.8
                        HINTS IlmBase_DIR-NOTFOUND OpenEXR_DIR-NOTFOUND)
        find_dependency(OpenEXR 2.5.8
                        HINTS OpenEXR_DIR-NOTFOUND)
        find_dependency(ZLIB 1.2.11)  # Because OpenEXR doesn't do it
        find_dependency(Threads)  # Because OpenEXR doesn't do it
    endif ()
endif ()

if (NOT TRUE AND NOT ON)
    find_dependency(fmt)
endif ()

if (NOT ON)
    # This is required in static library builds, as e.g. PNG::PNG appears among
    # INTERFACE_LINK_LIBRARIES. If the project does not know about PNG target, it will cause
    # configuration error about unknown targets being linked in.
    find_dependency(TIFF)
    if (TRUE)
        find_dependency(JPEG)
    endif()
    if (FALSE)
        find_dependency(PNG)
    endif()
    # The following have the same problem except that INTERFACE_LINK_LIBRARIES use
    # TARGET_NAME_IF_EXISTS, so the error only happens on link time.
    if (TRUE)
        find_dependency(TBB)
    endif ()
endif ()

# Compute the installation prefix relative to this file. Note that cmake files are installed
# to ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} (see OIIO_CONFIG_INSTALL_DIR)
get_filename_component(_CURR_INSTALL_LIBDIR "${CMAKE_CURRENT_LIST_DIR}/../../" ABSOLUTE)
get_filename_component(_ABS_CMAKE_INSTALL_LIBDIR "/home/leftcircle/programming/clemson/cgi/open_source_compiles/OpenImageIO-2.4.17.0/dist/lib" ABSOLUTE)
get_filename_component(_ABS_CMAKE_INSTALL_INCLUDEDIR "/home/leftcircle/programming/clemson/cgi/open_source_compiles/OpenImageIO-2.4.17.0/dist/include" ABSOLUTE)
file(RELATIVE_PATH _INCLUDEDIR_RELATIVE_TO_LIBDIR
     "${_ABS_CMAKE_INSTALL_LIBDIR}" "${_ABS_CMAKE_INSTALL_INCLUDEDIR}")
get_filename_component(_CURR_INSTALL_INCLUDE_DIR
                       "${_CURR_INSTALL_LIBDIR}/${_INCLUDEDIR_RELATIVE_TO_LIBDIR}" ABSOLUTE)

set_and_check (OpenImageIO_INCLUDE_DIR "${_CURR_INSTALL_INCLUDE_DIR}")
set_and_check (OpenImageIO_INCLUDES    "${_CURR_INSTALL_INCLUDE_DIR}")
set_and_check (OpenImageIO_LIB_DIR     "${_CURR_INSTALL_LIBDIR}")
set (OpenImageIO_PLUGIN_SEARCH_PATH    "")

if (NOT 0)
    list (APPEND OpenImageIO_INCLUDES ${IMATH_INCLUDES} ${OPENEXR_INCLUDES})
endif ()

set (OIIO_USING_IMATH_VERSION_MAJOR 2)
set (OIIO_USING_IMATH_VERSION_MINOR 5)

#...logic to determine installedPrefix from the own location...
#set (OpenImageIO_CONFIG_DIR  "${installedPrefix}/")

include ("${CMAKE_CURRENT_LIST_DIR}/OpenImageIOTargets.cmake")

check_required_components ("OpenImageIO")

# Set a CMake variable that says if this OpenImageIO build has OCIO support
set (OpenImageIO_HAS_OpenColorIO 0)
