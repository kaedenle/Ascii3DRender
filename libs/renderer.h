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
        ZBuffer buffer;
        Renderer(vector< vector<char> >& grid, int, int);

        void renderLine(Edge &, coords3DFloat, float , char );
        void renderLine(Point3D &, Point3D &, coords3DFloat, float, char);
        void Bresenham3D(Point3D &, Point3D &, char);

        void renderPoint(Point &, char, int = numeric_limits<int>::max());
        //to check buffer
        void renderPoint(Point3D &, char, int = 0);

        void renderBorder();
        void renderShape(Shape &);

    private:
        void renderFace(Face &);
        void renderTriangle(TriangularPiece &);
        float getZValue(float, float, coords3DFloat, float, float = numeric_limits<int>::min());

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