#ifndef GEOMETRY_H
#define GEOMETRY_H

using namespace std;

#include<vector>
#include<list>
#include<cmath>
#include<iostream>
#include "mathdata.h"

class Shape;
class Face;
class TriangularPiece;
class ShapeBase;

//to allow easy access to math functions
class GeoBase{
    public:
        coords3D calcCenter(vector<Point3D> &shape_points){
            float sum_x = 0, sum_y = 0, sum_z = 0, sum_L = 0;

            for(int i = 0; i < shape_points.size(); i++){
                int last_i = i == 0 ? shape_points.size() - 1 : i - 1;
                int x0 = shape_points[last_i].coord3D.x, y0 = shape_points[last_i].coord3D.y, z0 = shape_points[last_i].coord3D.z;
                int x1 = shape_points[i].coord3D.x, y1 = shape_points[i].coord3D.y, z1 = shape_points[i].coord3D.z;

                float L = sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2) + pow(z1 - z0, 2));

                sum_x += (x0 + x1)/2 * L;
                sum_y += (y0 + y1)/2 * L;
                sum_z += (z0 + z1)/2 * L;
                sum_L += L;
            }
            int xc = sum_x / sum_L, yc = sum_y / sum_L, zc = sum_z / sum_L;

            return {xc, yc, zc};
        }

        //for 3 verticies that are connected
        coords3D crossProduct(coords3D vector1, coords3D vector2){
            int x = (vector1.y * vector2.z) - (vector1.z * vector2.y);
            int y = (vector1.z * vector2.x) - (vector1.x * vector2.z);
            int z = (vector1.x * vector2.y) - (vector1.y * vector2.x);

            return {x, y, z};
        }

        //for 3 verticies that are connected
        coords3DFloat crossProduct(coords3DFloat vector1, coords3DFloat vector2){
            float x = (vector1.y * vector2.z) - (vector1.z * vector2.y);
            float y = (vector1.z * vector2.x) - (vector1.x * vector2.z);
            float z = (vector1.x * vector2.y) - (vector1.y * vector2.x);

            return {x, y, z};
        }

        //get dot product of vectors
        float dotProduct(coords3D vector1, coords3D vector2){
            float x = vector1.x * vector2.x;
            float y = vector1.y * vector2.y;
            float z = vector1.z * vector2.z;
            return x + y + z;
        }

        //get magnitude of a vector
        float magnitude(coords3DFloat vec){
            return sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
        }

        float magnitude(coords3D vec){
            return sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
        }

        //normalize a vector
        coords3DFloat normalize(coords3DFloat vector){
            float mag = 1/magnitude(vector);
            vector.x *= mag;
            vector.y *= mag;
            vector.z *= mag;
            return vector;
        }

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

        bool CCWCheck(Point3D &p1, Point3D &p2){
            //matrix construction
            vector< vector<int> >unit1 = {{0}, {0}, {1}};
            vector< vector<int> >unit2 = {{1}, {0}, {1}};
            vector< vector<int> >unit3 = {{0}, {1}, {1}};
        }

        vector< vector<float> > mulMat(vector< vector<float> > mat1, vector< vector<float> > mat2)
        {
            int R1 = mat1.size();
            int C1 = mat1[0].size();

            int R2 = mat2.size();
            int C2 = mat2[0].size();

            vector< vector<float> >rslt(R1, vector<float>(C2, 0));
        
            for (int i = 0; i < R1; i++) {
                for (int j = 0; j < C2; j++) {
                    rslt[i][j] = 0;
        
                    for (int k = 0; k < R2; k++) {
                        rslt[i][j] += mat1[i][k] * mat2[k][j];
                    }
        
                    //cout << rslt[i][j] << "\t";
                }
                //cout << endl;
            }

            return rslt;
        }

        int average (vector<int> &v){
            int sum = 0;

            for(int n : v){
                sum += n;
            }

            int s = v.size();
            sum /= s;

            return sum;
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
        Shape *parent;

        //will be ordered eventually
        vector<Point3D*> points_ref;
        vector<Edge*> edges_ref;

        TriangularPiece(vector<Edge*>, FaceType, Shape *);
        //~TriangularPiece();
    private:
        void getPoints();
        void addPoint(Point3D *);
        void sortClockwise();
        //have a boolean within points/edges that tells whether it's been culled or not?
        //have to reset it eventually however
};

#endif