cmake_minimum_required(VERSION 3.20)

project(web_server VERSION 1.0
                   DESCRIPTION "A basic HTTP server"
                   LANGUAGES CXX)

set(SOURCES src/main.cpp src/util.cpp)

set(CMAKE_CXX_STANDARD 20 CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Fetch ASIO library from GitHub
include(FetchContent)
FetchContent_Declare(
  asiocmake
  GIT_REPOSITORY "https://github.com/OlivierLDff/asio.cmake"
  GIT_TAG        "main"
)
FetchContent_MakeAvailable(asiocmake)

# Fetch fmt library from GitHub
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG master
)
FetchContent_MakeAvailable(fmt)

add_executable(web ${SOURCES})
target_link_libraries(web 
  PRIVATE
    asio::asio
    fmt)


# add_compile_options(-Wall -Werror -Wextra -Wno-unused-parameter -Wno-unused-variable)

# include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
# conan_basic_setup()

# add_executable(web main.cpp)
# target_link_libraries(web ${CONAN_LIBS})
