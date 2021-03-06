cmake_minimum_required(VERSION 2.6)

project(rtflagger)

find_library(AOFLAGGER_LIB aoflagger)
find_path(AOFLAGGER_INCLUDE_DIR NAMES aoflagger.h)

enable_language(Fortran OPTIONAL)
find_package(LAPACK REQUIRED)
find_package(Boost REQUIRED COMPONENTS date_time thread filesystem system)
find_library(FFTW3_LIB fftw3 REQUIRED)
find_package(LibXml2 REQUIRED)
find_library(LIBSLA_LIB sla REQUIRED)
find_library(PNG_LIB png REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DNDEBUG -O3 -march=native -std=c++11")

include_directories(${AOFLAGGER_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIR})

# The following stuff will set the "rpath" correctly, so that
# LD_LIBRARY_PATH doesn't have to be set.

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF("${isSystemDir}" STREQUAL "-1")

add_executable(rtflagger main.cpp rtflagger.cpp stopwatch.cpp)

add_executable(fixmwams fixmwams.cpp fitsuser.cpp metafitsfile.cpp mwaconfig.cpp mwams.cpp)

target_link_libraries(rtflagger ${AOFLAGGER_LIB} ${LAPACK_lapack_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_DATE_TIME_LIBRARY} ${LIBXML2_LIBRARIES} ${FFTW3_LIB} ${PNG_LIB} ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES})

install (TARGETS rtflagger DESTINATION bin)
