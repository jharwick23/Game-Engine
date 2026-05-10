#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

// Class for window input
class WindowInput
{
private:
public:
    static void processInput(bool &isRunning);
};

#endif