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
    /*vector<coords3D> coords{{5, 5, 5}, {-5, 5, 5}, {-5, -5, 5}, {5, -5, 5}, {5, 5, -5}, {-5, 5, -5}, {-5, -5, -5}, {5, -5, -5}};
    vector<EdgeDef> edges{{0, 1}, {1, 2}, {2, 3}, {0, 3}, {0, 4}, {1, 5}, {2, 6}, {3, 7}, {4, 5}, {5, 6}, {6, 7}, {4, 7}};
    vector<Face> faces{Face(::FaceType::forward, vector<int>{0, 1, 2, 3}), Face(::FaceType::right, vector<int>{4, 3, 7, 11}), Face(::FaceType::back, vector<int>{8, 11, 10, 9}), 
                        Face(::FaceType::left, vector<int>{5, 9, 6, 1}), Face(::FaceType::top, vector<int>{8, 4, 0, 5}), Face(::FaceType::bottom, vector<int>{2, 6, 10, 7})};
    Shape cube(coords, 
            edges,
            faces);*/
    
    //pyramids
    /*vector<coords3D> coords2{{-6, -6, -6}, {6, -6, -6}, {-6, -6, 6}, {6, -6, 6}, {0, 8, 0}};
    vector<EdgeDef> edges2{{0, 1}, {2, 3}, {0, 2}, {1, 3}, {0, 4}, {1, 4}, {2, 4}, {3, 4}};
    vector<Face> faces2{Face(::FaceType::back, vector<int>{0, 4, 5}), Face(::FaceType::right, vector<int>{3, 5, 7}), 
                        Face(::FaceType::forward, vector<int>{1, 6, 7}), Face(::FaceType::left, vector<int>{2, 4, 6}), 
                        Face(::FaceType::bottom, vector<int>{0, 1, 2, 3})};
    Shape pyramid(coords2, 
            edges2,
            faces2);*/

    //hexagonal pyramid
    vector<coords3D> coords3{{{-4, 6, 0}, {4, 6, 0}, {6, 0, 0}, {4, -6, 0}, {-4, -6, 0}, {-6, 0, 0}, {0, 0, 8}}};
    vector<EdgeDef> edges3{{{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {0, 5}, {0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6}}};
    vector<Face> faces3{Face(::FaceType::back, vector<int>{1, 2, 3, 4, 0, 5}), Face(::FaceType::forward, {0, 6, 7}), Face(::FaceType::forward, {1, 7, 8}), 
            Face(::FaceType::forward, {2, 8, 9}), Face(::FaceType::forward, {4, 10, 11}), Face(::FaceType::forward, {3, 9, 10}), Face(::FaceType::forward, {5, 11, 6})};
    
    Shape hexagon(coords3, 
            edges3,
            faces3);

    bool pressing[7]{ 0 };
    bool hasPressed = TRUST_E_NO_SIGNER_CERT;
    bool flag = true;
    float x_angle = 0, y_angle = 0, z_angle = 0;
    const float ADD = 5;

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
        if(keyListener(VK_ESCAPE, pressing[6]) == 0){
            flag = false;
            hasPressed = true;
        }

        if(hasPressed){
            hasPressed = false;
            fill(begin(pressing), end(pressing), false);
            reRender(cs, hexagon, x_angle, y_angle, z_angle);
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