#include <iostream>

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include "../input/input.h"
#include "../render/render.h"

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

    SDL_SetWindowResizable(window, SDL_TRUE);

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

    Renderer renderer;

    renderer.initRenderer();

    // Print OpenGL version
    std::cout << "OpenGL Version: "
              << glGetString(GL_VERSION)
              << std::endl;

    // Main loop for window
    bool running = true;

    while(running)
    {
        WindowInput::processInput(running);

        renderer.render(window);
    }

    // Cleanup
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}