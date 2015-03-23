cmake_minimum_required(VERSION 3.0)
project(lisp)
add_definitions("-std=c++14")
file(GLOB SOURCE
  "*.h"
  "*.cpp"
  "lisp/*.h"
  "lisp/*.cpp"
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")
add_executable(${PROJECT_NAME} ${SOURCE})
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin)
