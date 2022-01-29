//Using SDL and standard IO
#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include "SDL_Manager.h"
#include "Screen.h"
#include "Screen_Draw.h"
#include "GFX_Manager.h"

#include "global_def_macros.h"
//#include <stdio.h> // for printf

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