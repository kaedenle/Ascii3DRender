 #ifndef RENDERER_H
 #define RENDERER_H

#include<vector>
#include "mathdata.h"
#include "geometry.h"

using namespace std;

class Renderer{
    vector< vector<char> >& grid;
    int XMAX, YMAX;
    public:
        Renderer(vector< vector<char> >& grid, int, int);

        void renderLine(Edge &, char);
        void renderLine(Point &, Point &, char);

        void renderPoint(Point &, char);
        //to check buffer
        void renderPoint(Point3D &, char);

        void renderBorder();
        void renderShape(Shape &);

    private:
        void renderFace(Face &);
        void renderTriangle(TriangularPiece &);

        int sign(int n){
            if(n == 0)
                return 0;
            return n > 0 ? 1 : -1;
        }

        bool inbounds(Point p){
            return p.world.x <= XMAX && p.world.x >= -XMAX && p.world.y <= YMAX && p.world.y >= -YMAX;
        }
};

#endif