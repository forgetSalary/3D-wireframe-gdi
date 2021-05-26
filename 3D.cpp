#include "obj_parser.cpp"

template <typename T>
struct Matrix {
    T** data;
    size_t size_a;
    size_t size_b;

    T* operator[](int index) { return data[index]; }
};

double static inline DegreeToRad(double Angle) {
    return (((double)Angle * PI) / 180);
}

double static inline Abs(double X) {
    return (X < 0 ? -X : X);
}

void CreatePyramid(POINT3* WPT,Edge* edges, POINT3 surface_centre, int height, int edge_len, int edges_count) {
    // generating screen_vertices
    WPT[0] = surface_centre;
    WPT[0].z += height;

    WPT[1].x = 0;
    WPT[1].y = edge_len;
    WPT[1].z = surface_centre.z;

    double segment_angle_rad = DegreeToRad(360 / edges_count);
    double cos_a = cos(segment_angle_rad);
    double sin_a = sin(segment_angle_rad);

    for (size_t i = 2; i < edges_count + 1; i++) {
        WPT[i] = PointEx(Round(WPT[i - 1].x * cos_a - WPT[i - 1].y * sin_a),
                         Round(WPT[i - 1].x * sin_a + WPT[i - 1].y * cos_a),
                         surface_centre.z);
    }

    for (size_t i = 1; i < edges_count + 1; i++) {
        WPT[i].x += surface_centre.x;
        WPT[i].y += surface_centre.y;
    }


    //generating Edge
    for (int i = 0; i < edges_count; ++i){
        edges[0].order[i] = i+1;
    }
    edges[0].order[edges_count] = 1;

    for (int i = 1; i < edges_count+1; ++i) {
        edges[i].order[0] = 0;
        edges[i].order[1] = i;
    }
}

void ObjectRotateX(POINT3* points, int count, double deg) {
    int M1[4];

    double A = DegreeToRad(deg);

    double sin_A = sin(A);
    double cos_A = cos(A);

    double M2[4][4] = { {1,0    ,0      ,0},
                        {0,cos_A,-sin_A, 0},
                        {0,sin_A,cos_A,  0},
                        {0,0    ,0      ,1} };
    int M3[4];
    int S = 0;

    //Умножаю вектор на матрицу
    for (size_t i = 0; i < count; i++) {
        M1[0] = points[i].x;
        M1[1] = points[i].y;
        M1[2] = points[i].z;
        M1[3] = 1;

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
                S = S + Round(M1[j] * (double)M2[i][j]);
            M3[i] = S; S = 0;
        }
        points[i] = PointEx(M3[0], M3[1], M3[2]);
    }
}
void ObjectRotateY(POINT3* points, int count, double deg) {
    int M1[4];
    double A = DegreeToRad(deg);

    double sin_A = sin(A);
    double cos_A = cos(A);

    double M2[4][4] = { {cos_A, 0,sin_A,0},
                        {0,     1,0    ,0},
                        {-sin_A,0,cos_A,0},
                        {0,     0,0    ,1} };

    int M3[4];
    int S = 0;

    for (size_t i = 0; i < count; i++) {
        M1[0] = points[i].x;
        M1[1] = points[i].y;
        M1[2] = points[i].z;
        M1[3] = 1;

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
                S = S + Round(M1[j] * (double)M2[i][j]);
            M3[i] = S; S = 0;
        }
        points[i] = PointEx(M3[0], M3[1], M3[2]);
    }
}
void ObjectRotateZ(POINT3* points, int count, double deg) {
    int M1[4];

    double A = DegreeToRad(deg);

    double sin_A = sin(A);
    double cos_A = cos(A);

    double M2[4][4] = { {cos_A,-sin_A,0,0},
                        {sin_A,cos_A, 0,0},
                        {0,    0,     1,0},
                        {0,    0,     0,1} };
    int M3[4];
    int S = 0;

    //Умножаю вектор на матрицу
    for (size_t i = 0; i < count; i++) {
        M1[0] = points[i].x;
        M1[1] = points[i].y;
        M1[2] = points[i].z;
        M1[3] = 1;

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
                S = S + Round(M1[j] * (double)M2[i][j]);
            M3[i] = S; S = 0;
        }
        points[i] = PointEx(M3[0], M3[1], M3[2]);
    }
}

POINT3 GlobalPointToScreen(POINT3 world_point, double A, double B, int R) {
    A = DegreeToRad(A);
    B = DegreeToRad(B);

    double sin_A = sin(A);
    double sin_B = sin(B);
    double cos_A = cos(A);
    double cos_B = cos(B);

    double X = world_point.x, Y= world_point.y, Z= world_point.z;

    int M3[3] = {Round(Y*cos_A - X*sin_A),
                 Round(Z*sin_B - X*cos_A*cos_B - Y*sin_A*cos_B),
                 Round(-X*cos_A*cos_B - Y*sin_A*sin_B - Z*cos_B + R)};

    POINT3 res = PointEx(M3[0], M3[1], M3[2]);
    return res;
}

POINT Perspective(POINT3 PT, double d) {
    int x = d*((double)PT.x);
    int y = d*((double)PT.y);

    return Point(x, y);
}

POINT ToScreenPoint(POINT PT, int w_height, int w_width){
    return Point(PT.x + w_width/2,-PT.y + w_height/2);
}

void Object::update(ViewPoint &vp, int w_height, int w_width) {
    for (int i = 0; i < vertices_count; ++i) {
        screen_points[i] = ToScreenPoint(Perspective(GlobalPointToScreen(
                world_points[i], vp.A, vp.B, vp.R), vp.d), w_height, w_width);
    }
}

void Object::draw(HDC hdc) {
    POINT p1,p2;
    for (int i = 0; i < edges_count; ++i) {
        for (int j = 0; j < edges[i].vertices_count; ++j) {
            p1 = screen_points[edges[i].order[j]];
            p2 = screen_points[edges[i].order[j+1]];
            MoveToEx(hdc,p1.x,p1.y, nullptr);
            LineTo(hdc,p2.x,p2.y);
        }
//        p1 = screen_points[edges[i].order[edges[i].vertices_count]];
//        p2 = screen_points[edges[i].order[0]];
//        MoveToEx(hdc,p1.x,p1.y, nullptr);
//        LineTo(hdc,p2.x,p2.y);
    }
}