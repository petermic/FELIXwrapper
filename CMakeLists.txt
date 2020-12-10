####################################
# General project definition
####################################

CMAKE_MINIMUM_REQUIRED(VERSION 3.5.2 FATAL_ERROR)
SET(BUILD_SHARED_LIBS ON)

# Set cmake policy by version: https://cmake.org/cmake/help/latest/manual/cmake-policies.7.html
if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
  cmake_policy(VERSION 3.12)
endif()

# Define project
if(${CMAKE_VERSION} VERSION_GREATER 3.8.1)
  project(FELIXwrapper
    VERSION 0.0.2
    DESCRIPTION "FELIX wrapper for ALICE O2 software"
    LANGUAGES CXX
  )
else()
  project(FELIXwrapper
    VERSION 0.0.2
    LANGUAGES CXX
  )
endif()

# Documentation dir

# Add compiler flags for warnings and debug symbols
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")

# Set fPIC for all targets
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Set CMAKE_INSTALL_LIBDIR explicitly to lib (to avoid lib64 on CC7)
set(CMAKE_INSTALL_LIBDIR lib)

# Used only for debugging
# set(CMAKE_BUILD_TYPE "Debug")

# Set the default build type to "RelWithDebInfo"
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo"
    CACHE
    STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel Coverage."
    FORCE
  )
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)

####################################
# Dependencies
####################################

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(Threads REQUIRED)
find_package(Boost 1.5.6
  COMPONENTS
  exception
  REQUIRED
)

set(felix_deps FlxCard drivers_rcc regmap)

## find felix related software -- note link_directories is not encouraged by cmake
## somehow this is the only way that makes everything work
## also it needs to be called before adding the targets
if(EXISTS $ENV{FELIX_ROOT})
  message("-- Detecting FELIX installation at $ENV{FELIX_ROOT}.")
  foreach(felix_dep ${felix_deps})
    string(TOLOWER ${felix_dep} dep_loc)
    include_directories($ENV{FELIX_ROOT}/${dep_loc})
    link_directories($ENV{FELIX_ROOT}/$ENV{FELIX_ARCH}/${dep_loc})
  endforeach(felix_dep)
else()
  message(FATAL_ERROR "-- Cannot find FELIX.")
endif()

####################################
# Module, library and executable definition
####################################

add_library(FELIXwrapper SHARED
  src/CardInterface.cxx
  src/BarInterface.cxx
)

target_include_directories(FELIXwrapper
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>

  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(LINK_LIBS
  Boost::boost
)

# Link targets
target_link_libraries(FELIXwrapper
  PUBLIC
    ${LINK_LIBS}
  PRIVATE
    Threads::Threads
)

set_target_properties(FELIXwrapper
  PROPERTIES
  OUTPUT_NAME
    FELIXwrapper
)

## Used for debugging purposes to expose more options for benchmarking
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
endif()


# Use C++17
target_compile_features(FELIXwrapper PUBLIC cxx_std_17)

####################################
# Tests
####################################

enable_testing()
set(TEST_SRCS
)

foreach (test ${TEST_SRCS})
  include_directories(src)
  get_filename_component(test_name ${test} NAME)
  string(REGEX REPLACE ".cxx" "" test_name ${test_name})

  add_executable(${test_name} ${test})
  target_link_libraries(${test_name}
    PRIVATE
      FELIXwrapper
      Boost::unit_test_framework
      Threads::Threads
  )
  add_test(NAME ${test_name} COMMAND ${test_name})
  set_tests_properties(${test_name} PROPERTIES TIMEOUT 15) # TODO: WHAT IS THIS?
endforeach()

####################################
# Executables
####################################

if(CMAKE_BUILD_TYPE STREQUAL "Debug")

endif()

####################################
# Install
####################################

include(GNUInstallDirs)

# Build targets with install rpath on Mac to dramatically speed up installation
# https://gitlab.kitware.com/cmake/community/wikis/doc/cmake/RPATH-handling
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}" isSystemDir)
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  if("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "@loader_path/../${CMAKE_INSTALL_LIBDIR}")
  endif()
  set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
endif()
unset(isSystemDir)

# Install library and executables
install(TARGETS FELIXwrapper ${EXE_NAMES}
  EXPORT FELIXwrapperTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Create version file
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/cmake/FELIXwrapperConfigVersion.cmake"
  VERSION ${PACKAGE_VERSION}
  COMPATIBILITY AnyNewerVersion
)

# Install headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/FELIXwrapper DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

# Export targets
install(EXPORT FELIXwrapperTargets
  FILE
    FELIXwrapperTargets.cmake
  NAMESPACE
    AliceO2::
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/FELIXwrapper
)

# Configure and install Config files
