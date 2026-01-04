cmake_minimum_required(VERSION 3.15.0)

project(Mysterious_Dungeon VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})
include_directories(src)

file(GLOB_RECURSE LIB_SRC
  "src/*.h"
  "src/*.cpp"
)
list(REMOVE_ITEM LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp) # remove main.cpp from LIB_SRC

# Create a library
add_library(Mysterious_Dungeon ${LIB_SRC})
target_link_libraries(Mysterious_Dungeon Threads::Threads ${CURSES_LIBRARIES})

# Create an executable
add_executable(main src/main.cpp)
target_link_libraries(main Mysterious_Dungeon)

include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

install(TARGETS main
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(TARGETS Mysterious_Dungeon
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -Wno-error=maybe-uninitialized")
endif()

include(CTest)
enable_testing()

# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.10.0.tar.gz
)
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

add_subdirectory(tests)
