#include <SDL.h>
#include <stdbool.h>
#include "SDL_Manager.h"
#include "GFX_Manager.h"
#include "Screen_Scale.h"
#include "Screen.h"
#include "Screen_Draw.h"

#include "Queue.h"

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
static int AnimationThread(void * ptr);
static SDL_Thread * anim_thread = NULL;
/* TEMP END */

void GFX_Mgr_ProcessCmd(void);
static int gfx_mgr_thread(void * ptr);

static SDL_Thread * gfx_thread = NULL;
static bool gfx_initialized = false;
static Screen_t screen;


// TEMP
static int AnimationThread(void * ptr)
{
    while (gfx_initialized == false) {}; // wait until launch
    
    GFX_Mgr_SendCmd(GFX_CMD_CLEAR_SCREEN);
    while(gfx_initialized == true)
    {
        static bool is_bg = true;

        if (is_bg != false)
        {
            GFX_Mgr_SendCmd(GFX_CMD_CLEAR_CANVAS);
        } else
        {
            GFX_Mgr_SendCmd(GFX_CMD_DRAW_PIXEL_MAP);
        }

        is_bg = !is_bg;
        SDLMgr_WaitMS(2000); 
    }
    return 0;
}
// END TEMP

void GFX_Mgr_SendCmd(GFXCmd_t cmd) // possibly add screen index for multiple screens later
{
    if (screen.cmd_queue != NULL) // might need check for screen null when multiple screens are implemented
    {
        QueueEnqueue(screen.cmd_queue, &cmd);
    }
}

void GFX_Mgr_ProcessCmd(void)
{
    bool render_needed = false;
    if ((screen.renderer != NULL) && (screen.cmd_queue != NULL))// might need check for screen null when multiple screens are implemented
    {
        GFXCmd_t * cmd = (GFXCmd_t*)QueueDequeue(screen.cmd_queue);
        int pixel_sz = 10;
        int horiz_pixels = ORIG_WIDTH/2/pixel_sz;
        int vert_pixels = ORIG_HEIGHT/2/pixel_sz;
        int num_rects = horiz_pixels*vert_pixels;
        SDL_Rect rects[num_rects];
        
        while (cmd != NULL) // process all commands
        {
            render_needed = true; // at leeast one valid command

            switch ((*cmd))
            {
                case GFX_CMD_CLEAR_SCREEN:
                    // Change color to black and paint entire window
                    Screen_SetRendererColor(&screen, 0, 0, 0, 255);
                    SDL_RenderClear(screen.renderer);
                    break;
                case GFX_CMD_CLEAR_CANVAS:
                    Screen_SetRendererColor(&screen, 200, 200, 200, 255);
                    SDL_Rect bg_rect = Screen_ScaleCreateSDLRectangle(0, 0, ORIG_WIDTH, ORIG_HEIGHT); // need to make screen dimensions a part of the screen struct
                    Screen_DrawRect(&screen, screen.rendererColor, &bg_rect, 1, true);
                    break;
                case GFX_CMD_DRAW_PIXEL_MAP: // change to reading from file
                    Screen_SetRendererColor(&screen, 10, 200, 255, 255);



                    for (int i = 0; i < horiz_pixels; i++) // convert this to filling from a file/buffer
                    {
                        for (int j = 0; j < vert_pixels; j++)
                        {
                            SDL_Rect px_rect = Screen_ScaleCreateSDLRectangle((i*2*pixel_sz), (j*2*pixel_sz), pixel_sz, pixel_sz);
                            int rect_index = i*vert_pixels + j;
                            rects[rect_index] = px_rect;
                        }
                    }
                    Screen_DrawRect(&screen, screen.rendererColor, rects, num_rects, true);
                    break;
                default:
                    break;
            }
            free(cmd);
            cmd = NULL;
            QueueDequeue(screen.cmd_queue);
        }

        if (render_needed != false)
        {
            SDL_RenderPresent(screen.renderer);
            DBG_LOG("Screen Update\n");
        }
    }
    SDLMgr_WaitMS(10);
}

static int gfx_mgr_thread(void * ptr)
{
    int ret = 0;

    gfx_initialized = true;
    
    // STARTUP
    Screen_ScaleInit(SSM_STRETCH, SCREEN_WIDTH, SCREEN_HEIGHT, ORIG_WIDTH, ORIG_HEIGHT);
    
    while (gfx_initialized != false)
    {
        GFX_Mgr_ProcessCmd();
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
    if (Screen_Init(&screen, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE) != false)
    {
        // test thread
        anim_thread = SDL_CreateThread(AnimationThread, "AnimationThread", (void*)NULL);
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
