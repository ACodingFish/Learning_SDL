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

void Screen_ScaleInit(ScreenScaleMode_t mode, int scr_w, int scr_h, int orig_w, int orig_h);
void Screen_ScalePointToCanvas(ScreenScalePoint_t * point); // scales from original resolution to the canvas
void Screen_ScaleSwitchMode(ScreenScaleMode_t mode);
void Screen_ScaleSwitchScreenResolution(int scr_w, int scr_h, bool refresh_mode);
void Screen_ScaleSwitchOrigResolution(int orig_w, int orig_h, bool refresh_mode);

SDL_Rect Screen_ScaleCreateSDLRectangle(int x, int y, int w, int h);
SDL_Rect Screen_ScaleSDLRectangle(SDL_Rect rect);

#endif //_SCREEN_SCALE_H_
