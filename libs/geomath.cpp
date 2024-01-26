#include "geomath.h"
#include "mathdata.h"
#include <vector>

using namespace std;

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

//check if point is within the triangle on the x-y plane
float sign (coords p1, coords p2, coords p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool PointInTriangle (coords pt, coords v1, coords v2, coords v3)
{
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(pt, v1, v2);
    d2 = sign(pt, v2, v3);
    d3 = sign(pt, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}
