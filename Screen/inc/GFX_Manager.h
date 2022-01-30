#ifndef _GFX_MANAGER_H_
#define _GFX_MANAGER_H_

typedef enum GFXCmd_t_def
{
    GFX_CMD_CLEAR_SCREEN,
    GFX_CMD_CLEAR_CANVAS,
    GFX_CMD_DRAW_PIXEL_MAP,
}GFXCmd_t;

void GFX_Mgr_SendCmd(GFXCmd_t cmd);

bool GFX_Mgr_Init(void);
void GFX_Mgr_Shutdown(void);

#endif //_GFX_MANAGER_H_