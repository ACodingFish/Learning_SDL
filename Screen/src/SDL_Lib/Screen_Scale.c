#include <SDL.h>
#include <stdbool.h>
#include "Screen_Scale.h"

#include "global_def_macros.h"

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

void Screen_ScaleGetCanvasDims(void * sdim_ptr);
int Sreen_ScaleLinearTransformation(int val, int old_min, int old_max, int new_min, int new_max);

//static ScreenScaleDimension_t sdim = {600, 480, 600, 480, 600, 480, 0, 0, 0.0, 0.0, SSM_CENTER};

void * Screen_ScaleCreate(ScreenScaleMode_t mode, int scr_w, int scr_h, int orig_w, int orig_h)
{
    ScreenScaleDimension_t *sdim = malloc(sizeof(ScreenScaleDimension_t));
    Screen_ScaleSwitchScreenResolution((void *)sdim, scr_w, scr_h, false);
    Screen_ScaleSwitchOrigResolution((void*)sdim, orig_w, orig_h, false);
    Screen_ScaleSwitchMode((void*)sdim, mode);
    return (void*)sdim;
}

void Screen_ScaleDestroy(void * sdim)
{
    if (sdim != NULL)
    {
        free(sdim);
    }
}

void Screen_ScaleSwitchScreenResolution(void * sdim_ptr, int scr_w, int scr_h, bool refresh_mode) // refresh mode unless initializing
{
    if (sdim_ptr != NULL)
    {
        ScreenScaleDimension_t* sdim = (ScreenScaleDimension_t*)sdim_ptr;
        sdim->scr_w = scr_w;
        sdim->scr_h = scr_h;
        sdim->new_ar = (float)sdim->scr_w/(float)sdim->scr_h;
        if (refresh_mode != false)
        {
            Screen_ScaleSwitchMode(sdim_ptr, sdim->scale_mode); //Refresh mode parameters
        }
    } else
    {
        DBG_ERR("Null Scale Mode\n");
    }
}

void Screen_ScaleSwitchOrigResolution(void * sdim_ptr, int orig_w, int orig_h, bool refresh_mode) // refresh mode unless initializing
{
    if (sdim_ptr != NULL)
    {
        ScreenScaleDimension_t* sdim = (ScreenScaleDimension_t*)sdim_ptr;
        sdim->orig_w = orig_w;
        sdim->orig_h = orig_h;
        sdim->orig_ar = (float)sdim->orig_w/(float)sdim->orig_h;
        if (refresh_mode != false)
        {
            Screen_ScaleSwitchMode(sdim_ptr, sdim->scale_mode); //Refresh mode parameters
        }
    } else
    {
        DBG_ERR("Null Scale Mode\n");
    }
}

void Screen_ScaleSwitchMode(void * sdim_ptr, ScreenScaleMode_t mode)
{
    if (sdim_ptr != NULL)
    {
        ScreenScaleDimension_t* sdim = (ScreenScaleDimension_t*)sdim_ptr;
        sdim->scale_mode = mode;
        Screen_ScaleGetCanvasDims(sdim_ptr);
        sdim->offset_x = (sdim->scr_w - sdim->cvs_w)/2;
        sdim->offset_y = (sdim->scr_h - sdim->cvs_h)/2;
    } else
    {
        DBG_ERR("Null Scale Mode\n");
    }
}

// Fill Canvas and Offset parameters
void Screen_ScaleGetCanvasDims(void * sdim_ptr) // only supports up-scaling, not down-scaling
{
    if (sdim_ptr != NULL)
    {
        ScreenScaleDimension_t* sdim = (ScreenScaleDimension_t*)sdim_ptr;
        switch (sdim->scale_mode)
        {
            default:
            case SSM_CENTER:
                sdim->cvs_w = sdim->orig_w;
                sdim->cvs_h = sdim->orig_h;
                break;
            case SSM_SCALE:

                if (sdim->new_ar >= 1.0) // Width is greater, base on height
                {
                    sdim->cvs_w = (int)((float)sdim->scr_h*sdim->orig_ar);
                    sdim->cvs_h = sdim->scr_h;
                } else
                {
                    sdim->cvs_w = sdim->scr_w;
                    sdim->cvs_h = (int)((float)sdim->scr_w/sdim->orig_ar);
                }
                break;
            case SSM_STRETCH:
                sdim->cvs_w = sdim->scr_w;
                sdim->cvs_h = sdim->scr_h;
                break;
        }
        DBG_LOG("Screen Switched Scaling Mode: %d\n", (int)sdim->scale_mode);
    } else
    {
        DBG_ERR("Null Scale Mode\n");
    }
}

int Sreen_ScaleLinearTransformation(int val, int old_min, int old_max, int new_min, int new_max) // Map from old to new
{
    return ((val - old_min)*(new_max - new_min))/(old_max - old_min) + new_min;
}

// Transform a point from original grid to the canvas
void Screen_ScalePointToCanvas(void * sdim_ptr, ScreenScalePoint_t * point)
{
    if (sdim_ptr != NULL)
    {
        ScreenScaleDimension_t* sdim = (ScreenScaleDimension_t*)sdim_ptr;
        point->x = Sreen_ScaleLinearTransformation(point->x, 0, sdim->orig_w, sdim->offset_x, (sdim->offset_x + sdim->cvs_w));
        point->y = Sreen_ScaleLinearTransformation(point->y, 0, sdim->orig_h, sdim->offset_y, (sdim->offset_y + sdim->cvs_h));
    } else
    {
        DBG_ERR("Null Scale Mode\n");
    }
}

SDL_Rect Screen_ScaleCreateSDLRectangle(void * sdim_ptr, int x, int y, int w, int h)
{
    SDL_Rect rect;
    if (sdim_ptr != NULL)
    {
        ScreenScalePoint_t tl_point = {x, y};   // top left point
        ScreenScalePoint_t br_point = {(x+w), (y+h)}; // bottom right point
        Screen_ScalePointToCanvas(sdim_ptr, &tl_point);
        Screen_ScalePointToCanvas(sdim_ptr, &br_point);
        rect.x = tl_point.x;
        rect.y = tl_point.y;
        rect.w = br_point.x - rect.x;
        rect.h = br_point.y - rect.y;
    } else
    {
        rect.x = x;
        rect.y = y;
        rect.w = w;
        rect.h = h;
        DBG_ERR("Null Scale Mode\n");
    }
    return rect;
}

SDL_Rect Screen_ScaleSDLRectangle(void * sdim_ptr, SDL_Rect rect)
{
    return Screen_ScaleCreateSDLRectangle(sdim_ptr, rect.x, rect.y, rect.w, rect.h);
}

SDL_Rect Screen_ScaleGetCanvasSDLRectangle(void * sdim_ptr)
{
    SDL_Rect rect;
    if (sdim_ptr != NULL)
    {
        ScreenScaleDimension_t* sdim = (ScreenScaleDimension_t*)sdim_ptr;
        rect = Screen_ScaleCreateSDLRectangle(sdim_ptr, 0, 0, sdim->orig_w, sdim->orig_h);
    } else
    {
        rect.x = 0;
        rect.y = 0;
        rect.w = 1;
        rect.h = 1;
        DBG_ERR("Null Scale Mode\n");
    }
    return rect;
}