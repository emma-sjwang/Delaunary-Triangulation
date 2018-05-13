//
// Created by sjwang on 6/5/2018.
//

#ifndef DT_POINT_H
#define DT_POINT_H

#include <cmath>

class Point2
{
public:
    //
    // Constructors
    //
    Point2()
    {
        x = 0;
        y = 0;
        inserted = false;
    }

    Point2(float _x, float _y)
    {
        x = _x;
        y = _y;
        inserted = false;
    }

    Point2(const Point2 &v)
    {
        x = v.x;
        y = v.y;
        inserted = v.inserted;
    }

    inline void set(const Point2 &v)
    {
        x = v.x;
        y = v.y;
        inserted = v.inserted;
    }
    inline void set(float _x, float _y)
    {
        x = _x;
        y = _y;
        inserted = false;
    }
    //
    // Operations
    //

    // Subtract
    inline Point2 operator - (const Point2& v) const
    {
        return Point2(x - v.x, y - v.y) ;
    }

    // Dot product
    inline double Dot(const Point2& v) const
    {
        return x * v.x + y * v.y ;
    }

    inline double CrossProduct(const Point2 vec)
    {
        return x * vec.y - y * vec.x;
    }

    double x;
    double y;
    bool inserted;
};

inline bool operator == (Point2 v1, Point2 v2)
{
    return (v1.x == v2.x) && (v1.y == v2.y);
}

std::ostream &operator << (std::ostream &str, Point2 const &point)
{
    return str << point.x << " " << point.y;
}

#endif //DT_POINT_H
