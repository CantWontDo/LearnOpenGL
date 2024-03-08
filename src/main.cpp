#include <iostream>
#include "glad/glad.h"
#include "glfw/include/GLFW/glfw3.h"
#include "helpers/Shader.h"
#include "stb/stb_image.h"
#include "helpers/Texture2D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// called when application window is resized
void framebufferSizeCallback(GLFWwindow* window, GLint width, GLint height);

void processInput(GLFWwindow* window);

void mouseCallback(GLFWwindow* window, double xPos, double yPos);

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);


glm::vec3 cameraPos = glm::vec3(0, 0, 3);
glm::vec3 cameraFront = glm::vec3(0, 0, -1);
glm::vec3 cameraUp = glm::vec3(0, 1, 0);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

const float SCR_WIDTH = 1280;
const float SCR_HEIGHT = 720;

float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;

float yaw = -90;
float pitch = 0;
float fov = 45;

bool firstMouse = true;

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
    window = glfwCreateWindow((int)SCR_WIDTH, (int)SCR_HEIGHT, "My Window", nullptr, nullptr);

    if(!window)
    {
        std::cout << "Failed to create glfw window!\n";
        glfwTerminate();
        return -1;
    }

    // makes window current context (stores all the states)
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);


    // loads function pointers through GLAD (so don't need to do manually)
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD!\n";
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // shaders are also represented with objects/ids
    Shader basicShader {"../shaders/basic_vertex_shader.vert"
                            , "../shaders/basic_fragment_shader.frag"};

    // *** Initialization of VAO starts here ***
    // vertices of triangle, each vertex has 3 values (x, y, z). z is zero here to make it look 2d
    // these are unique vertices

    GLfloat vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    GLuint indices[] {
        0, 1, 3,
        1, 2, 3
    };

    // A vertex array object stores vertex attribute calls
    // only have to configure once
    // can store different types of config and switch just by binding to vao
    GLuint vao;

    // generates an id for vertex array object
    glGenVertexArrays(1, &vao);

    // vertex buffer object, stores many vertices at once, sends large batches to reduce sending data
    GLuint vbo;
    GLuint ebo;

    // generates an id for the buffer, first argument is amount of ids generated
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // bind vao before vbo to store vertex attrib
    glBindVertexArray(vao);

    // binds type of buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // following calls configure bound buffer (vbo)

    // copies vertex data into buffer memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // first arg: type of bound buffer
    // second arg: size of data in bytes
    // third arg: data
    // fourth arg: how to manage memory {
    //  GL_STATIC_STREAM: set once and a few times
    //  GL_STATIC_DRAW: set once and used many times
    //  GL_DYNAMIC_DRAW: set many times and used many times

    // tells opengl how to interpret vertex data
    // first param: which vertex attribute (layout in vertex shader)
    // second param: size/amount of values of vertex attribute
    // third param: type of data
    // fourth: whether data is normalized (converting integer data to -1, 0, 1)
    // fifth: space between consecutive vertex attributes
    //          when data is tightly packed, can use 0 to automatically deduce stride
    //          requires explicit value when dealing with multiple vertex attributes
    // sixth: offset of data in buffer (this is beginning of buffer, so zero is fine)

    // vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    // allows vertex attribute to be used
    glEnableVertexAttribArray(0);

    // vertex colors
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

   // glEnableVertexAttribArray(1);

    // texCoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glEnableVertexAttribArray(2);
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

    // generate an object to hold texture

    // flips all loaded images on the y-axis when loading
    stbi_set_flip_vertically_on_load(true);

    Texture2D texture1 {"../textures/container.jpg"};
    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    float aspect = 640.f / 480;

    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        processInput(window);

        glEnable(GL_DEPTH_TEST);

        glClearColor(0.2, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // rendering here

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


        glm::mat4 projection = glm::perspective(glm::radians(fov), 640 / 480.0f, 0.1f, 100.0f);

        //glm::mat4 projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f, 0.1f, 100.0f);

        // sets active texture unit
        // allows to set multiple texture uniforms
        // to use, just set active texture unit and bind texture
        // activates shader program

        // bind texture (maps to ourTexture uniform in frag shader)
        // Reuse VAO to prevent rebinding data to VBO
        glBindVertexArray(vao);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // draw triangle
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        basicShader.use();
        for(int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::identity<glm::mat4>();

            model = glm::rotate(model, glm::radians((float)20 * i), glm::vec3(0, 1, 0));

            model = glm::translate(model, cubePositions[i]);

            // tells frag shader which texture sampler goes with which texture unit
            basicShader.setTexture2D("texture1", 0, texture1);
            basicShader.setMat4("model", model);
            basicShader.setMat4("projection", projection);
            basicShader.setMat4("view", view);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        // check/call events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // deallocate resources
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

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
    const float cameraSpeed = 6.0f * deltaTime;

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPos += cameraFront * cameraSpeed;
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos -= cameraFront * cameraSpeed;
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse) // initially set to true
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    float xOffset = xPos - lastX;
    float yOffset = yPos - lastY;
    yOffset *= -1;

    lastX = xPos;
    lastY = yPos;

    const float sensitivity = 0.2f;
    yOffset *= sensitivity;
    xOffset *= sensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if(pitch > 89.0f)
    {
        pitch = 89.0f;
    }
    if(pitch < -89.0f)
    {
        pitch = -89.0f;
    }
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    fov -= (float)yOffset;
    if(fov < 1.0)
    {
        fov = 1.0;
    }
    if(fov > 45.0)
    {
        fov = 45.0;
    }
}