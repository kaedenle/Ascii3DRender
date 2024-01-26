#ifndef GEOMATH_H
#define GEOMATH_H

#include <vector>
#include "mathdata.h"

using namespace std;

vector< vector<float> > mulMat(vector< vector<float> > mat1, vector< vector<float> > mat2);

int average (vector<int> &v);

//for 3 verticies that are connected
coords3D crossProduct(coords3D vector1, coords3D vector2);

//for 3 verticies that are connected
coords3DFloat crossProduct(coords3DFloat vector1, coords3DFloat vector2);

coords3D calcCenter(vector<Point3D> &shape_points);

//get dot product of vectors
template <typename coords>
float dotProduct(coords vector1, coords vector2){
    float x = vector1.x * vector2.x;
    float y = vector1.y * vector2.y;
    float z = vector1.z * vector2.z;
    return x + y + z;
}

//get magnitude of a vector
template <typename coords>
float magnitude(coords vec){
    return sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
}

//normalize a vector
template <typename coords>
coords normalize(coords vector){
    float mag = 1/magnitude<coords>(vector);
    vector.x *= mag;
    vector.y *= mag;
    vector.z *= mag;
    return vector;
}

//check if point is within the triangle on the x-y plane
float sign (coords p1, coords p2, coords p3);

bool PointInTriangle (coords pt, coords v1, coords v2, coords v3);

#endif