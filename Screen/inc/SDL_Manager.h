#ifndef _SDL_MANAGER_H_
#define _SDL_MANAGER_H_

typedef enum SDL_Component_t_def // SDL Init Component Enum
{
    SDLC_VIDEO,

    SDLC_COUNT,
}SDL_Component_t;

void SDLMgr_WaitMS(uint32_t time_ms);
bool SDLMgr_InitAll(void);
bool SDLMgr_IsComponentInitialized(SDL_Component_t sdl_component);
void SDLMGR_Close(void);

#endif //_SDL_MANAGER_H_
