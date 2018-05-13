//
// Created by sjwang on 6/5/2018.
//

#ifndef DT_EDGE_H
#define DT_EDGE_H

#include "point.h"


class Edge
{
public:

    Edge( Point2 _p1,  Point2 _p2) : p1(_p1), p2(_p2) {};
    Edge( Edge &e) : p1(e.p1), p2(e.p2){};

    inline void setPoint(Point2 _p1,  Point2 _p2){
        p1 = _p1;
        p2 = _p2;
    }
    inline bool iscontain(Point2 _p1){
        if ((p1 == _p1)||(p2 == _p1))
            return true;
        else{
            return false;
        }

    }
    Point2 p1;
    Point2 p2;
};



#endif //DT_EDGE_H
