#include<iostream>
#include "renderer.h"

using namespace std;

#define POINTCHAR 'o'
#define BORDERCHAR '+'
#define LINECHAR 'i'
#define AXISCHAR '#'
#define CENTERCHAR '0'
#define SEMICENTERCHAR '.'

Renderer::Renderer(vector< vector<char> >& g, int x, int y) : grid(g), XMAX(x), YMAX(y){}

void Renderer::renderPoint(Point &p, char c){
    p.convertToIndex(XMAX, YMAX);
    //make check to see if in range
    if(inbounds(p) && grid[p.index.y][p.index.x] == ' ') grid[p.index.y][p.index.x] = c;
}

void Renderer::renderPoint(Point3D &p, char c){
    p.convertToIndex(XMAX, YMAX);
    //make check to see if in range
    if(inbounds(p) && grid[p.index.y][p.index.x] != POINTCHAR) grid[p.index.y][p.index.x] = c;
}

//bresenham line algorithm
void Renderer::renderLine(Point &p1, Point &p2, char c){
    //initialize data
    int world_x = p1.world.x, world_y = p1.world.y;
    int dx = abs(p2.world.x - p1.world.x), dy = abs(p2.world.y - p1.world.y);
    int sign1 = sign(p2.world.x - p1.world.x), sign2 = sign(p2.world.y - p1.world.y);

    //check if line is in vertical-skewed octant
    //if it is, interchange values
    bool interchange = false;
    if(dy > dx){
        swap(dx, dy);
        interchange = true;
    }

    //define values
    int error = 2 * dy - dx;
    int a = 2 * dy, b = 2 * dy - 2 * dx;

    //plot starting point
    Point start(world_x, world_y);
    renderPoint(start, c);

    //iteratively plot all points, moving a pixel based on the algorithm
    for(int i = 1; i < dx; i++){
        if(error < 0){
            if(interchange)
                world_y += sign2;
            else
                world_x += sign1;
            error += a;
        }
        else{
            world_y += sign2;
            world_x += sign1;
            error += b;
        }
        //plot starting point
        Point p(world_x, world_y);
        renderPoint(p, c);
    }
}

//if you want to just pass in an edge
void Renderer::renderLine(Edge &e, char c){
    renderLine(e.p1, e.p2, c);
}

//render borders around grid
void Renderer::renderBorder(){
    //fill the horizontal borders
    int lastIndex = grid.size() - 1;

    fill(grid[0].begin(), grid[0].end(), BORDERCHAR);
    fill(grid[lastIndex].begin(), grid[lastIndex].end(), BORDERCHAR);

    //fill the vertical of the grid
    for (int i = 0; i < grid.size(); i++)
    {
        grid[i][0] = BORDERCHAR;
        grid[i][grid[i].size() - 1] = BORDERCHAR;
    }
}

//render a shape
//works by rendering face by face
//then by piece by piece per face
void Renderer::renderShape(Shape &shape){
    for(Face &face : shape.face_list){
        renderFace(face);
    }
}

//render a face (helper)
void Renderer::renderFace(Face &face){
    string s = "";
    switch (face.faceRot)
    {
        case 0:
            s = "front";
            break;
        case 1:
            s = "back";
            break;
        case 2:
            s = "right";
            break;
        case 3:
            s = "left";
            break;
        case 4:
            s = "top";
            break;
        case 5:
            s = "bottom";
            break;
        default:
            break;
    }
    //cout << s << endl;
    for(TriangularPiece &triangle : face.triangle_list){
        renderTriangle(triangle);
    }
    //cout << endl;
}

//render a triangle
void Renderer::renderTriangle(TriangularPiece &piece){
    //backface culling to see if this should even be rendered (should only be three pieces)
    if(piece.backfaceCull(*piece.points_ref[1], *piece.points_ref[2], *piece.points_ref[0])) return;
    //render points
    for(Point3D* point : piece.points_ref){
        char rendering = char(point->id + '0');
        switch(point->id){
            case -1:
                rendering = CENTERCHAR;
                break;
            case -2:
                rendering = SEMICENTERCHAR;
                break;
        }

        //cout << point->id << " ";

        //if(point->render_coords3D.z == -5) rendering = '8';

        if(point->visible)
            renderPoint(*point, rendering);
    }

    //cout << endl;

    //render edges
    for(Edge* edge : piece.edges_ref){
        if(edge->visible)
            renderLine(*edge, LINECHAR);
    }
}