//
// Created by sjwang on 6/5/2018.
//
#include <iostream>
#include <chrono>
#include <vector>
#include <iterator>
#include <algorithm>
#include <array>

#include <SFML/Graphics.hpp>
#include <fstream>

#include "point.h"
#include "triangle.h"
#include "delaunay.h"
#include "edge.h"

using namespace std;



inline float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom)
{
    const sf::Vector2f beforeCoord{ window.mapPixelToCoords(pixel) };
    sf::View view{ window.getView() };
    view.zoom(zoom);
    window.setView(view);
    const sf::Vector2f afterCoord{ window.mapPixelToCoords(pixel) };
    const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
    view.move(offsetCoords);
    window.setView(view);
}


int main()
{

    // change these two variables
    int numberPoints = 50;
    bool GENERATE_POINTS_IN_CODE = true;  // true or false


    vector<Point2> points;
    const int BOUNDINGBOXWEIGHT = 2000;
    const int BOUNDINGBOXHEIGHT = 2000;


    if (GENERATE_POINTS_IN_CODE) {   // random generate points
        srand(time(NULL));
        cout << "Generating " << numberPoints << " random points" << endl;
        for (int i = 0; i < numberPoints; i++) {
            points.push_back(Point2(RandomFloat(50, BOUNDINGBOXWEIGHT-50), RandomFloat(50, BOUNDINGBOXHEIGHT-50)));
        }
    }
    else{ //points read from file
        ifstream inFile;
        inFile.open("..//inputs//points_copy.txt");
        if (!inFile){
            cout << "[ERROR]: Failed to open " << endl;
            exit(-1);
        }
        inFile >> numberPoints;

        for (int i = 0; i < numberPoints; i++) {
            float x, y;
            inFile >> x >> y;
            points.push_back(Point2(x, y));
        }
        inFile.close();
        cout << "Read " << numberPoints << " points" << endl;
        // random insert points
//        random_shuffle ( points.begin(), points.end() );
        cout << "Random Shuffle " << numberPoints << " points" << endl;
    }

    // print time in ms
    Delaunay triangulation;
    vector<Triangle*> currentTriangle;
    auto started = std::chrono::high_resolution_clock::now();

    triangulation.triangulate(points, currentTriangle);

    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << " ms" << endl;

    vector<Edge*> edges = triangulation.getEdges();

    ofstream outFile;
    outFile.open("..//outputs//segments.txt");
    if (!outFile){
        cout << "[ERROR]: Failed to open " << endl;
        exit(-1);
    }
    for(auto p = begin(edges); p != end(edges); p++) {
        outFile << (*p)->p1.x << " " << (*p)->p2.x << " " << (*p)->p1.y << " " << (*p)->p2.y << endl;
    }
    outFile.close();


    // SFML window
    sf::RenderWindow window(sf::VideoMode(BOUNDINGBOXWEIGHT, BOUNDINGBOXHEIGHT), "Delaunay triangulation");

    // Make the lines
    vector<array<sf::Vertex, 2> > lines;
    for(auto e = begin(edges); e != end(edges); e++) {
        lines.push_back({{
                                 sf::Vertex(sf::Vector2f((*e)->p1.x + 1, (*e)->p1.y + 1)),
                                 sf::Vertex(sf::Vector2f((*e)->p2.x + 1, (*e)->p2.y + 1))
                         }});
    }
    const float zoomAmount{ 1.1f };
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
//            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed || event.key.code == sf::Keyboard::Escape)

            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseWheelScrolled)
            {
                if (event.mouseWheelScroll.delta > 0)
                    zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, (1.f / zoomAmount));
                else if (event.mouseWheelScroll.delta < 0)
                    zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, zoomAmount);
            }
        }

        window.clear();

        // Draw the lines
        for(auto l = begin(lines); l != end(lines); l++) {
            window.draw((*l).data(), 2, sf::Lines);
        }

        window.display();
    }

    return 0;
}
