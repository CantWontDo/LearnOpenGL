#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// called when application window is resized
void framebufferSizeCallback(GLFWwindow* window, GLint width, GLint height);

int main()
{

    if(!glfwInit())
    {
        std::cout << "Failed to initialize GLFW!\n";
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;

    // creates a window with specified width and height
    window = glfwCreateWindow(640, 480, "My Window", nullptr, nullptr);

    if(!window)
    {
        std::cout << "Failed to create glfw window!\n";
        glfwTerminate();
        return -1;
    }

    // makes window current context (stores all the states)
    glfwMakeContextCurrent(window);


    // loads function pointers through GLAD (so don't need to do manually)
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD!\n";
        glfwTerminate();
        return -1;
    }

    // sets size of rendering dimensions (separate from window size)
    glViewport(0, 0, 640, 480);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // repeatedly polls events (input, etc.) while window shouldn't close, keeps it open
    // also swaps buffers (we use double buffer, means that we draw to back buffer, and it swaps at
    // beginning of frame, prevents visible lag

    while(!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleans up and terminates glfw
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, GLint width, GLint height)
{
    glViewport(0, 0, width, height);
}
