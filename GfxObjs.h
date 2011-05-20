/* 
 * File:   GfxObjs.h
 * Author: ron
 *
 * Created on February 24, 2011, 5:48 PM
 */

#ifndef _GFXOBJS_H
#define	_GFXOBJS_H

#include "GfxObj.h"
#include "GfxCntr.h"
#include <cmath>
#include <time.h>

using namespace std;

class GfxObjs {
public:
    GfxObjs();
    GfxObjs(const GfxObjs& orig);
    virtual ~GfxObjs();
private:

};

class GfxCircle : public GfxGeoObj {
public:

    GfxCircle() {

    }

    ~GfxCircle() {

    }

    void SetRadius(int radius) {
        this->radius = radius;
    }

    int GetRadius() {
        return this->radius;
    }

    virtual bool Contains(double x, double y) {
        double dist = (pos.X - x)*(pos.X - x)+(pos.Y - y)*(pos.Y - y);
        if ((dist <= radius * radius) && (dist > (radius - penWidth)*(radius - penWidth)))
            return true;
        else
            return false;
    }
private:
    int radius;
};

class GfxChess : public GfxGeoObj {
public:

    GfxChess() {
    }

    ~GfxChess() {
    }

    virtual bool Contains(double x, double y) {
        return ((int) (pos.X + pos.Y + x + y)) % 2;
    }
};

class GfxRectangle : public GfxGeo2DObj {
public:

    GfxRectangle() {
    }

    ~GfxRectangle() {
    }

    virtual bool Contains(double x, double y) {
        if (abs(pos.X - x) <= (size.Width / 2) && abs(pos.Y - y) <= (size.Height / 2)) {
            if (abs(pos.X - x) > (size.Width / 2 - penWidth) || abs(pos.Y - y) > (size.Height / 2 - penWidth))
                return true;
        }
        return false;
    }
};

class GfxLine : public GfxGeoObj {
public:

    GfxLine() {
    }

    ~GfxLine() {
    }

    Position GetRelEndPos() {
        return relEndPos;
    }

    void SetRelEndPos(double x, double y) {
        relEndPos.X = x;
        relEndPos.Y = y;
    }

    virtual bool Contains(double x, double y) {
        if ((x < pos.X - penWidth && x < pos.X + relEndPos.X - penWidth) ||
                (x > pos.X + penWidth && x > pos.X + relEndPos.X + penWidth) ||
                (y < pos.Y - penWidth && y < pos.Y + relEndPos.Y - penWidth) ||
                (y > pos.Y + penWidth && y > pos.Y + relEndPos.Y + penWidth))
            //if((x-pos.X)*(x-pos.X) + (y-pos.Y)*(y-pos.Y) > relEndPos.X*relEndPos.X +relEndPos.Y*relEndPos.Y)
            return false;
        double dx, dy;
        if (abs(relEndPos.Y) < 1)
            dx = y - pos.Y;
        else
            dx = (y - pos.Y) / relEndPos.Y * relEndPos.X - x + pos.X;
        if (abs(relEndPos.X) < 1)
            dy = x - pos.X;
        else
            dy = (x - pos.X) / relEndPos.X * relEndPos.Y - y + pos.Y;

        if (abs(dx) <= penWidth / 2 || abs(dy) <= penWidth / 2)
            return true;
        return false;
    }
protected:
    Position relEndPos;
};

class GfxVector : public GfxGeoObj {
public:

    GfxVector() {
    }

    ~GfxVector() {
    }
protected:
    double degree;
    double length;
};

class GfxClock : public GfxGeoObj {
public:

    GfxClock() {
        cr.SetPenWidth(3);
        cr.SetPosition(28 * 8, 10 * 12);
        cr.SetRadius(115);
        cr2.SetPenWidth(6);
        cr2.SetPosition(28 * 8, 10 * 12);
        cr2.SetRadius(5);
        ls.SetPosition(28 * 8, 10 * 12);
        lm.SetPosition(28 * 8, 10 * 12);
        lh.SetPosition(28 * 8, 10 * 12);
        ls.SetPenWidth(1);
        lm.SetPenWidth(3);
        lh.SetPenWidth(5);
        cnt.Add(&cr);
        cnt.Add(&cr2);
        cnt.Add(&ls);
        cnt.Add(&lm);
        cnt.Add(&lh);
        sec = 22;
        hrs = 20;
        min = 15;
        Update();
    }

    ~GfxClock() {
    }

    virtual bool Contains(double x, double y) {
        return cnt.Contains(x, y);
    }

    void Update() {
        time_t t;
        struct tm *ts;
        t = time(NULL);
        ts = localtime(&t);

        sec = ts->tm_sec;
        min = ts->tm_min;
        hrs = ts->tm_hour;

        ls.SetRelEndPos(105 * sin(sec * 2 * 3.141 / 60), -105 * cos(sec * 2 * 3.141 / 60));
        lm.SetRelEndPos(90 * sin((min + sec / 60.0) * 2 * 3.141 / 60), -100 * cos((min + sec / 60.0) * 2 * 3.141 / 60));
        lh.SetRelEndPos(50 * sin((hrs + min / 60.0) * 2 * 3.141 / 12), -50 * cos((hrs + min / 60.0 ) * 2 * 3.141 / 12));
    }
protected:
    GfxCntr cnt;
    GfxCircle cr;
    GfxCircle cr2;
    GfxLine ls;
    GfxLine lm;
    GfxLine lh;
    int sec;
    int min;
    int hrs;
};

#endif	/* _GFXOBJS_H */

