# MICA top level CMake build script - A. J. Dobbs
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(mica VERSION 1.1.1 LANGUAGES CXX)
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
                        src/AnalyserFactory.cc
                        src/IAnalyser.cc
                        src/AnalyserGroup.cc
                        src/AnalyserTrackerAngularMomentum.cc
                        src/AnalyserTrackerMC.cc
                        src/AnalyserTrackerMCPRResiduals.cc
                        src/AnalyserTrackerMCPurity.cc
                        src/AnalyserTrackerPREfficiency.cc
                        src/AnalyserTrackerPRSeedNPEResidual.cc
                        src/AnalyserTrackerPRSeedResidual.cc
                        src/AnalyserTrackerPRStats.cc
                        src/AnalyserTrackerSpacePoints.cc
                        src/AnalyserTrackerSpacePointSearch.cc
                        src/AnalyserTrackerSpacePointSearchStation.cc
                        src/AnalyserTrackerChannelHits.cc
                        src/AnalyserTrackerKFStats.cc
                        src/AnalyserTofTracker.cc
                        src/AnalyserTrackerKFMomentum.cc
                        src/AnalyserViewerRealSpace)
target_link_libraries(MicaCore ${ROOT_LIBRARIES} MausCpp)

# Build the MICA app
link_directories(${CMAKE_BINARY_DIR})
add_executable(mica app/mica.cc)
target_link_libraries(mica ${ROOT_LIBRARIES} MausCpp MicaCore)

# Build the event viewer app
link_directories(${CMAKE_BINARY_DIR})
add_executable(event-viewer app/event-viewer.cc)
target_link_libraries(event-viewer ${ROOT_LIBRARIES} MausCpp MicaCore)

# Specify where installing will place the output
install(TARGETS mica event-viewer MicaCore
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

# Generate the Doxygen documentation
# First we can indicate the documentation build as an option and set it to ON by default
# option(BUILD_DOC "Build documentation" OFF)
#
# # Check if Doxygen is installed
# find_package(Doxygen)
# if (DOXYGEN_FOUND)
#     # Set input and output files
#     set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
#     set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile)
#
#     # Request to configure the file
#     configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
#     message("Doxygen build started")
#
#     # Note the option ALL which allows to build the docs together with the application
#     add_custom_target( doc_doxygen ALL
#         COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
#         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#         COMMENT "Generating API documentation with Doxygen"
#         VERBATIM )
# else (DOXYGEN_FOUND)
#   message("Doxygen need to be installed to generate the doxygen documentation")
# endif (DOXYGEN_FOUND)
