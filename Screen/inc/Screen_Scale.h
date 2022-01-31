#ifndef _SCREEN_SCALE_H_
#define _SCREEN_SCALE_H_

typedef enum ScreenScaleMode_t_def
{
    SSM_CENTER, // Centered (not scaled)
    SSM_SCALE, // Scaled (evenly stretched)
    SSM_STRETCH, // Stretched (Fit / Stretched and skewed)

    SSM_COUNT,
} ScreenScaleMode_t;

typedef struct ScreenScalePoint_t_def
{
    int x;
    int y;
} ScreenScalePoint_t;


void * Screen_ScaleCreate(ScreenScaleMode_t mode, int scr_w, int scr_h, int orig_w, int orig_h);
void Screen_ScaleDestroy(void * sdim);
void Screen_ScalePointToCanvas(void * sdim_ptr, ScreenScalePoint_t * point); // scales from original resolution to the canvas
void Screen_ScaleSwitchMode(void * sdim_ptr, ScreenScaleMode_t mode);
void Screen_ScaleSwitchScreenResolution(void * sdim_ptr, int scr_w, int scr_h, bool refresh_mode);
void Screen_ScaleSwitchOrigResolution(void * sdim_ptr, int orig_w, int orig_h, bool refresh_mode);

SDL_Rect Screen_ScaleCreateSDLRectangle(void * sdim_ptr, int x, int y, int w, int h);
SDL_Rect Screen_ScaleSDLRectangle(void * sdim_ptr, SDL_Rect rect);
SDL_Rect Screen_ScaleGetCanvasSDLRectangle(void * sdim_ptr);

#endif //_SCREEN_SCALE_H_
