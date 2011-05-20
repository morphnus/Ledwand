/* 
 * File:   GfxHndl.h
 * Author: ron
 *
 * Created on January 27, 2011, 6:07 PM
 */

#ifndef _GFXHNDL_H
#define	_GFXHNDL_H

#define GFX_LINE 0x0001
#define GFX_NORM 0x0002
#define GFX_ADD 0x0008
#define GFX_SUB 0x0010
#define GFX_INVE 0x0020

#include "LedWand.h"
#include "GfxCntr.h"
#include <math.h>

class GfxHndl {
public:
    GfxHndl(char* buff);
    GfxHndl(const GfxHndl& orig);
    virtual ~GfxHndl();
    void LED_Set(int,int,bool = true);
    void DrawLine(int,int,int,int);
    void DrawLine(int,int,int,int,int);
    void DrawCircle(int,int,int,int=1);
    void DrawEllipse(int,int,int,int,int=1);
    void DrawRectangle(int,int,int,int,int=1);
    void DrawContainer(GfxCntr* cnt);
    void FillCircle(int,int,int);
    void FillEllipse(int,int,int,int);
    void FillRectangle(int,int,int,int);
    void Clear();
    void Inverse();
    void DrawBitmap();
    int GetWidth();
    int GetHeight();
private:
    char* gfx_buff;
    int gfx_mode;

};

#endif	/* _GFXHNDL_H */

