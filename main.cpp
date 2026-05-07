#include <iostream>

#include <glad/glad.h>
#include <SDL2/SDL.h>

float vertices[] = {
    // Triangle 1
    -0.5f, -0.5f,
     0.0f, -0.5f,
    -0.25f, 0.5f,

    // Triangle 2
     0.0f, -0.5f,
     0.5f, -0.5f,
     0.25f, 0.5f
};

const char* vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec2 aPos;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
)";

unsigned int VAO, VBO;
unsigned int shaderProgram;

// Process input for the window
static void processInput(bool &isRunning)
{
    // Process Input
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            isRunning = false;
        }

        if(event.type == SDL_KEYDOWN)
        {
            if(event.key.keysym.sym == SDLK_ESCAPE)
            {
                isRunning = false;
            }
        }
    }
}

// Render with OpenGL within the window
static void render(SDL_Window *window)
{
    // Render
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    SDL_GL_SwapWindow(window);
}

// Generate the GPU Objects
static void genGPUObj()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

// Bind Objects
static void bindGPUObj()
{
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                sizeof(vertices),
                vertices,
                GL_STATIC_DRAW);
}

// Describe the vertex layout
static void descVerLay()
{
    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);
}

// Create the shaderprogram object
static void createShaderProgram()
{
    // Vertex Shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(
        vertexShader,
        1,
        &vertexShaderSource,
        nullptr
    );

    glCompileShader(vertexShader);

    // Fragment Shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(
        fragmentShader,
        1,
        &fragmentShaderSource,
        nullptr
    );

    glCompileShader(fragmentShader);

    // Shader Program
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    // Cleanup shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

int main()
{
    // Initializes a SDL video subsystem
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL failed to initialize\n";
        return -1;
    }

    // Tell SDL to open OpenGL and the attributes for the verison
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    // Create the actual window
    SDL_Window *window = SDL_CreateWindow(
        "Game Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280,
        720,
        SDL_WINDOW_OPENGL
    );

    if(!window)
    {
        std::cout << "Failed to create window\n";
        SDL_Quit();
        return -1;
    }

    // Create the OpenGL Context
    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    if(!glContext)
    {
        std::cout << "Failed to create OpenGL context\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Initialize GLAD
    if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    createShaderProgram();
    genGPUObj();
    bindGPUObj();
    descVerLay();

    // Print OpenGL version
    std::cout << "OpenGL Version: "
              << glGetString(GL_VERSION)
              << std::endl;

    // Main loop for window
    bool running = true;

    while(running)
    {
        processInput(running);

        render(window);
    }

    // Cleanup
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}