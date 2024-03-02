#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

// called when application window is resized
void framebufferSizeCallback(GLFWwindow* window, GLint width, GLint height);

void processInput(GLFWwindow* window);

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
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // loads function pointers through GLAD (so don't need to do manually)
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD!\n";
        glfwTerminate();
        return -1;
    }

    // shaders are also represented with objects/ids
    Shader basicShader {"../shaders/basic_vertex_shader.vert"
                            , "../shaders/basic_fragment_shader.frag"};

    // *** Initialization of VAO starts here ***
    // vertices of triangle, each vertex has 3 values (x, y, z). z is zero here to make it look 2d
    // these are unique vertices
    GLfloat vertices[] = {
            // positions         // colors
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
            0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top

    };

    // A vertex array object stores vertex attribute calls
    // only have to configure once
    // can store different types of config and switch just by binding to vao
    GLuint vao;

    // generates an id for vertex array object
    glGenVertexArrays(1, &vao);

    // vertex buffer object, stores many vertices at once, sends large batches to reduce sending data
    GLuint vbo;

    // generates an id for the buffer, first argument is amount of ids generated
    glGenBuffers(1, &vbo);

    // bind vao before vbo to store vertex attrib
    glBindVertexArray(vao);

    // binds type of buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // following calls configure bound buffer (vbo)

    // copies vertex data into buffer memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // first arg: type of bound buffer
    // second arg: size of data in bytes
    // third arg: data
    // fourth arg: how to manage memory {
    //  GL_STATIC_STREAM: set once and a few times
    //  GL_STATIC_DRAW: set once and used many times
    //  GL_DYNAMIC_DRAW: set many times and used many times

    // tells opengl how to interpret vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);

    // first param: which vertex attribute (layout in vertex shader)
    // second param: size/amount of values of vertex attribute
    // third param: type of data
    // fourth: whether data is normalized (converting integer data to -1, 0, 1)
    // fifth: space between consecutive vertex attributes
    //          when data is tightly packed, can use 0 to automatically deduce stride
    //          requires explicit value when dealing with multiple vertex attributes
    // sixth: offset of data in buffer (this is beginning of buffer, so zero is fine)

    // allows vertex attribute to be used
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glEnableVertexAttribArray(1);
    // unbind vbo since it is stored in vao

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // *** Initialization of VAO ends here ***

    // unbind VAO so other can be created
    glBindVertexArray(0);


    // draws primitive (triangle in this case) using currently used shader program and bound vbo / vao

    // first: type of primitive (triangle, point, line)
    // second: starting index of vertices
    // third: amount of vertices to draw

    // summary:
    // 1. copy vertices to a vertex buffer object or vbo for efficiency
    // 2. tell opengl how to interpret the data with a vertex attribute pointer
    // 3. use the shader program
    // 4. draw the triangle

    // repeatedly polls events (input, etc.) while window shouldn't close, keeps it open
    // also swaps buffers (we use double buffer, means that we draw to back buffer, and it swaps at
    // beginning of frame, prevents visible lag

    while(!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // clears color buffer (changing color of screen)

        glClearColor(0.2, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        // rendering here

        // uses
        basicShader.use();

        // Reuse VAO to prevent rebinding data to VBO
        glBindVertexArray(vao);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // draw triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // check/call events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // deallocate resources
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    // cleans up and terminates glfw
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, GLint width, GLint height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}