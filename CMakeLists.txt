cmake_minimum_required(VERSION 3.15.0)

project(Mysterious_Dungeon VERSION 1.0.0)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

file(GLOB MAIN_SRC
  "src/*.h"   
  "src/*.cpp"
)

add_executable(main ${MAIN_SRC})  
target_link_libraries(main ${CURSES_LIBRARIES})
