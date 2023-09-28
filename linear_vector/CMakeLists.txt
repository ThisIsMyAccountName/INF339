cmake_minimum_required(VERSION 3.15)

project(linear_matrix_verctor
        VERSION 1.0
        DESCRIPTION "Linear Matrix-Vector Multiplication"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED
             COMPONENTS
                 program_options
                 mpi
             REQUIRED)

add_executable(lin_mat_vec_mult linear_MVM.cpp)
target_include_directories(lin_mat_vec_mult PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(lin_mat_vec_mult PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(lin_mat_vec_mult PRIVATE ${Boost_LIBRARIES})
target_compile_options(lin_mat_vec_mult PRIVATE #[[ -Wall -Wextra -Wpedantic -Werror ]])

set(CMAKE_COLOR_DIAGNOSTICS On)