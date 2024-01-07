 #ifndef COORDS_H
 #define COORDS_H

#include<vector>
#include "mathdata.h"
#include "renderer.h"
#include "geometry.h"

using namespace std;

class CoordinateSpace{
    int XMAX, YMAX;
    vector< vector<char> > grid;

    public:
        Renderer renderer;
        CoordinateSpace(int, int);
        //draw the grid onto the terminal
        void drawGrid();
        void clearGrid();
};

#endif