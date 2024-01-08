 #ifndef MATHDATA_H
 #define MATHDATA_H

 #include<vector>
 #include<cmath>

using namespace std;

//camera z coord
#define CONST_Z 16

class Edge;
class Point3D;

struct coords
{
     int x;
     int y;
};

struct coords3D
{
     int x;
     int y;
     int z;
};

//to deal with normalization
struct coords3DFloat
{
    float x;
    float y;
    float z;
};

//struct made of two indexes corresponding to the vertex list
//allows easy definition of edges
struct EdgeDef
{
    int i1;
    int i2;
};

//defines types of faces (as if you were facing it)
enum FaceType 
{
    forward,
    notforward
};

class Point{
    public:
        int id;
        bool visible;
        coords world, index;
        Point(int, int);
        Point();

        coords convertToIndex(int, int);
        bool isWorldEqual(Point);
};

class Point3D : public Point{
    public:
        //the original coords and the coords that are going to be rendered
        coords3D coord3D;
        coords3D render_coords3D;
        Point3D(coords3D, int, bool=true);
        Point3D();
        bool is3DEqual(Point3D);
        void setcoords3D(coords3D);
        void print3DCoords();
    private:
        void project3Dcoords();
};

class Edge{
    public:
        bool visible;
        Point3D &p1, &p2;
        Edge(Point3D &, Point3D &, bool);
};

//to store data about mathematical vectors (used to easily sort points in clockwise order)
class Vector
{
    public:
        Point3D *initial;
        Point3D *ending;
        coords3D vec;
        Vector(Point3D *initial, Point3D *ending) : initial(initial), ending(ending){
            vec = {ending->coord3D.x - initial->coord3D.x, ending->coord3D.y - initial->coord3D.y, ending->coord3D.z - initial->coord3D.z};
        }
};


#endif