# Exercise 1 CMakeLists.txt

add_executable(exercise1 main.cpp)
target_include_directories(exercise1 PUBLIC ${HELPER_INCLUDE_DIR})
target_link_libraries(exercise1 helpers)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/input.txt DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})