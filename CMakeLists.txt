project(FindPair C CXX)
cmake_minimum_required(VERSION 3.8)
set (CMAKE_CXX_STANDARD 14)

set(SRC_FIND_PAIR
    src/FindPair.cpp
)

add_executable(FindPair ${SRC_FIND_PAIR})

install(TARGETS FindPair
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
