//
// Created by sjwang on 6/5/2018.
//

#ifndef DT_TRIANGLE_H
#define DT_TRIANGLE_H


#include "point.h"
#include "edge.h"
#include "curPoint.h"

#include <assert.h>
#include <math.h>

using namespace std;

class Triangle
{
public:

    Triangle( Point2 *_p1,  Point2 *_p2,  Point2 *_p3)
    {
        p1 =_p1;
        p2 = _p2;
        p3 = _p3;
        s1 = NULL;
        s2 = NULL;
        s3 = NULL;
    }
    Triangle( Triangle * _t)
    {
        p1 =_t->p1;
        p2 = _t->p2;
        p3 = _t->p3;
        s1 = _t->s1;
        s2 = _t->s2;
        s3 = _t->s3;
    }

    inline bool circumCircleContains(const Point2 &v)
    {
        double ab = (p1->x * p1->x) + (p1->y * p1->y);
        double cd = (p2->x * p2->x) + (p2->y * p2->y);
        double ef = (p3->x * p3->x) + (p3->y * p3->y);

        double circum_x = (ab * (p3->y - p2->y) + cd * (p1->y - p3->y) + ef * (p2->y - p1->y)) / (p1->x * (p3->y - p2->y) + p2->x * (p1->y - p3->y) + p3->x * (p2->y - p1->y)) / 2.f;
        double circum_y = (ab * (p3->x - p2->x) + cd * (p1->x - p3->x) + ef * (p2->x - p1->x)) / (p1->y * (p3->x - p2->x) + p2->y * (p1->x - p3->x) + p3->y * (p2->x - p1->x)) / 2.f;
        double circum_radius = sqrtf(((p1->x - circum_x) * (p1->x - circum_x)) + ((p1->y - circum_y) * (p1->y - circum_y)));

        double dist = sqrtf(((v.x - circum_x) * (v.x - circum_x)) + ((v.y - circum_y) * (v.y - circum_y)));
        return dist <= circum_radius;
    }

    inline Triangle * TriContain2Pt(Point2 * _p1, Point2 * _p2){
        if (s1 && s1->ContainPt(_p1) && s1->ContainPt(_p2))
            return s1;
        else if (s2 && s2->ContainPt(_p1) && s2->ContainPt(_p2))
            return s2;
        else if (s3 && s3->ContainPt(_p1) && s3->ContainPt(_p2))
            return s3;
        else{
            return NULL;
        }
    }
    inline bool ContainPt(Point2 * _p){
        return ((*p1)==(*_p))||((*p2)==(*_p))||((*p3)==(*_p));
    }
    inline Point2* getCircumPt(Point2 *_p1, Point2 *_p2){
        if ((p1 == _p1) ||(p1 == _p2))
        {
            if ((p2 == _p1) ||(p2 == _p2)){
                return p3;
            }
            else{
                return p2;
            }
        }
        else{
            return p1;
        }
    }

    inline void setAroundTriangle(Triangle * _s1 = NULL,Triangle * _s2=NULL,Triangle * _s3=NULL){
            s1 = _s1;
            s2 = _s2;
            s3 = _s3;
    }

    inline void setTri(Triangle * _t){
        p1 = (_t->p1);
        p2 = (_t->p2);
        p3 = (_t->p3);
        s1 = (_t->s1);
        s2 = (_t->s2);
        s3 = (_t->s3);
    }
    inline void setPt(Point2 * _p1, Point2 * _p2, Point2 * _p3){
        p1 = _p1;
        p2 = _p2;
        p3 = _p3;
    }

    inline void setTriUsingTri(Point2 * _p1, Point2 * _p2, Triangle * _t)
    {
        if (s1 && s1->ContainPt(_p1) && s1->ContainPt(_p2))
            s1 = _t;
        else if (s2 && s2->ContainPt(_p1) && s2->ContainPt(_p2))
            s2 = _t;
        else if (s3 && s3->ContainPt(_p1) && s3->ContainPt(_p2))
            s3 = _t;

        return;
    }

    inline bool PointinTriangle( Point2 p)
    {
        Point2 PA = *p1 - p;
        Point2 PB = *p2 - p;
        Point2 PC = *p3 - p;
        double t1 = PA.CrossProduct(PB);
        double t2 = PB.CrossProduct(PC);
        double t3 = PC.CrossProduct(PA);
        return t1*t2 >= 0.00000 && t1*t3 >= 0.0000000;
    }

    Point2 * p1;
    Point2 * p2;
    Point2 * p3;
    Triangle * s1;
    Triangle * s2;
    Triangle * s3;
    vector<int> nextpt;
};

#endif //DT_TRIANGLE_H
