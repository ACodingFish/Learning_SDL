//Using SDL and standard IO
#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include "SDL_Manager.h"

#include "Queue.h"

#include "global_def_macros.h"
//#include <stdio.h> // for printf

int main(int argc, char* args[])
{
    Queue_t * q = QueueCreate(sizeof(int), 3);
    if (SDLMgr_InitAll() != false)
    {
        
        for (int i = 0; i < 5; i++)
        {
            QueueEnqueue(q, &i);
        }

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

    QueueDestroy(q);
    SDLMGR_Close();

    return 0;
}