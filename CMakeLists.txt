# MICA top level CMake build script - A. J. Dobbs
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(mica VERSION 0.1.0 LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 11)

# Specify where build output should go
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# Pull in ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT)
include(${ROOT_USE_FILE}) # Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)

# Pull in MAUS
include_directories(include $ENV{MAUS_ROOT_DIR} $ENV{MAUS_ROOT_DIR}/src/common_cpp)
link_directories($ENV{MAUS_ROOT_DIR}/build)

# Build the MICA library
add_library(MicaCore SHARED src/AnalyserBase.cc
                        src/AnalyserTrackerAngularMomentum.cc
                        src/AnalyserTrackerMC.cc
                        src/AnalyserTrackerMCPurity.cc
                        src/AnalyserTrackerPREfficiency.cc
                        src/AnalyserTrackerPRSeedNPEResidual.cc
                        src/AnalyserTrackerPRSeedResidual.cc
                        src/AnalyserTrackerPRStats.cc
                        src/AnalyserTrackerSpacePoints.cc
                        src/AnalyserTrackerSpacePointSearch.cc
                        src/AnalyserTrackerSpacePointSearchStation.cc)
target_link_libraries(MicaCore ${ROOT_LIBRARIES} MausCpp)

# Build the MICA app
link_directories(${CMAKE_BINARY_DIR})
add_executable(mica app/mica.cc)
target_link_libraries(mica ${ROOT_LIBRARIES} MausCpp MicaCore)

# Specify where installing will place the output
install(TARGETS mica MicaCore
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
