cmake_minimum_required(VERSION 3.0)
project(g4cmpHVeV)

#----------------------------------------------------------------------------
# Find G4CMP package -- NOTE: Assumes user has run g4cmp_env.*sh script!
#
set(CMAKE_PREFIX_PATH $ENV{G4CMPINSTALL}/../..)
find_package(G4CMP)

#----------------------------------------------------------------------------
# Find Geant4 package
# NOTE: WITH_GEANT4_UIVIS and USE_GEANT4_STATIC_LIBS are defined here
find_package(Geant4 REQUIRED ui_all vis_all)
#
# include($ENV{G4CMPINSTALL}/FindGeant4.cmake)

#----------------------------------------------------------------------------
# Setup include directories and compile definitions
# NOTE: Need to include G4CMP directories before G4.
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${G4CMP_INCLUDE_DIRS})
include(${G4CMP_USE_FILE})
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# RPATH stuff
#
# Automatically add paths of linked libraries to RPATH *unless* they are in the 
# build directory
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# Set the RPATH of the installed objects *unless* they are installing to a 
# system directory, in which case they shouldn't need an explicitly set RPATH.
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES 
     "${CMAKE_INSTALL_PREFIX}/lib" is_SYSTEM_DIR)
if("${is_SYSTEM_DIR}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif()

#----------------------------------------------------------------------------
# Build stuff
#
set(hvev_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HVeVActionInitialization.cc 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HVeVConfigManager.cc 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HVeVConfigMessenger.cc 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HVeVDetectorConstruction.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HVeVPrimaryGeneratorAction.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HVeVSensitivity.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HVeVRunAction.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HVeVEventAction.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HVeVTrackingAction.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HVeVSteppingAction.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HVeVHit.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HVeVKaplanQP.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HVeVPhononElectrode.cc
    )
    
if(USE_GEANT4_STATIC_LIBS)
    add_library(hvevLib STATIC ${hvev_SOURCES})
else()
    add_library(hvevLib SHARED ${hvev_SOURCES})
endif()
set_target_properties(hvevLib PROPERTIES OUTPUT_NAME g4cmpHVeV)

target_link_libraries(hvevLib ${G4CMP_LIBRARIES} ${Geant4_LIBRARIES})

add_executable(g4cmpHVeV g4cmpHVeV.cc)
target_link_libraries(g4cmpHVeV
    ${G4CMP_LIBRARIES}
    ${Geant4_LIBRARIES}
    -Wl,-force_load,$<TARGET_FILE:hvevLib>
    hvevLib)

install(TARGETS hvevLib DESTINATION lib)
install(TARGETS g4cmpHVeV DESTINATION bin)
