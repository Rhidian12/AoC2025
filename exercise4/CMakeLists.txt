# Exercise 4 CMakeLists.txt

add_executable(exercise4 main.cpp)
target_include_directories(exercise4 PUBLIC ${HELPER_INCLUDE_DIR})
target_link_libraries(exercise4 helpers)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/input.txt DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})