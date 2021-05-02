cmake_minimum_required(VERSION 3.12)

# set the project name
project(TinyEvent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()


add_subdirectory(src)
enable_testing()
add_subdirectory(tests)