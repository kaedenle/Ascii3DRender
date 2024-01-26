#include<iostream>
#include<vector>
#include<windows.h>
#include<stdlib.h>
#include "./libs/coordinates.h"

using namespace std;

int keyListener(int, bool &);
void reRender(CoordinateSpace &, Shape &, float, float, float);
float addToAngle(float, float);

//the algorithm to sort clockwise in geometry.cpp is broken
//not sorting correctly sometimes (particularly the pyramid)

int main(){
    CoordinateSpace cs(12, 12);
    //cube
    vector<coords3D> coords{{5, 5, 5}, {-5, 5, 5}, {-5, -5, 5}, {5, -5, 5}, {5, 5, -5}, {-5, 5, -5}, {-5, -5, -5}, {5, -5, -5}};
    vector<EdgeDef> edges{{0, 1}, {1, 2}, {2, 3}, {0, 3}, {0, 4}, {1, 5}, {2, 6}, {3, 7}, {4, 5}, {5, 6}, {6, 7}, {4, 7}};
    vector<Face> faces{Face(::FaceType::forward, vector<int>{0, 1, 2, 3}), Face(::FaceType::notforward, vector<int>{4, 3, 7, 11}), Face(::FaceType::notforward, vector<int>{8, 11, 10, 9}), 
                        Face(::FaceType::notforward, vector<int>{5, 9, 6, 1}), Face(::FaceType::notforward, vector<int>{8, 4, 0, 5}), Face(::FaceType::notforward, vector<int>{2, 6, 10, 7})};
    Shape cube(coords, 
            edges,
            faces);
    
    //pyramids
    vector<coords3D> coords2{{-6, -6, -6}, {6, -6, -6}, {-6, -6, 6}, {6, -6, 6}, {0, 6, 0}};
    vector<EdgeDef> edges2{{0, 1}, {2, 3}, {0, 2}, {1, 3}, {0, 4}, {1, 4}, {2, 4}, {3, 4}};
    vector<Face> faces2{Face(::FaceType::notforward, vector<int>{0, 4, 5}), Face(::FaceType::notforward, vector<int>{3, 5, 7}), 
                        Face(::FaceType::forward, vector<int>{1, 6, 7}), Face(::FaceType::notforward, vector<int>{2, 4, 6}), 
                        Face(::FaceType::notforward, vector<int>{0, 1, 2, 3})};
    Shape pyramid(coords2, 
            edges2,
            faces2);

    //hexagonal pyramid
    vector<coords3D> coords3{{{-4, 6, 0}, {4, 6, 0}, {6, 0, 0}, {4, -6, 0}, {-4, -6, 0}, {-6, 0, 0}, {0, 0, 8}}};
    vector<EdgeDef> edges3{{{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {0, 5}, {0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6}}};
    vector<Face> faces3{Face(::FaceType::notforward, vector<int>{1, 2, 3, 4, 0, 5}), Face(::FaceType::forward, {0, 6, 7}), Face(::FaceType::forward, {1, 7, 8}), 
            Face(::FaceType::forward, {2, 8, 9}), Face(::FaceType::forward, {4, 10, 11}), Face(::FaceType::forward, {3, 9, 10}), Face(::FaceType::forward, {5, 11, 6})};
    
    Shape hexagon(coords3, 
            edges3,
            faces3);

    //triangular prism
    vector<coords3D> coords4{{{-4, 6, 4}, {4, 6, 4}, {0, 6, -4}, {-4, -6, 4}, {4, -6, 4}, {0, -6, -4}}};
    vector<EdgeDef> edges4{{{0, 1}, {0, 2}, {1, 2}, {3, 4}, {3, 5}, {4, 5}, {0, 3}, {1, 4}, {2, 5}}};
    vector<Face> faces4{Face(::FaceType::forward, vector<int>{0, 3, 6, 7}), Face(::FaceType::notforward, {1, 4, 6, 8}), Face(::FaceType::notforward, {2, 5, 7, 8}), 
            Face(::FaceType::notforward, {0, 1, 2}), Face(::FaceType::notforward, {3, 4, 5})};
    
    Shape prism(coords4, 
            edges4,
            faces4);

    //Hollow Cube
    /*vector<coords3D> coords5{{{6, 6, 2}, {6, 6, -2}, {-6, 6, 2}, {-6, 6, -2}, {6, -6, 2}, {6, -6, -2}, {-6, -6, 2}, {-6, -6, -2},
                            {9, 9, 2}, {9, 9, -2}, {-9, 9, 2}, {-9, 9, -2}, {9, -9, 2}, {9, -9, -2}, {-9, -9, 2}, {-9, -9, -2}}};
    vector<EdgeDef> edges5{{{0, 2}, {0, 4}, {4, 6}, {2, 6}, {8, 10}, {8, 12}, {12, 14}, {10, 14}, {0, 8}, {4, 12}, {6, 14}, {2, 10}, 
                            {1, 3}, {1, 5}, {5, 7}, {3, 7}, {9, 11}, {9, 13}, {13, 15}, {11, 15}, {1, 9}, {5, 13}, {7, 15}, {3, 11},
                            {10, 11}, {14, 15}, {2, 3}, {6, 7}, {8, 9}, {12, 13}, {0, 1}, {4, 5}}};
    vector<Face> faces5{
            Face(::FaceType::forward, vector<int>{0, 4, 8, 11}), Face(::FaceType::forward, {2, 9, 6, 10}), 
            Face(::FaceType::forward, {3, 7, 10, 11}), Face(::FaceType::forward, {1, 5, 8, 9}), 
            Face(::FaceType::notforward, {12, 16, 20, 23}), Face(::FaceType::notforward, {13, 17, 20, 21}),
            Face(::FaceType::notforward, {14, 18, 21, 22}), Face(::FaceType::notforward, {15, 19, 22, 23}),
            Face(::FaceType::notforward, {0, 12, 26, 30}), Face(::FaceType::notforward, {4, 6, 24, 28}),
            Face(::FaceType::notforward, {3, 15, 26, 27}), Face(::FaceType::notforward, {7, 19, 24, 25}),
            Face(::FaceType::notforward, {1, 3, 30, 31}), Face(::FaceType::notforward, {5, 17, 28, 29}),
            Face(::FaceType::notforward, {2, 14, 27, 31}), Face(::FaceType::notforward, {6, 18, 25, 29})
            };
    
    Shape hollow(coords5, 
            edges5,
            faces5);*/

    vector<Shape *> shape_list{&cube, &pyramid, &hexagon, &prism};
    int shape_index = 0;
    Shape *current = shape_list[shape_index];

    bool pressing[8]{ 0 };
    bool hasPressed = true;
    bool shapeRelease = false;
    bool flag = true;

    float x_angle = 0, y_angle = 0, z_angle = 0;
    const float ADD = 5;

    //cs.renderer.buffer.outputBuffer();
    //key listener
    while(flag == true){
        if(keyListener('D', pressing[0]) == 0){
            y_angle = addToAngle(y_angle, ADD);
            hasPressed = true;
        }
        if(keyListener('W', pressing[1]) == 0){
            x_angle = addToAngle(x_angle, ADD);
            hasPressed = true;
        }
        if(keyListener('A', pressing[2]) == 0){
            y_angle = addToAngle(y_angle, -ADD);
            hasPressed = true;
        }
        if(keyListener('Z', pressing[3]) == 0){
            z_angle = addToAngle(z_angle, ADD);
            hasPressed = true;
        }
        if(keyListener('X', pressing[4]) == 0){
            z_angle = addToAngle(z_angle, -ADD);
            hasPressed = true;
        }
        if(keyListener('S', pressing[5]) == 0){
            x_angle = addToAngle(x_angle, -ADD);
            hasPressed = true;
        }

        if(keyListener('C', pressing[7]) == 1){
            shape_index = (shape_index + 1) % shape_list.size();
            current = shape_list[shape_index];
            reRender(cs, *current, x_angle, y_angle, z_angle);
            pressing[7] = false;
        }

        if(keyListener(VK_ESCAPE, pressing[6]) == 0){
            flag = false;
            hasPressed = true;
        }

        if(hasPressed){
            hasPressed = false;
            fill(begin(pressing), end(pressing), false);
            reRender(cs, *current, x_angle, y_angle, z_angle);
            Sleep(20);
        }
    }
}

float addToAngle(float angle, float ADD){
    angle += ADD;
    if(angle > 360) angle -= 360;
    if(angle < 0) angle += 360;

    return angle;
}

void reRender(CoordinateSpace &cs, Shape &s, float x_angle, float y_angle, float z_angle){
    cs.clearGrid();
    cout << "\033[2J\033[1;1H";
    cout << endl;
    cs.renderer.renderBorder();
    
    s.rotateShape(x_angle, y_angle, z_angle);
    cs.renderer.renderShape(s);

    //cout << endl;
    cs.drawGrid();
    //cs.renderer.buffer.outputBuffer();
}

int keyListener(int key, bool &isPressed){
    if (GetAsyncKeyState(key) < 0 && isPressed == false)
    {
        isPressed = true;
        return 0;
    }
    if (GetAsyncKeyState(key) == 0 && isPressed == true)
    {
        isPressed = false;
        return 1;
    }
    return 2;
}