#ifndef _SCREEN_DRAW_H_
#define _SCREEN_DRAW_H_

void Screen_DrawPixel(Screen_t * screen, ScreenPixelColor_t color, int x, int y); // likely won't ever use a single pixel, likely will use multi for this
void Screen_DrawPixels(Screen_t * screen, ScreenPixelColor_t color, SDL_Point * points, int point_count);

void Screen_DrawLines(Screen_t * screen, ScreenPixelColor_t color, SDL_Point* points, int point_count);

void Screen_DrawRect(Screen_t * screen, ScreenPixelColor_t color, SDL_Rect* rects, int rect_count, bool filled);

#endif //_SCREEN_DRAW_H_
