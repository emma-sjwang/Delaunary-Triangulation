//
// Created by sjwang on 6/5/2018.
//

#ifndef DT_DELAUNAY_H
#define DT_DELAUNAY_H

#include "point.h"
#include "edge.h"
#include "triangle.h"

#include <vector>
#include <algorithm>


using namespace std;
class Edge;

class Delaunay
{
public:
    const void triangulate(std::vector<Point2> vertices, vector<Triangle*> & currentTriangle)
    {
        // initialization
        // generate large range
        float minX =  vertices[0].x;
        float minY = vertices[0].y;
        float maxX = minX;
        float maxY = minY;

        for(std::size_t i = 0; i < vertices.size(); ++i)
        {
            if (vertices[i].x < minX) minX = vertices[i].x;
            if (vertices[i].y < minY) minY = vertices[i].y;
            if (vertices[i].x > maxX) maxX = vertices[i].x;
            if (vertices[i].y > maxY) maxY = vertices[i].y;
        }

        float dx = maxX - minX;
        float dy = maxY - minY;
        float deltaMax = std::max(dx, dy);
        float midx = (minX + maxX) / 2.f;
        float midy = (minY + maxY) / 2.f;

        // set super triangle to contain everything
        pp1.set(midx - 20 * deltaMax, midy - 20 * deltaMax);
        pp2.set(midx, midy + 20 * deltaMax);
        pp3.set(midx + 20 * deltaMax, midy - 20 * deltaMax);

        // Create super triangle and generate point list for this triangle.
        Triangle t(&pp1, &pp2, &pp3);

        // current useful triangle, this is used for output
        _triangles.push_back(&t);

        for(std::vector<int>::size_type i = 0; i != vertices.size(); i++) {
            t.nextpt.push_back(i);
            currentTriangle.push_back(&t);
        }

        // insert each point into delaunary triangle
        for(std::vector<int>::size_type i = 0; i != vertices.size(); i++)
        {
            // currentTriangle[i] contains vertices[i], directly get it, insert the new point generate new triangle, destroy old one.
            Point2 * p1 = currentTriangle[i]->p1;
            Point2 * p2 = currentTriangle[i]->p2;
            Point2 * p3 = currentTriangle[i]->p3;

            // remove current point
            vector<int> pretri = currentTriangle[i]->nextpt;
            std::vector<int>::iterator position = std::find(pretri.begin(), pretri.end(), i);
            if (position != pretri.end()) // == myVector.end() means the element was not found
                pretri.erase(position);

            // create three new triangles
            Triangle* ts1 = currentTriangle[i];
            ts1->setPt(p1, p2, &vertices[i]);
            ts1->nextpt.clear();
            Triangle* ts2 = new Triangle(p2, p3, &vertices[i]);
            Triangle* ts3 = new Triangle(p3, p1, &vertices[i]);
            // update their around triangles
            Triangle * ttt1 = ts1->TriContain2Pt(ts1->p1, ts1->p2);
            Triangle * ttt2 = ts1->TriContain2Pt(ts2->p1, ts2->p2);
            Triangle * ttt3 = ts1->TriContain2Pt(ts3->p1, ts3->p2);
            ts2->setAroundTriangle(ttt2, ts3, ts1);
            ts3->setAroundTriangle(ttt3, ts1, ts2);
            ts1->setAroundTriangle(ttt1, ts2, ts3);
            if(ttt1)
                ttt1->setTriUsingTri(ts1->p1, ts1->p2, ts1);
            if(ttt2)
                ttt2->setTriUsingTri(ts1->p1, ts1->p2, ts2);
            if(ttt3 )
                ttt3->setTriUsingTri(ts1->p1, ts1->p2, ts3);

            updatePtList(vertices, currentTriangle, ts1, ts2, ts3, pretri);

            _triangles.push_back(ts2);
            _triangles.push_back(ts3);

            // check legality for each edge of initial triangle.
            checklegality(vertices, currentTriangle, ts2, _triangles);
            checklegality(vertices, currentTriangle, ts3, _triangles);
            checklegality(vertices, currentTriangle, ts1, _triangles);
        }

        generateEdges();
//        cout << "length of _triangles" << _triangles.size() << endl;
    }
    inline const std::vector<Triangle*>& getTriangles() const { return _triangles; };
    inline const std::vector<Edge*>& getEdges() const { return _edges; };
    inline void generateEdges();
    inline void updatePtList(vector<Point2> &vertices, vector<Triangle*> & currentTriangle, Triangle * ts1, Triangle * ts2, Triangle * ts3, vector<int> &tri);
    inline void checklegality(vector<Point2> &vertices, vector<Triangle*> & currentTriangle, Triangle * subtri, vector<Triangle*> &_triangles);
//    void generateEdges();
public:
    Point2  pp1, pp2, pp3;      // contain infinity points
    std::vector<Triangle*> _triangles;  // all triangles need to be print
    std::vector<Edge*> _edges;          // all triangles need to be print
};

