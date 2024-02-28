cmake_minimum_required(VERSION 3.27)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)

add_executable(LearnOpenGL
        main.cpp
        glad.c)

target_include_directories(LearnOpenGL PRIVATE dependencies)

add_subdirectory(dependencies/glfw)

target_link_libraries(LearnOpenGL glfw ${GLFW_LIBRARIES})
