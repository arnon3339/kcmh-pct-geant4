cmake_minimum_required(VERSION 3.28.3)
project(kcmh-pct-geant4)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

find_package(ROOT REQUIRED)

include(${Geant4_USE_FILE})
include(${ROOT_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include
 ${PROJECT_SOURCE_DIR}/phantom)

file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

set(SCRIPTS
    run.mac
    vis.mac
    init_vis.mac
    beam_collimated.mac
    beam_kcmh.mac
    beam_paralell.mac
    run_noph.mac
)

foreach(_script ${SCRIPTS})
    configure_file(
        ${PROJECT_SOURCE_DIR}/${_script}
        ${PROJECT_BINARY_DIR}/${_script}
        COPYONLY
    )
endforeach()

add_definitions(-DPHANTOM_PATH="${PROJECT_SOURCE_DIR}/phantom")
add_executable(pctKCMH pctKCMH.cc ${headers} ${sources})
target_link_libraries(pctKCMH ${Geant4_LIBRARIES})
target_link_libraries(pctKCMH ${ROOT_LIBRARIES})