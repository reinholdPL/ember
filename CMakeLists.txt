cmake_minimum_required(VERSION 3.15)

# Project name and version
project(ember3 VERSION 1.0)

#verbose build
# set(CMAKE_VERBOSE_MAKEFILE ON)

# # Set the C standard
# set(CMAKE_C_STANDARD 99)
# set(CMAKE_C_STANDARD_REQUIRED ON)

# use clang
set(CMAKE_CXX_COMPILER "clang++")
# c++17
set(CMAKE_CXX_STANDARD 17)

# clang use c++17
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

#configure compiler to not show unused variable warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable -g")

# Find the GLFW library
# find_package(GLFW3 REQUIRED)
# find_package(GLFW REQUIRED)

# Find OpenGL (required for GLFW)
find_package(OpenGL REQUIRED)

# Add all cpp files in src to the executable
file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(ember3
    ${SOURCES}
)

# add all sources from modules/glad/src
file(GLOB_RECURSE GLAD_SOURCES modules/glad/src/*.c)
target_sources(ember3 PRIVATE ${GLAD_SOURCES} )

#add all src from modules/imgui
file(GLOB IMGUI_SOURCES modules/imgui/*.cpp)
target_sources(ember3 PRIVATE ${IMGUI_SOURCES} 
    ./modules/imgui/backends/imgui_impl_glfw.cpp
    ./modules/imgui/backends/imgui_impl_opengl3.cpp
)

# Include directories
target_include_directories(ember3 PRIVATE
    ./include
    ./modules/imgui
    ./modules/imgui/backends
    ${GLFW3_INCLUDE_DIR}
    ./modules/glad/include
    
)

# Link libraries
target_link_libraries(ember3 PRIVATE
    glfw
    OpenGL::GL
    -lX11
)

# Optional: Set compiler warnings
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ember3 PRIVATE -Wall -Wextra -pedantic)
elseif (MSVC)
    target_compile_options(ember3 PRIVATE /W4)
endif()

