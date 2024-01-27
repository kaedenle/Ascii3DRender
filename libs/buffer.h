 #ifndef BUFFER_H
 #define BUFFER_H

 #include<vector>
 #include<limits>
 #include "mathdata.h"
 #include "geometry.h"
 #include "geomath.h"

 using namespace std;

class TriangularPiece;

class ZBuffer{
    int XMAX, YMAX;
    public:
        vector< vector<float> > grid;
        ZBuffer(int, int);
        void addToBuffer(int, int, float);
        void resolveBuffer(TriangularPiece &tri);
        void outputBuffer();
        void clearBuffer();
};

 #endif