#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

#include "Screen_Scale.h"
#include "Screen.h"
#include "Screen_Draw.h"

//Create a renderer draw queue to process draw commands (so that they are processed sequentially?)

void Screen_DrawPixel(Screen_t * screen, ScreenPixelColor_t color, int x, int y)
{
    if ((screen != NULL) && (screen->renderer != NULL) && (Screen_PointIsOnScreen(screen, x, y) != false))
    {
        Screen_SetRendererColor(screen, color.red, color.green, color.blue, color.alpha);
        SDL_RenderDrawPoint(screen->renderer, x, y);
    }
}

void Screen_DrawPixels(Screen_t * screen, ScreenPixelColor_t color, SDL_Point * points, int point_count)
{
    if ((screen != NULL) && (screen->renderer != NULL) && (points != NULL) && (point_count > 0))
    {
        bool points_valid = true;
        for (int i = 0; i < point_count; i++)
        {
            if (Screen_PointIsOnScreen(screen, points[i].x, points[i].y) == false)
            {
                points_valid = false;
                break;
            }
        }

        if (points_valid != false)
        {
            Screen_SetRendererColor(screen, color.red, color.green, color.blue, color.alpha);
            SDL_RenderDrawPoints(screen->renderer, points, point_count);
        }
    }
}

// Draws a continuous set of lines from point to point
void Screen_DrawLines(Screen_t * screen, ScreenPixelColor_t color, SDL_Point * points, int point_count)
{
    if ((screen != NULL) && (screen->renderer != NULL) && (points != NULL) && (point_count > 0))
    {
        bool points_valid = true;
        for (int i = 0; i < point_count; i++)
        {
            if (Screen_PointIsOnScreen(screen, points[i].x, points[i].y) == false)
            {
                points_valid = false;
                break;
            }
        }

        if (points_valid != false)
        {
            Screen_SetRendererColor(screen, color.red, color.green, color.blue, color.alpha);
            SDL_RenderDrawLines(screen->renderer, points, point_count);
        }
    }
}

void Screen_DrawRect(Screen_t * screen, ScreenPixelColor_t color, SDL_Rect* rects, int rect_count, bool filled)
{
    if ((screen != NULL) && (screen->renderer != NULL) && (rects != NULL) && (rect_count > 0))
    {
        bool points_valid = true;
        for (int i = 0; i < rect_count; i++)
        {
            int right_x = rects[i].x + rects[i].w;
            int lower_y = rects[i].y + rects[i].h;
            if ((Screen_PointIsOnScreen(screen, rects[i].x, rects[i].y) == false)
                && (Screen_PointIsOnScreen(screen, right_x, rects[i].y) == false)
                && (Screen_PointIsOnScreen(screen, right_x, lower_y) == false)
                && (Screen_PointIsOnScreen(screen, rects[i].x, lower_y) == false))
            {
                points_valid = false;
                break;
            }
        }

        if (points_valid != false)
        {
            Screen_SetRendererColor(screen, color.red, color.green, color.blue, color.alpha);
            if (filled == false)
            {
                SDL_RenderDrawRects(screen->renderer, rects, rect_count); // outline only
            } else
            {
                SDL_RenderFillRects(screen->renderer, rects, rect_count); // filled rectangle
            }
        }
    }
}

