#include<iostream>
#include<limits>
#include<cmath>
#include "mathdata.h"

Point::Point(){}
Point::Point(int x, int y) : world{x, y} {}

coords Point::convertToIndex(int mX, int mY){
    index = {world.x + mX + 1, -world.y + mY + 1};
    return index;
}

bool Point::isWorldEqual(Point p){
    return p.world.x == world.x && p.world.y == world.y;
}

Point3D::Point3D(){}
Point3D::Point3D(coords3D coord, int ID, bool render) : coord3D(coord), render_coords3D(coord) {
    id = ID;
    visible = render;
    project3Dcoords();
}

//I only want the copy here
void Point3D::setcoords3D(coords3D coord){
    render_coords3D = coord;
    project3Dcoords();
}

void Point3D::project3Dcoords(){
    int TrueZ = CONST_Z * 2;
    //get division constant
    float value = 1 / (float)(-render_coords3D.z + TrueZ);
    //project 3d coords to x and y
    int proj_x = (int)(render_coords3D.x * TrueZ * value), proj_y = (int)(render_coords3D.y * TrueZ * value);
    //get 2d world coordinates
    world = {proj_x, proj_y};
}

bool Point3D::is3DEqual(Point3D p){
    return p.coord3D.x == coord3D.x && p.coord3D.y == coord3D.y && p.coord3D.z == coord3D.z;
}

void Point3D::print3DCoords(){
    cout << render_coords3D.x << " " << render_coords3D.y << " " << render_coords3D.z << endl;
}

Edge::Edge(Point3D &p1, Point3D &p2, bool visible) : p1(p1), p2(p2), visible(visible){}