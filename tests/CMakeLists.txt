add_executable(unit_tests test_a_star.cpp)

# Include the directories for gtest and gtest_main
target_include_directories(unit_tests PRIVATE ${gtest_SOURCE_DIR} ${gtest_main_SOURCE_DIR})

target_link_libraries(unit_tests gtest_main Mysterious_Dungeon ${CURSES_LIBRARIES} Threads::Threads)

add_test(NAME unit
         COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_tests)

