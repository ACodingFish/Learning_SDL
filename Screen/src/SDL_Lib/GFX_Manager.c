#include <SDL.h>
#include <stdbool.h>
#include "SDL_Manager.h"
#include "GFX_Manager.h"

#include "global_def_macros.h"

static int gfx_mgr_thread(void * ptr);
SDL_Thread * gfx_thread = NULL;
bool gfx_initialized = false;


static int gfx_mgr_thread(void * ptr)
{
    int ret = 0;
    gfx_initialized = true;
    while (gfx_initialized != false)
    {
        DBG_LOG("Screen Update\n");
        SDLMgr_WaitMS(2000);
    }
    // Cleanup here
    if (ret != 0)
    {
        DBG_ERR("GFX failed with error: %d\n", ret);
    }
    return ret;
}

bool GFX_Mgr_Init(void)
{
    gfx_thread = SDL_CreateThread(gfx_mgr_thread, "gfx_mgr_thread", (void *)NULL);

    return (gfx_thread != NULL);
}

void GFX_Mgr_Shutdown(void)
{
    DBG_LOG("Shutting Down GFX ... \n");
    gfx_initialized = false;
    SDL_WaitThread(gfx_thread, NULL);
    DBG_LOG("GFX Down\n");

}
