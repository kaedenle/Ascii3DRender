#include<cmath>
#include<iostream>
#include<algorithm>
#include "geometry.h"

#define RENDERCENTER false
#define RENDERSEMICENTER false

Shape::Shape(vector<coords3D> &points, vector<EdgeDef> &edges, vector<Face> &faces) : edge_index_list(edges), face_list(faces){
    //define vertex list
    for(int i = 0; i < points.size(); i++){
        vertex_list.push_back(Point3D(points[i], i));
    }

    //get center from points
    center = findCenter();

    //define edge list
    for(EdgeDef edge : edge_index_list){
        Edge e(vertex_list[edge.i1], vertex_list[edge.i2], true);
        edge_list.push_back(e);
    }

    //add all verticies to ref list
    for(int i = 0; i < points.size(); i++){
        vertex_list_ref.push_back(&vertex_list[i]);
    }

    //add center to list ref
    vertex_list_ref.push_back(&center);

    //initalize face list (connect this parent to the face)
    for(int i = 0; i < face_list.size(); i ++){
        //pass in current objects pointer
        face_list[i].init(this);
    }
}

void Shape::rotateShape(float x_rot, float y_rot, float z_rot){
    //convert degrees to rad
    x_rot *= 3.14159/180;
    y_rot *= 3.14159/180;
    z_rot *= 3.14159/180;

    //define rotation matrix for each axis
    vector< vector<float> > ROT_X{{1, 0, 0}, {0, cos(x_rot), -sin(x_rot)}, {0, sin(x_rot), cos(x_rot)}};
    vector< vector<float> > ROT_Y{{cos(y_rot), 0, sin(y_rot)}, {0, 1, 0}, {-sin(y_rot), 0, cos(y_rot)}};
    vector< vector<float> > ROT_Z{{cos(z_rot), -sin(z_rot), 0}, {sin(z_rot), cos(z_rot), 0}, {0, 0, 1}};

    for(int i = 0; i < vertex_list_ref.size(); i++){
        coords3D coord = vertex_list_ref[i]->coord3D;
        vector< vector<float> > result{{(float)coord.x}, {(float)coord.y}, {(float)coord.z}};
        result = mulMat(ROT_X, result);
        result = mulMat(ROT_Y, result);
        result = mulMat(ROT_Z, result);

        vertex_list_ref[i]->setcoords3D({(int)result[0][0], (int)result[1][0], (int)result[2][0]});
    }

    //change center coords (in case rotation changed the center)
    center = findCenter();
    //cout << "CENTER: " << center.coord3D.x << " " << center.coord3D.y << " " << center.coord3D.z << endl;
}

Point3D Shape::findCenter(){
    vector<int> x;
    vector<int> y;
    vector<int> z;

    //find all components
    for(Point3D p : vertex_list){
        x.push_back(p.render_coords3D.x);
        y.push_back(p.render_coords3D.y);
        z.push_back(p.render_coords3D.z);
    }

    int x_avg = average(x);
    int y_avg = average(y);
    int z_avg = average(z);

    Point3D ret({x_avg, y_avg, z_avg}, -1, RENDERCENTER);
    return ret;
}

Face::Face(FaceType facetype, vector<int> edge_index) : index_list(edge_index), faceRot(facetype), n(edge_index.size()){}

void Face::init(Shape *s){
    parent = s;
    getEdges();
    getPoints();
    calculateCenter(points);
    defineTriangles();
}

void Face::getEdges(){
    //get all edges being used
    for(int index : index_list){
        edges.push_back(&parent->edge_list[index]);
    }
}

//get all points within face
void Face::getPoints(){
    //keep track of what indexes used
    vector<int> used;

    vector<Point3D> &parent_list = parent->vertex_list;
    vector<EdgeDef> &parent_edgedef_list = parent->edge_index_list;

    for(int index : index_list){
        EdgeDef def = parent_edgedef_list[index];

        //check if verticies have not been iterated over
        if(find(used.begin(), used.end(), def.i1) == used.end()){
            used.push_back(def.i1);
            points.push_back(&parent_list[def.i1]);
        }

        if(find(used.begin(), used.end(), def.i2) == used.end()){
            used.push_back(def.i2);
            points.push_back(&parent_list[def.i2]);
        }
    }

    //int amount = used.size();
    //int a2 = points.size();
}

