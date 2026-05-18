#ifndef RENDER_H
#define RENDER_H

#include <array>
#include <glad/glad.h>
#include <SDL2/SDL.h>

class Renderer
{
private:
    // Vertex data stored on CPU side before upload to GPU
    std::array<float, 12> vertices;

    // Use indices and Element Buffer Objects to draw a rectangle without as much overhead
    std::array<unsigned int, 6> indices;

    unsigned int EBO;

    // GLSL source code for vertex shader
    const char* vertexShaderSource;

    // GLSL source code for fragment shader
    const char* fragmentShaderSource;

    // Vertex Array Object
    // Stores vertex attribute configuration/state
    unsigned int VAO;

    // Vertex Buffer Object
    // Stores vertex data on GPU
    unsigned int VBO;

    // Linked GPU shader program
    unsigned int shaderProgram;

public:
    // Constructor
    Renderer();

    // Render with OpenGL within the window
    void render(SDL_Window *window);

    // Set up OpenGL to render
    // Initializes shaders, buffers, and vertex layout
    void initRenderer();
    
    // Generate the GPU Objects
    // Creates VAO and VBO handles on GPU
    void genGPUObj();

    // Bind Objects to GPU
    // Uploads vertex data into GPU memory
    void bindGPUObj();

    // Describe the vertex layout
    // Explains how vertex data should be interpreted
    void descVerLay();

    // Create the shaderprogram object
    // Compiles shaders and links them into a shader program
    void createShaderProgram();
};

#endif