# Root CMakeLists.txt

cmake_minimum_required(VERSION 3.22)

project(AoC2025 VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

if(MSVC)
	add_compile_options(/W4) # /Wx
else()
	add_compile_options(-Wall -Wextra) # -Werror
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(helpers)
set(HELPER_INCLUDE_DIR ${HELPER_INCLUDE_DIR})

# add_subdirectory(exercise1)
# add_subdirectory(exercise2)
# add_subdirectory(exercise3)
add_subdirectory(exercise4)