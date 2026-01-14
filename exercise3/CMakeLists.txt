# Exercise 3 CMakeLists.txt

add_executable(exercise3 main.cpp)
target_include_directories(exercise3 PUBLIC ${HELPER_INCLUDE_DIR})
target_link_libraries(exercise3 helpers)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/input.txt DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})