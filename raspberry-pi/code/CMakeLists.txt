cmake_minimum_required(VERSION 3.15...3.31)

project(
  raspberry-pi-test
  VERSION 0.0
  LANGUAGES CXX)

file( GLOB TESTS_SRC ${PROJECT_SOURCE_DIR}/src/*.cc )
foreach( testsrc ${TESTS_SRC} )
    # Remove .cc
    get_filename_component( testname ${testsrc} NAME_WE )
    add_executable( ${testname} ${testsrc} )
    target_link_libraries( ${testname} wiringPi )
endforeach( testsrc ${TESTS_SRC} )

add_custom_target(clean-all
   COMMAND ${CMAKE_BUILD_TOOL} clean
   COMMAND ${CMAKE_COMMAND} -P CMakeTargets.cmake
)
