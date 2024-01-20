#ifndef GEOMETRY_H
#define GEOMETRY_H

using namespace std;

#include<vector>
#include<list>
#include<cmath>
#include<iostream>
#include "mathdata.h"
#include "geomath.h"

class Shape;
class Face;
class TriangularPiece;
class ShapeBase;

//to allow easy access to math functions
class GeoBase{
    public:
        bool backfaceCull(Point3D &p1, Point3D &p2, Point3D &shared){
            coords3D vec1 = {p1.render_coords3D.x - shared.render_coords3D.x, p1.render_coords3D.y - 
                                shared.render_coords3D.y,  p1.render_coords3D.z - shared.render_coords3D.z};
            coords3D vec2 = {p2.render_coords3D.x - shared.render_coords3D.x, p2.render_coords3D.y - 
                            shared.render_coords3D.y,  p2.render_coords3D.z - shared.render_coords3D.z};
            
            coords3D cross = crossProduct(vec1, vec2);
            coords3D cameraRay = {shared.render_coords3D.x - 0, shared.render_coords3D.y - 0, shared.render_coords3D.z - (int)((CONST_Z * 2))};
            
            float ret = dotProduct(cross, cameraRay);
            //cout << "(" << p1.id << " " << p2.id << ") " << ret << endl;;
            //p1.print3DCoords();
            //p2.print3DCoords();
            //cout << vec1.x << " " << vec1.y << " " << vec1.z << endl;
            //cout << vec2.x << " " << vec2.y << " " << vec2.z << endl;
            //cout << shared.render_coords3D.x << " " << shared.render_coords3D.y << " " << shared.render_coords3D.z << endl;
            return ret <= 0;
        }
};

class Shape : private GeoBase{
    public:
        //used to rotate
        vector<Point3D> vertex_list;
        vector<Edge> edge_list;
        vector<Face> face_list;

        //for extra points/edges defined in faces/triangles (used for rotation)
        vector<Point3D *> vertex_list_ref;
        //vector<Edge *> edge_list_ref;

        vector<EdgeDef> edge_index_list;
        Point3D center;

        void rotateShape(float, float, float);
        Point3D findCenter();
        Shape(vector<coords3D> &, vector<EdgeDef> &, vector<Face> &);
        Shape();
};


class Face : private GeoBase{
    //collection of indexes (corresponding to the edge list) defining a set of edges defining a face
    vector<int> index_list;
    Point3D center;
    Shape *parent;

    //all edges/points making up the face (including invisible ones)
    vector<Edge*> edges;
    vector<Point3D*> points;
    list<Edge> face_edges;

    int n;
    
    public:
        //hidden edges making up the triangle
        list<TriangularPiece> triangle_list;

        //type of rotation associated with the face by default
        FaceType faceRot;

        Face(FaceType, vector<int>);
        void init(Shape *);
    private:
        void getEdges();
        void getPoints();

        void calculateCenter(vector<Point3D *>);
        void defineTriangles();
};

//defines triangular pieces of shape
class TriangularPiece : public GeoBase{
    public:
        FaceType face;
        Point3D center;
        coords3DFloat normal;
        Shape *parent;

        float d;

        //will be ordered eventually
        vector<Point3D*> points_ref;
        vector<Edge*> edges_ref;
        vector<Point3D> in_betweens;

        TriangularPiece(vector<Edge*>, FaceType, Shape *);
        //~TriangularPiece();
    private:
        void getPoints();
        void addPoint(Point3D *);
        void sortClockwise();
        void getEquation(Point3D &, Point3D &, Point3D &);
        void getInBetweens(Point3D &, Point3D &, Point3D &);
        //have a boolean within points/edges that tells whether it's been culled or not?
        //have to reset it eventually however
};

#endif