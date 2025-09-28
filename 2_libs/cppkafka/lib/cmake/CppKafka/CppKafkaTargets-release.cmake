#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "CppKafka::cppkafka" for configuration "Release"
set_property(TARGET CppKafka::cppkafka APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(CppKafka::cppkafka PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libcppkafka.so.0.4.1"
  IMPORTED_SONAME_RELEASE "libcppkafka.so.0.4.1"
  )

list(APPEND _cmake_import_check_targets CppKafka::cppkafka )
list(APPEND _cmake_import_check_files_for_CppKafka::cppkafka "${_IMPORT_PREFIX}/lib/libcppkafka.so.0.4.1" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
