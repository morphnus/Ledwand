/* 
 * File:   GfxObj.h
 * Author: ron
 *
 * Created on February 3, 2011, 6:15 PM
 */

#ifndef _GFXOBJ_H
#define	_GFXOBJ_H

struct Position {
    double X;
    double Y;
};

struct GfSize {
    double Width;
    double Height;
};

class GfxObj {
public:

    GfxObj() {
        pos.X = 0;
        pos.Y = 0;

        buf_off_x = buf_off_y = buf_width = buf_height = buf_res_h = buf_res_v = 0;
        buffer = 0;
    }
    GfxObj(const GfxObj& orig);
    virtual ~GfxObj();
    virtual bool Contains(double, double) = 0;

    void Move(double off_x, double off_y) {
        pos.X += off_x;
        pos.Y += off_y;
    };

    void SetPosition(double x, double y) {
        pos.X = x;
        pos.Y = y;
    };

    Position GetPosition() {
        return pos;
    };

    void RenderBuffer() {
        if (buffer != 0)
            delete buffer;
        int buf_w = (int) (buf_width / buf_res_h);
        int buf_h = (int) (buf_height / buf_res_v);

        buffer = new bool[1];
        for (int x = pos.X - buf_off_x; x < pos.X - buf_off_x + buf_width; x += buf_res_h) {

        }
    }


    //virtual void Resize(double) = 0;
    //virtual bool Contains(int,int) = 0;
    //virtual void Rotate(double) = 0;
protected:
    struct Position pos;
    bool * buffer;
    double buf_off_x, buf_off_y, buf_width, buf_height, buf_res_h, buf_res_v;

};

class GfxGeoObj : public GfxObj {
public:

    GfxGeoObj() {
        penWidth = 2;
    }

    ~GfxGeoObj() {
    }

    void SetPenWidth(double width) {
        penWidth = width;
    }

    double GetPenWidth() {
        return penWidth;
    }
protected:
    double penWidth;
};

class GfxGeo2DObj : public GfxGeoObj {
public:

    GfxGeo2DObj() {
    }

    ~GfxGeo2DObj() {
    }

    void SetWidth(double width) {
        size.Width = width;
    }

    double GetWidth() {
        return size.Width;
    }

    void SetHeight(double height) {
        size.Height = height;
    }

    double GetHeight() {
        return size.Height;
    }
protected:
    GfSize size;
};


#endif	/* _GFXOBJ_H */

