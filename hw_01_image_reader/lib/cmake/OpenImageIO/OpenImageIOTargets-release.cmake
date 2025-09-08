#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "OpenImageIO::OpenImageIO_Util" for configuration "Release"
set_property(TARGET OpenImageIO::OpenImageIO_Util APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(OpenImageIO::OpenImageIO_Util PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "TBB::tbb"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libOpenImageIO_Util.so.2.4.17"
  IMPORTED_SONAME_RELEASE "libOpenImageIO_Util.so.2.4"
  )

list(APPEND _IMPORT_CHECK_TARGETS OpenImageIO::OpenImageIO_Util )
list(APPEND _IMPORT_CHECK_FILES_FOR_OpenImageIO::OpenImageIO_Util "${_IMPORT_PREFIX}/lib/libOpenImageIO_Util.so.2.4.17" )

# Import target "OpenImageIO::OpenImageIO" for configuration "Release"
set_property(TARGET OpenImageIO::OpenImageIO APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(OpenImageIO::OpenImageIO PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "TBB::tbb"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libOpenImageIO.so.2.4.17"
  IMPORTED_SONAME_RELEASE "libOpenImageIO.so.2.4"
  )

list(APPEND _IMPORT_CHECK_TARGETS OpenImageIO::OpenImageIO )
list(APPEND _IMPORT_CHECK_FILES_FOR_OpenImageIO::OpenImageIO "${_IMPORT_PREFIX}/lib/libOpenImageIO.so.2.4.17" )

# Import target "OpenImageIO::iconvert" for configuration "Release"
set_property(TARGET OpenImageIO::iconvert APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(OpenImageIO::iconvert PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/iconvert"
  )

list(APPEND _IMPORT_CHECK_TARGETS OpenImageIO::iconvert )
list(APPEND _IMPORT_CHECK_FILES_FOR_OpenImageIO::iconvert "${_IMPORT_PREFIX}/bin/iconvert" )

# Import target "OpenImageIO::idiff" for configuration "Release"
set_property(TARGET OpenImageIO::idiff APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(OpenImageIO::idiff PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/idiff"
  )

list(APPEND _IMPORT_CHECK_TARGETS OpenImageIO::idiff )
list(APPEND _IMPORT_CHECK_FILES_FOR_OpenImageIO::idiff "${_IMPORT_PREFIX}/bin/idiff" )

# Import target "OpenImageIO::igrep" for configuration "Release"
set_property(TARGET OpenImageIO::igrep APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(OpenImageIO::igrep PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/igrep"
  )

list(APPEND _IMPORT_CHECK_TARGETS OpenImageIO::igrep )
list(APPEND _IMPORT_CHECK_FILES_FOR_OpenImageIO::igrep "${_IMPORT_PREFIX}/bin/igrep" )

# Import target "OpenImageIO::iinfo" for configuration "Release"
set_property(TARGET OpenImageIO::iinfo APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(OpenImageIO::iinfo PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/iinfo"
  )

list(APPEND _IMPORT_CHECK_TARGETS OpenImageIO::iinfo )
list(APPEND _IMPORT_CHECK_FILES_FOR_OpenImageIO::iinfo "${_IMPORT_PREFIX}/bin/iinfo" )

# Import target "OpenImageIO::maketx" for configuration "Release"
set_property(TARGET OpenImageIO::maketx APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(OpenImageIO::maketx PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/maketx"
  )

list(APPEND _IMPORT_CHECK_TARGETS OpenImageIO::maketx )
list(APPEND _IMPORT_CHECK_FILES_FOR_OpenImageIO::maketx "${_IMPORT_PREFIX}/bin/maketx" )

# Import target "OpenImageIO::oiiotool" for configuration "Release"
set_property(TARGET OpenImageIO::oiiotool APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(OpenImageIO::oiiotool PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/oiiotool"
  )

list(APPEND _IMPORT_CHECK_TARGETS OpenImageIO::oiiotool )
list(APPEND _IMPORT_CHECK_FILES_FOR_OpenImageIO::oiiotool "${_IMPORT_PREFIX}/bin/oiiotool" )

# Import target "OpenImageIO::testtex" for configuration "Release"
set_property(TARGET OpenImageIO::testtex APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(OpenImageIO::testtex PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/testtex"
  )

list(APPEND _IMPORT_CHECK_TARGETS OpenImageIO::testtex )
list(APPEND _IMPORT_CHECK_FILES_FOR_OpenImageIO::testtex "${_IMPORT_PREFIX}/bin/testtex" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
