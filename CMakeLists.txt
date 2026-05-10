cmake_minimum_required(VERSION 3.16)

project(GameEngine)

set(CMAKE_CXX_STANDARD 17)

# Find packages
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

# Create executable
add_executable(GameEngine
    main.cpp
    vendor/glad/src/glad.c
    engine/input/input.cpp
    engine/render/render.cpp
)

# Include directories
target_include_directories(GameEngine PRIVATE
    vendor/glad/include
)

# Link libraries
target_link_libraries(GameEngine
    SDL2
    OpenGL::GL
    dl
)