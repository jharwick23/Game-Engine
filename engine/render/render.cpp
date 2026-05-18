#include "render.h"

Renderer::Renderer()
    : vertices{
        // For a rectangle
        //0.5f, 0.5f, 0.0f, // top right
        //0.5f, -0.5f, 0.0f, // bottom right
        //-0.5f, -0.5f, 0.0f, // bottom left
        //-0.5f, 0.5f, 0.0f // top left

        // Triangle 1
        -0.5f, -0.5f,
         0.0f, -0.5f,
        -0.25f,  0.5f,

        // Triangle 2
         0.0f, -0.5f,
         0.5f, -0.5f,
         0.25f,  0.5f
    },
    indices{
        0, 1, 2,  // first triangle
        2, 3, 0   // second triangle
    }
{
    // Vertex shader source code
    // Responsible for vertex positioning
    vertexShaderSource = R"(
    #version 330 core

    layout (location = 0) in vec2 aPos;

    void main()
    {
        gl_Position = vec4(aPos, 0.0, 1.0);
    }
    )";

    // Fragment shader source code
    // Responsible for final pixel color
    fragmentShaderSource = R"(
    #version 330 core

    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
    )";
}

void Renderer::render(SDL_Window *window)
{
    // Set screen clear/background color
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // Clear framebuffer color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Activate shader program for rendering
    glUseProgram(shaderProgram);

    // Bind vertex array containing vertex layout state
    glBindVertexArray(VAO);

    // Render the traingles rom an index buffer
    // When we use glDrawElements we draw using indices provided in the element buffer currently bound
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Draw triangles using currently bound VAO/shader
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Swap back buffer to front buffer for display
    SDL_GL_SwapWindow(window);
}

void Renderer::initRenderer()
{
    createShaderProgram();
    genGPUObj();
    bindGPUObj();
    descVerLay();
}

void Renderer::genGPUObj()
{
    // Generate Vertex Array Object
    glGenVertexArrays(1, &VAO);

    // Generate Vertex Buffer Object
    glGenBuffers(1, &VBO);

    // Generate Element Buffer Object
    glGenBuffers(1, &EBO);  
}

void Renderer::bindGPUObj()
{
    // Bind VAO so subsequent vertex state is stored inside it
    glBindVertexArray(VAO);

    // Bind VBO as current array buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Upload vertex data from CPU memory to GPU memory
    glBufferData(GL_ARRAY_BUFFER,
                sizeof(vertices),
                vertices.data(),
                GL_STATIC_DRAW);

    // Bind EBO as current element array buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(indices),
                 indices.data(),
                 GL_STATIC_DRAW);
}

void Renderer::descVerLay()
{
    // Describe vertex attribute layout:
    // location = 0
    // 2 floats per vertex for two triangles
    // 3 floats per vertex for rectangle
    // tightly packed
    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        (void*)0
    );

    // Enable vertex attribute location 0
    glEnableVertexAttribArray(0);
}

void Renderer::createShaderProgram()
{
    // Vertex Shader
    // Create vertex shader object
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Attach GLSL source code to shader object
    glShaderSource(
        vertexShader,
        1,
        &vertexShaderSource,
        nullptr
    );

    // Compile vertex shader into GPU instructions
    glCompileShader(vertexShader);

    // Fragment Shader
    // Create fragment shader object
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Attach GLSL source code to fragment shader
    glShaderSource(
        fragmentShader,
        1,
        &fragmentShaderSource,
        nullptr
    );

    // Compile fragment shader
    glCompileShader(fragmentShader);

    // Shader Program
    // Create final GPU shader program object
    shaderProgram = glCreateProgram();

    // Attach shaders to shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    // Link shaders into executable GPU program
    glLinkProgram(shaderProgram);

    // Cleanup shaders
    // Individual shader objects no longer needed after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}