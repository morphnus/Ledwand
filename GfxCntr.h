/* 
 * File:   GfxCntr.h
 * Author: ron
 *
 * Created on February 24, 2011, 10:25 PM
 */

#ifndef _GFXCNTR_H
#define	_GFXCNTR_H

#include <list>

#include "GfxObj.h"

using namespace std;

enum CntrMd {
    XOR, Norm
};

class GfxCntr {
public:

    GfxCntr(){
        mode = Norm;
    }
    GfxCntr(const GfxCntr& orig);

    void Add(GfxObj* obj) {
        gfxObjList.push_back(obj);
    }

    bool Contains(double x, double y) {
        if (mode == Norm) {
            for (list<GfxObj*>::iterator it = gfxObjList.begin(); it != gfxObjList.end(); it++) {
                if ((*it)->Contains(x, y))
                    return true;
            }
            return false;
        } else if(mode == XOR) {
            bool result = false;
            for (list<GfxObj*>::iterator it = gfxObjList.begin(); it != gfxObjList.end(); it++) {
                if ((*it)->Contains(x, y))
                    result = result? false:true;
            }
            return result;
        }
        return false;
    }

    virtual ~GfxCntr();
private:
    list<GfxObj*> gfxObjList;
    CntrMd mode;

};

class CoordSys{

};

#endif	/* _GFXCNTR_H */

