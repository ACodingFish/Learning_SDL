#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

#include "bit_macros.h"

#include "SDL_Manager.h"

#include <stdio.h>



bool SDLMgr_InitComponent(SDL_Component_t sdl_component);

static uint32_t sdl_init_mask = 0x0; // Component Mask

void SDLMgr_WaitMS(uint32_t time_ms)
{
    SDL_Delay(time_ms);
}

// Initialize All Components
bool SDLMgr_InitAll(void)
{
    bool ret = true;
    sdl_init_mask = 0x0;

    for (uint8_t i = 0; i < (uint8_t)SDLC_COUNT; i++)
    {
        SDLMgr_InitComponent((SDL_Component_t)i);
        if (SDLMgr_IsComponentInitialized((SDL_Component_t)i) == false) // Check if all components are initialized
        {   
            ret = false;
            break;
        }
    }
    return ret;
}

// Initialize a Single Component
bool SDLMgr_InitComponent(SDL_Component_t sdl_component)
{
    bool ret = false; 
    switch (sdl_component)
    {
        case SDLC_VIDEO:
            ret = (SDL_Init( SDL_INIT_VIDEO ) == 0);
            break;
        default:
            break;
    }

    if (ret != false)
    {
        MASK_BIT_SET(sdl_init_mask, (uint8_t)sdl_component);
    }

    return ret;
}

bool SDLMgr_IsComponentInitialized(SDL_Component_t sdl_component)
{
    return MASK_BIT_CHK(sdl_init_mask, (uint8_t)sdl_component);
}

void SDLMGR_Close(void)
{
    SDL_Quit();
}
