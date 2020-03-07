cmake_minimum_required(VERSION 3.9)

project(VimBackend VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

find_library(CURSES_PANEL_LIBRARY NAMES panel DOC "The curses panel library")
if (NOT CURSES_PANEL_LIBRARY)
    message(FATAL_ERROR "Curses' panel library not found.")
endif ()

add_library(VimBackend STATIC
    src/VimBackend
    src/InputReader.cpp
)

target_include_directories(VimBackend PUBLIC include common-interface)
target_include_directories(VimBackend PRIVATE ${CURSES_INCLUDE_DIRS} /usr/include/ncurses)
target_link_libraries(VimBackend ${CURSES_LIBRARIES} ${CURSES_PANEL_LIBRARY})

add_executable(test_tool
    testTool/main
)
set_target_properties(test_tool PROPERTIES EXCLUDE_FROM_ALL TRUE)
target_include_directories(test_tool PRIVATE ${CURSES_INCLUDE_DIRS} /usr/include/ncurses)
target_link_libraries(test_tool VimBackend ${CURSES_LIBRARIES} ${CURSES_PANEL_LIBRARY})
