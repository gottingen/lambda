#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "lambda::lambda" for configuration "Debug"
set_property(TARGET lambda::lambda APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(lambda::lambda PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/liblambda.a"
  )

list(APPEND _cmake_import_check_targets lambda::lambda )
list(APPEND _cmake_import_check_files_for_lambda::lambda "${_IMPORT_PREFIX}/lib/liblambda.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
