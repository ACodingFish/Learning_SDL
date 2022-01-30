#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

#include "SDL_Manager.h"
#include "Screen_Scale.h"
#include "Screen.h"

bool Screen_CreateWindow(Screen_t * screen);

bool Screen_InitWindow(Screen_t * screen, uint16_t width, uint16_t height, char* title)
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

        // Init SDL Screen
        if(SDLMgr_IsComponentInitialized(SDLC_VIDEO) != false)
        {
            ret = Screen_CreateWindow(screen);
        }
    }
    return ret;
}

// Renderer Example: https://wiki.libsdl.org/SDL_CreateWindowAndRenderer
bool Screen_CreateWindow(Screen_t * screen)
{
    bool ret = false;
    SDL_CreateWindowAndRenderer(screen->width, screen->height, screen->flags, &(screen->window), &(screen->renderer));
    if ((screen->window != NULL) && (screen->renderer != NULL))
    {
        if (screen->title != NULL)
        {
            SDL_SetWindowTitle(screen->window, screen->title);
        }
            Screen_SetRendererColor(screen, 0, 0, 0, SDL_ALPHA_OPAQUE);
            //SDL_FillRect( screen->renderer, NULL, SDL_MapRGB( screen->renderer->format, 0xFF, 0xFF, 0xFF )); // Draw white surface

            //SDL_UpdateWindowSurface(screen->window); // Update surface
            // draw here?
            ret = true;
    } else
    {
        Screen_DestroyWindow(screen);
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
        if (screen->renderer != NULL)
        {
            SDL_DestroyRenderer(screen->renderer);
        }

        if (screen->window != NULL)
        {
            SDL_DestroyWindow(screen->window);
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