# ./helpers/CMakeLists.txt

add_library(helpers STATIC FileUtilities.cpp Log.cpp)
set(HELPER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
message("Helper include dir: ${HELPER_INCLUDE_DIR}")