inline void Delaunay::generateEdges() {
    // only print the edges that not link with infinite points
    _edges.clear();
    for(auto p = begin(_triangles); p != end(_triangles); p++) {
        Edge *edge = new Edge(*((*p)->p1), *((*p)->p2));
        if ((edge->iscontain(pp1)) || (edge->iscontain(pp2))||(edge->iscontain(pp3))){
            ;
        }
        else{
            _edges.push_back(edge);
        }
        Edge *edge2 = new Edge(*((*p)->p2), *((*p)->p3));
        if ((edge2->iscontain(pp1)) || (edge2->iscontain(pp2))||(edge2->iscontain(pp3))){
            ;
        }
        else{
            _edges.push_back(edge2);
        }
        Edge *edge3 = new Edge(*((*p)->p3), *((*p)->p1));
        if ((edge3->iscontain(pp1)) || (edge3->iscontain(pp2))||(edge3->iscontain(pp3))){
            ;
        }
        else{
            _edges.push_back(edge3);
        }
    }
}

inline void Delaunay::updatePtList(vector<Point2> &vertices, vector<Triangle*> & currentTriangle, Triangle * ts1, Triangle * ts2, Triangle * ts3, vector<int> &tri){
// redistribute the point vector into new triangles
    for(auto index = begin(tri); index != end(tri); index++){
        if (ts1->PointinTriangle(vertices[*index])) {
            ts1->nextpt.push_back(*index);
            currentTriangle[*index] = ts1;
        }
        else if (ts2->PointinTriangle(vertices[*index])) {
            ts2->nextpt.push_back(*index);
            currentTriangle[*index] = ts2;
        }
        else  if (ts3 && (ts3->PointinTriangle(vertices[*index]))) {
            ts3->nextpt.push_back(*index);
            currentTriangle[*index] = ts3;
        }
//        else
//        {
//            cout << "ERROR: update!" << endl;
//            assert(false);
//        }
    }
}


inline void Delaunay::checklegality(vector<Point2> &vertices, vector<Triangle*> & currentTriangle, Triangle * subtri, vector<Triangle*> &_triangles){
    // check each new triangle is legal.
    bool illegal = false;
    Point2 *circumPts = NULL;
    Triangle * adjcentSubTri = subtri->TriContain2Pt(subtri->p1, subtri->p2);
    if (adjcentSubTri) {
        circumPts = (subtri->TriContain2Pt(subtri->p1, subtri->p2)->getCircumPt(subtri->p1, subtri->p2));    // cpt
        illegal = subtri->circumCircleContains(*circumPts);
        if (!illegal)
            return;

        // create a new point vector containing all points need to be redistribution.
        vector<int> A = subtri->nextpt;
        vector<int> B = adjcentSubTri->nextpt;
        vector<int> AB;
        AB.reserve( A.size() + B.size() ); // preallocate memory
        AB.insert( AB.end(), A.begin(), A.end() );
        AB.insert( AB.end(), B.begin(), B.end() );

        // set some temperate variable to contain surrounding triangles
        // because we just change the triangle and not generate new one, so there would be some tedious operation to change them
        Triangle * ttt1T2 = NULL;
        Triangle * ttt2T2 = NULL;
        Triangle * ttt3T2 = NULL;
        ttt1T2 = subtri->TriContain2Pt(subtri->p2, subtri->p3);
        Triangle * tempT1 = new Triangle(*adjcentSubTri);

        // create two new triangles, after flipping
        Triangle * T1 = subtri;
        T1->nextpt.clear();
        Triangle * T2 = adjcentSubTri;
        T2->nextpt.clear();

        Triangle * ttt1T1 = tempT1->TriContain2Pt(subtri->p1, circumPts);
        Triangle * ttt2T1 = T2;
        Triangle * ttt3T1 = subtri->TriContain2Pt(subtri->p1, subtri->p3);
        T1->setAroundTriangle(ttt1T1, ttt2T1, ttt3T1);

        // update their surrounding triangle
        ttt2T2 = T2->TriContain2Pt(circumPts, subtri->p2);
        ttt3T2 = T1;
        T2->setAroundTriangle(ttt1T2, ttt2T2, ttt3T2);
        // update surrounding triangle for surrounding triangle
        if (T1->TriContain2Pt(subtri->p1, circumPts)){
            T1->TriContain2Pt(subtri->p1, circumPts)->setTriUsingTri(subtri->p1, circumPts, T1);
        }
        if (T1->TriContain2Pt(subtri->p1, subtri->p3)){
            T1->TriContain2Pt(subtri->p1, subtri->p3)->setTriUsingTri(subtri->p1, subtri->p3, T1);
        }
        if (T2->TriContain2Pt(circumPts, subtri->p2))
        {
            T2->TriContain2Pt(circumPts, subtri->p2)->setTriUsingTri(circumPts, subtri->p2, T2);
        }
        if (T2->TriContain2Pt(subtri->p2, subtri->p3)){
            T2->TriContain2Pt(subtri->p2, subtri->p3)->setTriUsingTri(subtri->p2, subtri->p3, T2);
        }
        T2->setPt(circumPts, subtri->p2, subtri->p3);
        T1->setPt(subtri->p1, circumPts, subtri->p3);

        // update point list in triangle
        updatePtList(vertices, currentTriangle, T1, T2, NULL, AB);

        // check legality of new two triangles
        checklegality(vertices, currentTriangle, T1, _triangles);
        checklegality(vertices, currentTriangle, T2, _triangles);

    } else
    {
        return;
    }
}
#endif //DT_DELAUNAY_H
