cmake_minimum_required(VERSION 3.15)

project(bcast
        VERSION 1.0
        DESCRIPTION "bcast timer"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED
             COMPONENTS
                 program_options
                 mpi
             REQUIRED)

add_executable(bcast bcast.cpp)
target_include_directories(bcast PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(bcast PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(bcast PRIVATE ${Boost_LIBRARIES})
target_compile_options(bcast PRIVATE #[[ -Wall -Wextra -Wpedantic -Werror ]])

set(CMAKE_COLOR_DIAGNOSTICS On)