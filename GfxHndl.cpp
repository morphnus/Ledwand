/* 
 * File:   GfxHndl.cpp
 * Author: ron
 * 
 * Created on January 27, 2011, 6:07 PM
 */

#include "GfxHndl.h"

GfxHndl::GfxHndl(char* buff) {
    gfx_buff = buff;
    gfx_mode = GFX_LINE;
}

GfxHndl::GfxHndl(const GfxHndl& orig) {
}

GfxHndl::~GfxHndl() {
}

void GfxHndl::LED_Set(int x, int y, bool state) {
    int mod_height = (gfx_mode == GFX_LINE) ? MODULE_HEIGHT_SPACE : MODULE_HEIGHT;
    if (x >= MODULE_WIDTH * COLS)
        x = MODULE_WIDTH * COLS - 1;
    if (y >= mod_height * ROWS)
        y = mod_height * ROWS - 1;
    if (y % mod_height >= MODULE_HEIGHT) return;
    if (state)
        gfx_buff[(y / mod_height) * COLS * MODULE_WIDTH + x] |= (1 << (7 - (y % mod_height)));
    else
        gfx_buff[(y / mod_height) * COLS * MODULE_WIDTH + x] &= ~(1 << (7 - (y % mod_height)));
}

void GfxHndl::DrawLine(int x1, int y1, int x2, int y2) {
    if (x1 != x2) {
        if (x1 > x2) {
            int tmp = x1;
            x1 = x2;
            x2 = tmp;
            tmp = y1;
            y1 = y2;
            y2 = tmp;
        }
        int xsteps = x2 - x1;
        int ysteps = y2 - y1;
        float dy = ((float) ysteps) / ((float) xsteps);
        float y = y1;
        for (int x = x1; x < x2; x++) {
            if (dy > 0) {
                for (float ys = y; ys < y + dy; ys++)
                    LED_Set(x, (int) ys, true);
                y += dy;
            } else if (dy < 0) {
                for (float ys = y; ys > y + dy; ys--)
                    LED_Set(x, (int) ys, true);
                y += dy;
            } else {
                LED_Set(x, y, true);
                y += dy;
            }
        }

    } else {
        if (y1 > y2) {
            int tmp = y1;
            y1 = y2;
            y2 = tmp;
        }
        for (int y = y1; y < y2; y++)
            LED_Set(x1, y, true);
    }
}

int GfxHndl::GetWidth() {
    return MODULE_WIDTH * COLS;
}

int GfxHndl::GetHeight() {
    int mod_height = (gfx_mode == GFX_LINE) ? MODULE_HEIGHT_SPACE : MODULE_HEIGHT;
    return mod_height * ROWS;
}

void GfxHndl::DrawCircle(int x_m, int y_m, int r, int pen_width) {
    int width = GetWidth();
    int height = GetHeight();
    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++) {
            double dist = (x_m - x)*(x_m - x)+(y_m - y)*(y_m - y);
            if ((dist <= r * r) && (dist > (r - pen_width)*(r - pen_width)))
                LED_Set(x, y, true);
        }

}

void GfxHndl::DrawContainer(GfxCntr* cnt) {
    int width = GetWidth();
    int height = GetHeight();
    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++) {
            if (cnt->Contains(x, y))
                LED_Set(x, y, true);
        }
}

void GfxHndl::DrawEllipse(int x_m, int y_m, int r_x, int r_y, int pen_width) {
    int width = GetWidth();
    int height = GetHeight();
    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++) {
            double dist1 = (x_m - x)*(x_m - x) * r_y * r_y + (y_m - y)*(y_m - y) * r_x*r_x;
            double dist2 = (x_m - x)*(x_m - x) *(r_y - pen_width) *(r_y - pen_width) +
                    (y_m - y)*(y_m - y) * (r_x - pen_width)*(r_x - pen_width);
            if ((dist1 <= r_x * r_x * r_y * r_y) &&
                    (dist2 > (r_x - pen_width)*(r_x - pen_width)*
                    (r_y - pen_width)*(r_y - pen_width)))
                LED_Set(x, y, true);
        }

}

