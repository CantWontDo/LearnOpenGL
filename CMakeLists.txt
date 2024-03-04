cmake_minimum_required(VERSION 3.27)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 20)

add_executable(LearnOpenGL
        src/main.cpp
        dependencies/glad/glad.c
        src/helpers/Shader.cpp
        src/helpers/Shader.h
        dependencies/stb/stb_image.cpp
        src/helpers/Texture2D.cpp
        src/helpers/Texture2D.h)

target_include_directories(LearnOpenGL PRIVATE dependencies)

add_subdirectory(dependencies/glfw)

target_link_libraries(LearnOpenGL glfw ${GLFW_LIBRARIES})
