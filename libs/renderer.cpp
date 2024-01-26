#include<iostream>
#include<limits>
#include "renderer.h"

using namespace std;

#define POINTCHAR 'o'
#define BORDERCHAR '+'
#define LINECHAR 'i'
#define AXISCHAR '#'
#define CENTERCHAR '0'
#define SEMICENTERCHAR '.'

Renderer::Renderer(vector< vector<char> >& g, int x, int y) : grid(g), XMAX(x), YMAX(y), buffer(x, y){}

void Renderer::renderPoint(Point &p, char c, float z){
    p.convertToIndex(XMAX, YMAX);
    //make check to see if in range
    int new_z = round(z);
    if(new_z != numeric_limits<int>::max()) buffer.addToBuffer(p.index.y, p.index.x, new_z);
    if(inbounds(p) && grid[p.index.y][p.index.x] == ' ' && buffer.grid[p.index.y][p.index.x] <= new_z) grid[p.index.y][p.index.x] = c;
}

void Renderer::renderPoint(Point3D &p, char c){
    coords index = p.convertToIndex(XMAX, YMAX);
    int y_index = -p.render_coords3D.y + YMAX + 1;
    int x_index = p.render_coords3D.x + XMAX + 1;
    //coords cord = {p.render_coords3D.x + XMAX + 1, -p.render_coords3D.y + YMAX + 1};
    //make check to see if in range
    if(inbounds(p) && grid[y_index][x_index] == ' ' && buffer.grid[y_index][x_index] <= p.render_coords3D.z) grid[y_index][x_index] = c;
}

//bresenham line algorithm
//adjusted to calculate buffer
void Renderer::renderLine(Point3D &p1, Point3D &p2, coords3DFloat normal, float d, char c){
    float z;
    int new_z;
    //initialize data
    int world_x = p1.render_coords3D.x, world_y = p1.render_coords3D.y;
    int min_z = min(p2.render_coords3D.z, p1.render_coords3D.z), max_z = max(p2.render_coords3D.z, p1.render_coords3D.z);
    int dz = max_z - min_z;

    int dx = abs(p2.render_coords3D.x - p1.render_coords3D.x), dy = abs(p2.render_coords3D.y - p1.render_coords3D.y);
    int sign1 = sign(p2.render_coords3D.x - p1.render_coords3D.x), sign2 = sign(p2.render_coords3D.y - p1.render_coords3D.y);

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
    z = getZValue(world_x, world_y, normal, d, min_z);
    //char p = new_z + '0';

    renderPoint(start, c, z);

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
        //plot next point
        Point p(world_x, world_y);
        z = getZValue(world_x, world_y, normal, d, min_z);
        //char t = new_z + '0';

        renderPoint(p, c, z);
    }
}

//if you want to just pass in an edge
void Renderer::renderLine(Edge &e, coords3DFloat normal, float d, char c){
    renderLine(e.p1, e.p2, normal, d, c);
}

float Renderer::getZValue(float x, float y, coords3DFloat normal, float d, float def){
    //if plane perpendicular to FOV
    if(normal.z == 0){
        return def;
    }

    return -(d + normal.x * x + normal.y * y)/normal.z;
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
    //create buffer first then render
    for(Face &face : shape.face_list){
        for(TriangularPiece &piece : face.triangle_list){
            piece.makeEquation();
            buffer.resolveBuffer(piece);
        }
    }

    //render everything
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
    //if(piece.backfaceCull(*piece.points_ref[1], *piece.points_ref[2], *piece.points_ref[0])) return;

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
    //change this logic so we render the lines w/ coords in mind (will have to calculate equation from one ray to another)
    for(Edge* edge : piece.edges_ref){
        //line itself is visible and both points of edge are visible
        if(edge->visible){
            /*coords c1 = {edge->p1.render_coords3D.x + XMAX + 1, -edge->p1.render_coords3D.y + YMAX + 1};
            coords c2 = {edge->p2.render_coords3D.x + XMAX + 1, -edge->p2.render_coords3D.y + YMAX + 1};
            if(buffer.grid[c1.y][c1.x] <= edge->p1.render_coords3D.z && buffer.grid[c2.y][c2.x] <= edge->p2.render_coords3D.z)*/
            renderLine(*edge, piece.normal, piece.d, LINECHAR);
        }    
    }
}