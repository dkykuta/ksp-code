cmake_minimum_required (VERSION 2.8.3)

project (haruki-ksp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -p -pg -no-pie")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (CMAKE_VERSION VERSION_LESS "3.1")
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set (CMAKE_CXX_FLAGS "-std=gnu++11 ${CMAKE_CXX_FLAGS}")
  endif ()
else ()
  set (CMAKE_CXX_STANDARD 11) 
endif ()
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Setup testing

set(LIB_SOURCE
  src/graph.cpp
  src/path.cpp
  src/dijkstra.cpp
  src/yenksp.cpp
  src/pascoalksp.cpp
  src/fengksp.cpp
  src/fengcopyksp.cpp
  src/hybridfengksp.cpp
  src/dimacsreader.cpp
)

set(MAIN_SOURCE 
  src/main.cpp
  ${LIB_SOURCE})

set(TEST_SOURCE 
  test/testMain.cpp
  ${LIB_SOURCE})

set(SIMPLE_MAIN_SOURCE
    src/mainsinglealgo.cpp
    ${LIB_SOURCE})

add_executable(${PROJECT_NAME} ${MAIN_SOURCE})

add_executable(ksp-single-algorithm ${SIMPLE_MAIN_SOURCE})
set_target_properties( ksp-single-algorithm PROPERTIES COMPILE_FLAGS "-DHRK_COUNT_" )

add_executable(runTests ${TEST_SOURCE})

target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)

