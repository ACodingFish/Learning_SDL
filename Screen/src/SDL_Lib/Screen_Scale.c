#include <SDL.h>
#include <stdbool.h>
#include "Screen_Scale.h"

typedef struct ScreenScaleDimension_t_def
{
    int scr_w;  //Screen width (Full Screen - Native Resolution)
    int scr_h;  //Screen height
    int orig_w; // Non-scaled area
    int orig_h;
    int cvs_w;  //Canvas width (Scaled Area - Original resolution)
    int cvs_h;  //Canvas height
    int offset_x;   //canvas X offset from left
    int offset_y;   //canvas Y offset from top
    float new_ar; // screen aspect ratio
    float orig_ar; // non-scaled aspect ratio
    ScreenScaleMode_t scale_mode;

} ScreenScaleDimension_t;

void Screen_ScaleGetCanvasDims(void);
int Sreen_ScaleLinearTransformation(int val, int old_min, int old_max, int new_min, int new_max);

static ScreenScaleDimension_t sdim = {600, 480, 600, 480, 600, 480, 0, 0, 0.0, 0.0, SSM_CENTER};

void Screen_ScaleInit(ScreenScaleMode_t mode, int scr_w, int scr_h, int orig_w, int orig_h)
{


    Screen_ScaleSwitchScreenResolution(scr_w, scr_h, false);
    Screen_ScaleSwitchOrigResolution(orig_w, orig_h, false);
    Screen_ScaleSwitchMode(mode);

    // int pixel_sz = 40;
    // int horiz_pixels = sdim.cvs_w/2/pixel_sz;
    // int vert_pixels = sdim.cvs_h/2/pixel_sz;
    // int num_rects = horiz_pixels*vert_pixels;
}

void Screen_ScaleSwitchScreenResolution(int scr_w, int scr_h, bool refresh_mode) // refresh mode unless initializing
{
    sdim.scr_w = scr_w;
    sdim.scr_h = scr_h;
    sdim.new_ar = (float)sdim.scr_w/(float)sdim.scr_h;
    if (refresh_mode != false)
    {
        Screen_ScaleSwitchMode(sdim.scale_mode); //Refresh mode parameters
    }
}

void Screen_ScaleSwitchOrigResolution(int orig_w, int orig_h, bool refresh_mode) // refresh mode unless initializing
{
    sdim.orig_w = orig_w;
    sdim.orig_h = orig_h;
    sdim.orig_ar = (float)sdim.orig_w/(float)sdim.orig_h;
    if (refresh_mode != false)
    {
        Screen_ScaleSwitchMode(sdim.scale_mode); //Refresh mode parameters
    }
}

void Screen_ScaleSwitchMode(ScreenScaleMode_t mode)
{
    sdim.scale_mode = mode;
    Screen_ScaleGetCanvasDims();
    sdim.offset_x = (sdim.scr_w - sdim.cvs_w)/2;
    sdim.offset_y = (sdim.scr_h - sdim.cvs_h)/2;
}

// Fill Canvas and Offset parameters
void Screen_ScaleGetCanvasDims(void) // only supports up-scaling, not down-scaling
{
    switch (sdim.scale_mode)
    {
        default:
        case SSM_CENTER:
            sdim.cvs_w = sdim.orig_w;
            sdim.cvs_h = sdim.orig_h;
            break;
        case SSM_SCALE:

            if (sdim.new_ar >= 1.0) // Width is greater, base on height
            {
                sdim.cvs_w = (int)((float)sdim.scr_h*sdim.orig_ar);
                sdim.cvs_h = sdim.scr_h;
            } else
            {
                sdim.cvs_w = sdim.scr_w;
                sdim.cvs_h = (int)((float)sdim.scr_w/sdim.orig_ar);
            }
            break;
        case SSM_STRETCH:
            sdim.cvs_w = sdim.scr_w;
            sdim.cvs_h = sdim.scr_h;
            break;
    }
}

int Sreen_ScaleLinearTransformation(int val, int old_min, int old_max, int new_min, int new_max) // Map from old to new
{
    return ((val - old_min)*(new_max - new_min))/(old_max - old_min) + new_min;
}

// Transform a point from original grid to the canvas
void Screen_ScalePointToCanvas(ScreenScalePoint_t * point)
{
    point->x = Sreen_ScaleLinearTransformation(point->x, 0, sdim.orig_w, sdim.offset_x, (sdim.offset_x + sdim.cvs_w));
    point->y = Sreen_ScaleLinearTransformation(point->y, 0, sdim.orig_h, sdim.offset_y, (sdim.offset_y + sdim.cvs_h));
}

SDL_Rect Screen_ScaleCreateSDLRectangle(int x, int y, int w, int h)
{
    SDL_Rect rect;

    ScreenScalePoint_t tl_point = {x, y};   // top left point
    ScreenScalePoint_t br_point = {(x+w), (y+h)}; // bottom right point
    Screen_ScalePointToCanvas(&tl_point);
    Screen_ScalePointToCanvas(&br_point);
    rect.x = tl_point.x;
    rect.y = tl_point.y;
    rect.w = br_point.x - rect.x;
    rect.h = br_point.y - rect.y;
    return rect;
}

SDL_Rect Screen_ScaleSDLRectangle(SDL_Rect rect)
{
    return Screen_ScaleCreateSDLRectangle(rect.x, rect.y, rect.w, rect.h);
}