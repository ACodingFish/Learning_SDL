//Using SDL and standard IO
#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include "SDL_Manager.h"

#include "Queue.h"

#include "global_def_macros.h"


int main(int argc, char* args[])
{
    if (SDLMgr_InitAll() != false)
    {
        SDL_Event event;
        while(1)
        {
            if(SDL_PollEvent(&event) && (event.type == SDL_QUIT)) // Turn into SDL Event Manager Thread
            {
                break;
            }
        }
    } else
    {
        DBG_LOG("Failed to initialize SDL Components\n");
    }

    SDLMGR_Close();

    return 0;
}

