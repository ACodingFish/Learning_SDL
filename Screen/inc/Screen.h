#ifndef _SCREEN_H_
#define _SCREEN_H_

// Drawing Setup
typedef struct ScreenPixelColor_t_def // RGB
{
    uint8_t red;
    uint8_t blue;
    uint8_t green;
    uint8_t alpha; // Opacity - Recommend to SDL_ALPHA_OPAQUE (255)
} ScreenPixelColor_t;

// SDL Screen Struct
typedef struct Screen_t_def
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    uint16_t width;
    uint16_t height;
    char * title;
    uint32_t flags;
    uint32_t render_flags;
    ScreenPixelColor_t rendererColor;
    void * cmd_queue;
} Screen_t;

bool Screen_Init(Screen_t * screen, uint16_t width, uint16_t height, char* title);
void Screen_DestroyWindow(Screen_t * screen);
void Screen_SetRendererColor(Screen_t * screen, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
bool Screen_PointIsOnScreen(Screen_t * screen, int x, int y);



#endif //_SCREEN_H_
