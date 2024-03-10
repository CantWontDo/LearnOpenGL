#include <iostream>
#include "glad/glad.h"
#include "glfw/include/GLFW/glfw3.h"
#include "helpers/Shader.h"
#include "stb/stb_image.h"
#include "helpers/Texture2D.h"
#include "helpers/Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// called when application window is resized
void framebufferSizeCallback(GLFWwindow* window, GLint width, GLint height);

void processInput(GLFWwindow* window);

void mouseCallback(GLFWwindow* window, double xPos, double yPos);

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

const float SCR_WIDTH = 1280;
const float SCR_HEIGHT = 720;

float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;


bool firstMouse = true;
float fov = 45;
Camera camera { Camera()};

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
    Shader basicShader {"../shaders/basic_lighting_shader.vert"
                            , "../shaders/basic_lighting_shader.frag"};

    Shader basicLightShader {"../shaders/basic_light_shader.vert"
            , "../shaders/basic_light_shader.frag"};

    // *** Initialization of VAO starts here ***
    // vertices of triangle, each vertex has 3 values (x, y, z). z is zero here to make it look 2d
    // these are unique vertices

    GLfloat vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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
    // first param: which vertex attribute (layout in vertex shader)
    // second param: size/amount of values of vertex attribute
    // third param: type of data
    // fourth: whether data is normalized (converting integer data to -1, 0, 1)
    // fifth: space between consecutive vertex attributes
    //          when data is tightly packed, can use 0 to automatically deduce stride
    //          requires explicit value when dealing with multiple vertex attributes
    // sixth: offset of data in buffer (this is beginning of buffer, so zero is fine)

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texCoords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // unbind vbo since it is stored in vao

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // *** Initialization of VAO ends here ***

    // unbind VAO so other can be created
    glBindVertexArray(0);

    GLuint lightVao;

    glGenVertexArrays(1, &lightVao);
    glBindVertexArray(lightVao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);


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

    Texture2D container {"../textures/container2.png"};
    Texture2D containerSpecular {"../textures/container2_specular.png"};

    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3( 10.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  15.0f, -15.0f),
            glm::vec3( 0.0f,  0.0f,  10.0f),
            glm::vec3( 12.0f,  5.0f, -15.0f),
            glm::vec3( 0.0f,  10.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -5.0f),
            glm::vec3( 3.0f,  10.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f)
    };

    glm::vec3 lightCol = glm::vec3(1.0, .5, .75);


    while(!glfwWindowShouldClose(window))
    {
        glm::vec3 lightPos = glm::vec3(0, -3, 0);

        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        processInput(window);

        glEnable(GL_DEPTH_TEST);

        glClearColor(0.2, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // rendering here


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

        glm::mat4 view = camera.getView();

        basicShader.use();

        glm::mat4 model = glm::identity<glm::mat4>();

        for(int i = 0; i < 10; i++)
        {
            model = glm::identity<glm::mat4>();

            model = glm::rotate(model, glm::radians((float)20 * i), glm::vec3(0, 1, 0));

            model = glm::translate(model, cubePositions[i]);

            glm::mat3 normalMat = glm::transpose(glm::inverse(model));

            basicShader.setMat4("model", model);
            basicShader.setMat4("projection", projection);
            basicShader.setMat4("view", view);
            basicShader.setMat3("normalMat", normalMat);

            basicShader.setVec3("viewPos", camera.getCameraPos());

            basicShader.setTexture2D("material.diffuse", 0, container);
            basicShader.setTexture2D("material.specular", 1, containerSpecular);
            basicShader.setFloat("material.shininess", 64.0);

            basicShader.setVec3("light.ambient", glm::vec3(0.3f));
            basicShader.setVec3("light.diffuse", glm::vec3(0.75f));
            basicShader.setVec3("light.specular", glm::vec3(1));
            basicShader.setVec3("light.position",  lightPos);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(lightVao);
        basicLightShader.use();

        model = glm::identity<glm::mat4>();

        model = glm::translate(model, lightPos);

        basicLightShader.setMat4("model", model);
        basicLightShader.setMat4("projection", projection);
        basicLightShader.setMat4("view", view);

        basicLightShader.setVec3("lightColor", lightCol);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        // check/call events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // deallocate resources
    glDeleteVertexArrays(1, &vao);
    glDeleteVertexArrays(1, &lightVao);
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
    const float cameraSpeed = 6.0f * deltaTime;

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.changeCameraPos(glm::normalize(glm::vec3(camera.getCameraDir().x, 0, camera.getCameraDir().z)) * cameraSpeed);
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.changeCameraPos(-glm::normalize(glm::vec3(camera.getCameraDir().x, 0, camera.getCameraDir().z)) * cameraSpeed);
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.changeCameraPos(camera.getCameraRight() * cameraSpeed);
    }

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.changeCameraPos(-camera.getCameraRight() * cameraSpeed);
    }

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        camera.changeCameraPos(-camera.worldUp * cameraSpeed);
    }

    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        camera.changeCameraPos(camera.worldUp * cameraSpeed);
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

    lastX = xPos;
    lastY = yPos;

    const float sensitivity = 0.2f;
    yOffset *= sensitivity;
    xOffset *= -sensitivity;

    camera.changeYaw(xOffset);
    camera.changePitch(yOffset);
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