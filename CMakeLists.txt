cmake_minimum_required(VERSION 3.16)
project(HEDB VERSION 1.0 LANGUAGES CXX)
# make details
# set (CMAKE_VERBOSE_MAKEFILE on) 
set(CMAKE_CXX_STANDARD 20)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
    ADD_DEFINITIONS(-DHEDB_DEBUG)
    message(STATUS "STRAT HEDB DEBUG MODE")
    set(CMAKE_CXX_FLAGS "-O0 -g")
else() 
    set(CMAKE_CXX_FLAGS "-march=native -O3 -funroll-loops")
ENDIF()

#--------------------------Source tree-----------------------------
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")


#------------------------------Install-----------------------------
set(HEDB_INCLUDES_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/HEDB)

# ------Add source files to library and header files to install----
set(HEDB_SOURCE_FILES "")
add_subdirectory(src/HEDB/comparison)
add_subdirectory(src/HEDB/conversion)
add_subdirectory(src/HEDB/utils)

# -------------------------Add thirdparty--------------------------
add_subdirectory(thirdparty/TFHEpp)
add_subdirectory(thirdparty/SEAL)
find_package(OpenMP)
if(OpenMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

#-------------------Build only a static library--------------------
add_library(HEDB STATIC ${HEDB_SOURCE_FILES})
target_include_directories(HEDB PUBLIC src)
target_include_directories(HEDB PUBLIC thirdparty/SEAL/natice/src)
target_link_libraries(HEDB tfhe++ seal)
add_subdirectory(test)
