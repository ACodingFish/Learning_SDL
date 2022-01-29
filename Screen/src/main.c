//Using SDL and standard IO
#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include "SDL_Manager.h"
#include "Screen.h"
#include "Screen_Draw.h"

#include "global_def_macros.h"
//#include <stdio.h> // for printf

//Screen dimension constants
#define SCREEN_WIDTH 800u // need to make this default and read in from a conf file
#define SCREEN_HEIGHT 600u
#define CANVAS_WIDTH 640u
#define CANVAS_HEIGHT 480u
#define WINDOW_TITLE "HELLO NURSE\0"

int main(int argc, char* args[])
{
    if (SDLMgr_InitAll() != false)
    {
        Screen_t screen;
        if (Screen_InitWindow(&screen, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE))
        {
            ScreenPixelColor_t pix_color = {10, 200, 255, 255};
            ScreenPixelColor_t bg_color = {200, 200, 200, 255};
            //SDL_Point points[SCREEN_WIDTH];
            //for (int i = 0; i < SCREEN_WIDTH; i++)
            //{
            //    points[i].x = i;
            //    points[i].y = (SCREEN_HEIGHT/2);
            //}
            //Screen_DrawPixels(&screen, pix_color, points, SCREEN_WIDTH);
            
            // SDL_Point points[] = {
            //     {SCREEN_WIDTH/4, SCREEN_HEIGHT/4},
            //     {SCREEN_WIDTH*3/4, SCREEN_HEIGHT/4},
            //     {SCREEN_WIDTH*3/4, SCREEN_HEIGHT*3/4},
            //     {SCREEN_WIDTH/4, SCREEN_HEIGHT*3/4},
            // };
            // Screen_DrawLines(&screen, pix_color, points, 4);
            Screen_SetRendererColor(&screen, 0, 0, 0, 255);
            SDL_RenderClear(screen.renderer);



            int pixel_sz = 40;
            int offset_x = (SCREEN_WIDTH - CANVAS_WIDTH)/2;
            int offset_y = (SCREEN_HEIGHT - CANVAS_HEIGHT)/2;
            int horiz_pixels = CANVAS_WIDTH/2/pixel_sz;
            int vert_pixels = CANVAS_HEIGHT/2/pixel_sz;
            int num_rects = horiz_pixels*vert_pixels;

            SDL_Rect bg_rect = {offset_x,offset_y, CANVAS_WIDTH, CANVAS_HEIGHT};
            Screen_DrawRect(&screen, bg_color, &bg_rect, 1, true);

            SDL_Rect rects[num_rects];

            for (int i = 0; i < horiz_pixels; i++) // convert this to filling from a file/buffer
            {
                for (int j = 0; j < vert_pixels; j++)
                {
                    int rect_index = i*vert_pixels + j;
                    rects[rect_index].x = (i*2*pixel_sz) + offset_x;
                    rects[rect_index].y = (j*2*pixel_sz) + offset_y;
                    rects[rect_index].w = pixel_sz;
                    rects[rect_index].h = pixel_sz;
                }
            }
            Screen_DrawRect(&screen, pix_color, rects, num_rects, true);
            
            SDL_RenderPresent(screen.renderer);
            
            
            //SDLMgr_WaitMS(2000);
            SDL_Event event;
            while(1)
            {
                if(SDL_PollEvent(&event) && (event.type == SDL_QUIT)) // Turn into SDL Event Manager Thread
                {
                    Screen_DestroyWindow(&screen);
                    break;
                }
            }
        } else
        {
            DBG_LOG("Failed to create screen/window\n");
        }
    } else
    {
        DBG_LOG("Failed to initialize SDL Components\n");
    }

    SDLMGR_Close();

    return 0;
}