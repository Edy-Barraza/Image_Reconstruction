#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "PNGwriter::PNGwriter" for configuration "Release"
set_property(TARGET PNGwriter::PNGwriter APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(PNGwriter::PNGwriter PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libPNGwriter.so"
  IMPORTED_SONAME_RELEASE "libPNGwriter.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS PNGwriter::PNGwriter )
list(APPEND _IMPORT_CHECK_FILES_FOR_PNGwriter::PNGwriter "${_IMPORT_PREFIX}/lib/libPNGwriter.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
