#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

#include "global_def_macros.h"
#include "GFX_Manager.h"

#include "SDL_Manager.h"
//#include <stdio.h>


bool SDLMgr_InitComponents(void);
bool SDLMgr_InitComponent(SDL_Component_t sdl_component);
void SDLMgr_CloseComponent(SDL_Component_t sdl_component);

static uint32_t sdl_init_mask = 0x0; // Component Mask

// Initialize All Components
bool SDLMgr_InitAll(void)
{
    bool ret = false;
    if ((SDLMgr_InitComponents() != false) && (GFX_Mgr_Init() != false))
    {
        ret = true;
    }
    return ret;
}

bool SDLMgr_InitComponents(void)
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
            ret = (SDL_InitSubSystem( SDL_INIT_VIDEO ) == 0);
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
    bool ret = false;
    if (sdl_component < SDLC_COUNT)
    {
        ret = MASK_BIT_CHK(sdl_init_mask, (uint8_t)sdl_component);
    }
    return ret;
}

void SDLMgr_CloseComponent(SDL_Component_t sdl_component)
{ 
    bool is_initialized = SDLMgr_IsComponentInitialized(sdl_component);
    if (is_initialized != false)
    {
        switch (sdl_component)
        {
            case SDLC_VIDEO:
                SDL_QuitSubSystem( SDL_INIT_VIDEO );
                break;
            default:
                break;
        }

        MASK_BIT_CLR(sdl_init_mask, (uint8_t)sdl_component);
        DBG_LOG("SDL Component: %d shutdown\n", (uint8_t)sdl_component);
    } else
    {
        DBG_ERR("Component %d is not initialized.\n", (uint8_t)sdl_component);
    }
}

void SDLMGR_Close(void)
{
    GFX_Mgr_Shutdown();

    for (uint8_t i = 0; i < (uint8_t)SDLC_COUNT; i++)
    {
        SDLMgr_CloseComponent((SDL_Component_t)i);
    }

    SDL_Quit();
}
