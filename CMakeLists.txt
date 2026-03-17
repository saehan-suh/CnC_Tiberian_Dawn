cmake_minimum_required(VERSION 3.28)

project(CnC_TiberianDawn LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(cnc_td_lib STATIC
    ABSTRACT.CPP
    FACING.CPP
    COORD.CPP
)

target_include_directories(cnc_td_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(cnc_td WIN32
    CONQUER.CPP
)
target_link_libraries(cnc_td PRIVATE cnc_td_lib)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(tests)