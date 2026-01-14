cmake_minimum_required(VERSION 3.16)
project(chip8-emulator VERSION 1.0 LANGUAGES CXX)

# Standard C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Trouver SDL2
find_package(SDL2 REQUIRED)

# Sources
set(SOURCES
    src/main.cpp
    src/chip8.cpp
    src/display.cpp
)

# Exécutable
add_executable(chip8 ${SOURCES})

# Lier SDL2
target_include_directories(chip8 PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(chip8 PRIVATE ${SDL2_LIBRARIES})

# Message de configuration
message(STATUS "SDL2 Include: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2 Libraries: ${SDL2_LIBRARIES}")
