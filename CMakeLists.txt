cmake_minimum_required(VERSION 3.15.0)

project(Mysterious_Dungeon VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})
include_directories(src)

file(GLOB_RECURSE MAIN_SRC
  "src/*.h"   
  "src/*.cpp"
)

add_executable(main ${MAIN_SRC})  
target_link_libraries(main Threads::Threads)
target_link_libraries(main ${CURSES_LIBRARIES})
