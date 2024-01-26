#include "buffer.h"

ZBuffer::ZBuffer(int x, int y) : XMAX(x), YMAX(y), grid((y * 2) + 3 , vector<int>((x * 2) + 3, numeric_limits<int>::min())) {}

void ZBuffer::addToBuffer(int row, int col, int num){
    if(grid[row][col] < num){
        grid[row][col] = num;
    }
}

//get 3d coords of all points within triangle
void ZBuffer::resolveBuffer(TriangularPiece &tri){
    Point3D p1 = *tri.points_ref[1];
    Point3D p2 = *tri.points_ref[2];
    Point3D shared = *tri.points_ref[0];

    const int DIFF = 0;
    //1. get min and max
    int x_max = max(max(p1.render_coords3D.x, p2.render_coords3D.x), shared.render_coords3D.x) - DIFF; 
    int y_max = max(max(p1.render_coords3D.y, p2.render_coords3D.y), shared.render_coords3D.y) - DIFF; 
    int z_max = max(max(p1.render_coords3D.z, p2.render_coords3D.z), shared.render_coords3D.z); 

    int x_min = min(min(p1.render_coords3D.x, p2.render_coords3D.x), shared.render_coords3D.x) + DIFF; 
    int y_min = min(min(p1.render_coords3D.y, p2.render_coords3D.y), shared.render_coords3D.y) + DIFF; 
    int z_min = min(min(p1.render_coords3D.z, p2.render_coords3D.z), shared.render_coords3D.z); 

    for(int i = x_min; i <= x_max; i++){
        for(int j = y_min; j <= y_max; j++){
            //2. check if point is within triangle
            if(!PointInTriangle({i, j}, {shared.render_coords3D.x, shared.render_coords3D.y}, {p1.render_coords3D.x, p1.render_coords3D.y}, {p2.render_coords3D.x, p2.render_coords3D.y})){
                continue;
            }
            int new_x = i + XMAX + 1;
            int new_y = -j + YMAX + 1;
            float new_z = 0;
            //3. check if z coeff is 0 (COND END)
            if(tri.normal.z == 0){
                //add to z buffer using z_max
                new_z = z_min;
            }
            else{
                new_z = -(tri.d + tri.normal.x * i + tri.normal.y * j)/tri.normal.z;
            }

            int int_z = new_z;
            Point3D sample({i, j, int_z}, -1);

            //sample.project3DcoordsRound();
            //could use the raw world coordinates
            coords index = sample.convertToIndex(XMAX, YMAX);
            new_y = index.y;
            new_x = index.x;
            //new_y = -sample.render_coords3D.y + YMAX + 1;
            //new_x = sample.render_coords3D.x + XMAX + 1;
            
            addToBuffer(new_y, new_x, sample.render_coords3D.z);
        }
        
    }
}

void ZBuffer::outputBuffer(){
    string out = "";
    //output the entire grid
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            if(i == 0 || j == 0 || i == grid.size() - 1 || j == grid[i].size() - 1){
                out.push_back('/');
                out.push_back(' ');
                continue;
            }
            if(grid[i][j] != numeric_limits<int>::min()) {
                out.push_back(grid[i][j] + '0');
            }
            else{
                out.push_back(' ');
            }
            out.push_back(' ');
        }
        if(i != grid.size() - 1) out.push_back('\n');
    }
    cout << out;
}

void ZBuffer::clearBuffer(){
    for(int i = 0; i < grid.size(); i++){
        for(int j = 0; j < grid[i].size(); j++){
            grid[i][j] = numeric_limits<int>::min();
        }
    }
}
