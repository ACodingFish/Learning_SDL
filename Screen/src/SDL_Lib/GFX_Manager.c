#include <SDL.h>
#include <stdbool.h>
#include "SDL_Manager.h"
#include "GFX_Manager.h"
#include "Screen_Scale.h"
#include "Screen.h"
#include "Screen_Draw.h"

#include "global_def_macros.h"

//Screen dimension constants
#define SCREEN_WIDTH 1024u // need to make this default and read in from a conf file
#define SCREEN_HEIGHT 576u
#define CANVAS_WIDTH 640u
#define CANVAS_HEIGHT 480u
#define ORIG_WIDTH 150u
#define ORIG_HEIGHT 100u
#define WINDOW_TITLE "HELLO NURSE\0"

/* TEMP  - Graphics procesing Ops */
void CreateGraphic(void);
/* TEMP END */

// typedef enum GFXMgrCmd_t_def
// {
//     GFX_CMD_CLEAR_CANVAS,
//     GFX_CMD_DRAW_PIXEL_MAP,
// }GFXMgrCmd_t;

static int gfx_mgr_thread(void * ptr);
static SDL_Thread * gfx_thread = NULL;
static bool gfx_initialized = false;
static Screen_t screen;


void CreateGraphic(void)
{
    static bool is_bg = true;
    ScreenPixelColor_t pix_color = {10, 200, 255, 255};
    ScreenPixelColor_t bg_color = {200, 200, 200, 255};

    int pixel_sz = 10;
    int horiz_pixels = ORIG_WIDTH/2/pixel_sz;
    int vert_pixels = ORIG_HEIGHT/2/pixel_sz;
    int num_rects = horiz_pixels*vert_pixels;

    if (is_bg != false)
    {
        SDL_Rect bg_rect = Screen_ScaleCreateSDLRectangle(0, 0, ORIG_WIDTH, ORIG_HEIGHT);
        Screen_DrawRect(&screen, bg_color, &bg_rect, 1, true);
    } else
    {
        SDL_Rect rects[num_rects];

        for (int i = 0; i < horiz_pixels; i++) // convert this to filling from a file/buffer
        {
            for (int j = 0; j < vert_pixels; j++)
            {
                SDL_Rect px_rect = Screen_ScaleCreateSDLRectangle((i*2*pixel_sz), (j*2*pixel_sz), pixel_sz, pixel_sz);
                int rect_index = i*vert_pixels + j;
                rects[rect_index] = px_rect;
            }
        }
        Screen_DrawRect(&screen, pix_color, rects, num_rects, true);
    }

    is_bg = !is_bg;    
    
}

static int gfx_mgr_thread(void * ptr)
{
    int ret = 0;

    // Change color to black and paint entire window
    Screen_SetRendererColor(&screen, 0, 0, 0, 255);
    SDL_RenderClear(screen.renderer);

    gfx_initialized = true;
    
    // STARTUP
    Screen_ScaleInit(SSM_STRETCH, SCREEN_WIDTH, SCREEN_HEIGHT, ORIG_WIDTH, ORIG_HEIGHT);
    
    while (gfx_initialized != false)
    {
        DBG_LOG("Screen Update\n");
        CreateGraphic(); // Temp for testing
        SDL_RenderPresent(screen.renderer);
        SDLMgr_WaitMS(2000);
    }

    if (ret != 0)
    {
        DBG_ERR("GFX failed with error: %d\n", ret);
    }
    return ret;
}

bool GFX_Mgr_Init(void)
{
    bool ret = false;
    if (Screen_InitWindow(&screen, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE) != false)
    {
        gfx_thread = SDL_CreateThread(gfx_mgr_thread, "gfx_mgr_thread", (void *)NULL);
        if (gfx_thread != NULL)
        {
            ret = true;
        } else
        {
            DBG_ERR("Failed to create thread\n");
            Screen_DestroyWindow(&screen);
        }
    } else
    {
        DBG_ERR("Failed to create screen/window\n");
    }

    return ret;
}

void GFX_Mgr_Shutdown(void)
{
    if (gfx_initialized == true)
    {
        DBG_LOG("Shutting Down GFX ... \n");
        gfx_initialized = false;
        SDL_WaitThread(gfx_thread, NULL);
        Screen_DestroyWindow(&screen);
    }
    DBG_LOG("GFX Down\n");

}
