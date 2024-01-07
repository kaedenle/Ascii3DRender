#include<iostream>
#include "coordinates.h"

using namespace std;

#define SPACECHAR ' '

CoordinateSpace::CoordinateSpace(int x, int y) : XMAX(x), YMAX(y), grid((y * 2) + 3 , vector<char>((x * 2) + 3, SPACECHAR)), renderer(grid, x, y){
    renderer.renderBorder();
}

void CoordinateSpace::drawGrid(){
    string out = "";
    //output the entire grid
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            out.push_back(grid[i][j]);
            out.push_back(' ');
        }
        if(i != grid.size() - 1) out.push_back('\n');
    }
    cout << out;
}

void CoordinateSpace::clearGrid(){
    //output the entire grid
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            grid[i][j] = ' ';
        }
    }
}