#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

#include "SDL_Manager.h"
#include "Screen_Scale.h"

#include "GFX_Manager.h"
#include "Screen.h"


#include "Queue.h"

bool Screen_CreateWindow(Screen_t * screen);

bool Screen_Init(Screen_t * screen, uint16_t width, uint16_t height, char* title)
{
    bool ret = false;
    if ((screen != NULL) && (height > 0) && (width > 0))
    {
        // Init struct
        screen->window = NULL;
        screen->renderer = NULL;
        screen->width = width;
        screen->height = height;
        screen->title = title;
        screen->flags = (SDL_WINDOW_SHOWN); // Or flags together
        screen->render_flags = (SDL_RENDERER_ACCELERATED); // SDL_RENDERER_PRESENTVSYNC
        screen->cmd_queue = NULL;
        

        // Init SDL Screen
        if(SDLMgr_IsComponentInitialized(SDLC_VIDEO) != false)
        {
            ret = Screen_CreateWindow(screen);
        }

        if (ret == false) // if fail, cleanup
        {
            Screen_DestroyWindow(screen);
        }
    }
    return ret;
}

// Renderer Example: https://wiki.libsdl.org/SDL_CreateWindowAndRenderer
bool Screen_CreateWindow(Screen_t * screen)
{
    bool ret = false;
    if (screen != NULL)
    {
        screen->cmd_queue = (void*)QueueCreate(sizeof(GFXCmd_t),100);
        if (screen->cmd_queue != NULL)
        {
            // SDL_CreateWindowAndRenderer(screen->width, screen->height, screen->flags, &(screen->window), &(screen->renderer));
            screen->window = SDL_CreateWindow(screen->title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen->width, screen->height, screen->flags);
            if (screen->window != NULL)
            {
                screen->renderer = SDL_CreateRenderer(screen->window, -1, screen->render_flags);
                if (screen->renderer != NULL)
                {
                    // if (screen->title != NULL)
                    // {
                    //     SDL_SetWindowTitle(screen->window, screen->title);
                    // }
                    Screen_SetRendererColor(screen, 0, 0, 0, SDL_ALPHA_OPAQUE);
                    //SDL_FillRect( screen->renderer, NULL, SDL_MapRGB( screen->renderer->format, 0xFF, 0xFF, 0xFF )); // Draw white surface
                    ret = true;
                } else
                {
                    Screen_DestroyWindow(screen);
                }
            }
        }
    }
    return ret;
}

void Screen_SetRendererColor(Screen_t * screen, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
    if ((screen->rendererColor.red != red)||(screen->rendererColor.green != green)
        || (screen->rendererColor.blue != blue)||(screen->rendererColor.alpha != alpha))
    {
        screen->rendererColor.red = red;
        screen->rendererColor.green = green;
        screen->rendererColor.blue = blue;
        screen->rendererColor.alpha = alpha;
        SDL_SetRenderDrawColor(screen->renderer, red, green, blue, alpha);
    }
}
    
void Screen_DestroyWindow(Screen_t * screen)
{
    if (screen != NULL) 
    {
        if (screen->cmd_queue != NULL)
        {
            QueueDestroy(screen->cmd_queue);
            screen->cmd_queue = NULL;
        }
        if (screen->renderer != NULL)
        {
            SDL_DestroyRenderer(screen->renderer);
            screen->renderer = NULL;
        }

        if (screen->window != NULL)
        {
            SDL_DestroyWindow(screen->window);
            screen->window = NULL;
        }
    }
}

bool Screen_PointIsOnScreen(Screen_t * screen, int x, int y)
{
    bool ret = false;
    
    if (screen != NULL)
    {
        if ((x <= screen->width) && (x >= 0) && (y <= screen->height) && (y >= 0))
        {
            ret = true;
        }
    }
    return ret;
}