//calculate center
void Face::calculateCenter(vector<Point3D *> shape_points){
    vector<Point3D> pass_in;
    for(Point3D *p : shape_points){
        pass_in.push_back(*p);
    }

    coords3D c = calcCenter(pass_in);
    center = Point3D(c, -1, RENDERCENTER);

    //add to vectors (native and parent)
    points.push_back(&center);
    parent->vertex_list_ref.push_back(&center);
}

void Face::defineTriangles(){
    //if face is a triangle (only one triangle)
    if(n == 3){
        //add new triangle to triangle list
        triangle_list.emplace_back(edges, faceRot, parent);
        return;
    }
    //convert non-triangular faces into triangles
    
    //iterate over all edges and create a triangle with the center point
    for(Edge *edge : edges){
        //made up edges
        face_edges.push_back(Edge(center, edge->p1, false));
        Edge *e1 = &face_edges.back();
        face_edges.push_back(Edge(center, edge->p2, false));
        Edge *e2 = &face_edges.back();

        //add new triangle to triangle list
        triangle_list.emplace_back(vector<Edge*>{edge, e1, e2}, faceRot, parent);
    }
}

//sole purpose of triangular piece is to sort the pointers
TriangularPiece::TriangularPiece(vector<Edge*> edges, FaceType ft, Shape *parent) : face(ft), edges_ref(edges), parent(parent){
    getPoints();
    //get center to calc rotation
    vector<Point3D> points;
    for(Point3D *p : points_ref){
        points.push_back(*p);
    }

    coords3D c = calcCenter(points);
    center = Point3D(c, -2, RENDERSEMICENTER);
    //center will be added to points_ref later (after sorting)
    parent->vertex_list_ref.push_back(&center);

    sortClockwise();

    points_ref.push_back(&center);
};

/*
//deconstructor
TriangularPiece::~TriangularPiece(){
    for(Edge *e : edges_ref){
        delete(e);
    }
}
*/

//extracts all unique points from edges and puts them into points
void TriangularPiece::getPoints(){
    for(Edge* edge : edges_ref){
        addPoint(&edge->p1);
        addPoint(&edge->p2);
    }
}

void TriangularPiece::addPoint(Point3D *p){
    //if point3D already exists in points don't add it
    for(Point3D* point : points_ref){
        if(point == p)
            return;
    }
    points_ref.push_back(p);
}

//sort points clockwise
void TriangularPiece::sortClockwise(){
    //calculate camera vector
    coords3D camera_vec = {0 - center.coord3D.x, 0 - center.coord3D.y, CONST_Z - center.coord3D.z};

    //setup Vector vector for organization
    vector<Vector> vec_list;
    for(Point3D* point : points_ref){
        vec_list.push_back({&center, point});
    }

    //start the sort
    vector<Point3D*> sorted_list;
    while(sorted_list.size() != points_ref.size()){
        int index = 0;

        //iterate through all vec_list (except the first element)
        for(int i = 1; i < vec_list.size(); i++){
            //calculate normal
            coords3D normal = crossProduct(vec_list[i].vec, vec_list[index].vec);
            //dot product
            float dot = dotProduct(normal, camera_vec);

            //if normal is the wrong way (away from camera) new value
            if(dot < 0 && (face == FaceType::forward)){
                index = i;
            }

            if(dot > 0 && (face != FaceType::forward)){
                index = i;
            }

            //if reversed
            /*if(dot > 0 && (face == FaceType::back || face == FaceType::right ||face == FaceType::top ||  face == FaceType::left ||face == FaceType::bottom)){
                index = i;
            }*/
        }
        sorted_list.push_back(vec_list[index].ending);
        vec_list.erase(vec_list.begin() + index);
    }

    //set the sorted list
    points_ref = sorted_list;
}