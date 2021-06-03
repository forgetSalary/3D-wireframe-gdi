#include "obj_parser.cpp"

POINT3 SphericalCoords::to_cartesian() {
    if (phi == 0){
        return PointEx(0,0,Round(r.radius));
    }

    double sin_phi = sin(DegreeToRad(phi.angle));
    double sin_theta = sin(DegreeToRad(phi.angle));
    double cos_phi = cos(DegreeToRad(phi.angle));
    double cos_theta = cos(DegreeToRad(phi.angle));
    return PointEx(Round(r.radius * sin_theta * cos_phi), Round(r.radius * sin_theta * sin_phi), Round(r.radius * cos_theta));
}

static POINT RenderPoint(POINT3 world_point,Camera cam,double dc, int w_height, int w_width ){
    //view transformation:
    double A = DegreeToRad(cam.center_position.phi.angle);
    double B = DegreeToRad(cam.center_position.theta.angle);

    double sin_A = sin(A);
    double sin_B = sin(B);
    double cos_A = cos(A);
    double cos_B = cos(B);

    double X = world_point.x, Y= world_point.y, Z= world_point.z;

    double M3[3] = {Y*cos_A - X*sin_A,
                    Z*sin_B - X*cos_A*cos_B - Y*sin_A*cos_B,
                    -X*cos_A*cos_B - Y*sin_A*sin_B - Z*cos_B + cam.center_position.r.radius};

    POINT3 screen_dot = PointEx(M3[0], M3[1], M3[2]);

    //perspective:
    double sin_X = sin(DegreeToRad(cam.orientation.x));
    double sin_Y = sin(DegreeToRad(cam.orientation.y));
    double sin_Z = sin(DegreeToRad(cam.orientation.z));
    double cos_X = cos(DegreeToRad(cam.orientation.x));
    double cos_Y = cos(DegreeToRad(cam.orientation.y));
    double cos_Z = cos(DegreeToRad(cam.orientation.z));

    X = screen_dot.x,Y = screen_dot.y, Z = screen_dot.z;

    POINT3 d = PointEx(cos_Y*(sin_Z*Y + cos_Z*X) - sin_Y*Z,
                       sin_X*(cos_Y*Z + sin_Y*(sin_Z*Y + cos_Z*X)) + cos_X*(cos_Z*Y - sin_Z*X),
                       cos_X*(cos_Y*Z + sin_Y*(sin_Z*Y + cos_Z*X)) - sin_X*(cos_Z*Y - sin_Z*X));

    POINT projected = Point(Round((d.x)/(d.z)*dc),
                            Round((d.y)/(d.z)*dc));

    //screen transformation
    return Point(projected.x + w_width/2 + cam.offsets.x,-projected.y + w_height/1.5 + cam.offsets.y);
}


void Object::update(Camera& cam, double dc,int w_height, int w_width) {
    for (int i = 0; i < vertices_count; ++i) {
        screen_points[i] = RenderPoint(world_points[i],cam,dc,w_height,w_width);
    }
}

void Object::draw(HDC hdc,COLORREF color) {
    SelectObject(hdc,CreatePen(PS_SOLID,0,color));

    POINT p1,p2;
    for (int i = 0; i < edges_count; ++i) {
        p1 = screen_points[edges[i].order[0]];
        MoveToEx(hdc,p1.x,p1.y, nullptr);
        for (int j = 1; j < edges[i].vertices_count; ++j) {
            p2 = screen_points[edges[i].order[j]];
            LineTo(hdc,p2.x,p2.y);
        }
        LineTo(hdc,p1.x,p1.y);
    }
}
#define REDERED(x,y,z) RenderPoint(PointEx(x,y,z),cam,dc,w_height,w_width)
void draw_ground(HDC hdc,Camera& cam, double dc,int w_height, int w_width){
    SelectObject(hdc,CreatePen(PS_SOLID,0,RGB(156, 156, 156)));

    int range[2] = {-150,150};
    int step = 5;
    int lines_count = (-range[0]+range[1])/step;

    POINT p1,p2;
    //vertical
    for (int i = 0; i < lines_count; ++i) {
        p1 = REDERED(range[0] + step * i, range[0], 0);
        p2 = REDERED(range[0] + step * i, range[1], 0);

        MoveToEx(hdc,p1.x,p1.y, nullptr);
        LineTo(hdc,p2.x,p2.y);
    }

    //horizontal
    for (int i = 0; i < lines_count; ++i) {
        p1 = REDERED(range[0], range[0] + step * i, 0);
        p2 = REDERED(range[1], range[0] + step * i, 0);

        MoveToEx(hdc,p1.x,p1.y, nullptr);
        LineTo(hdc,p2.x,p2.y);
    }
}

void draw_coordinate_lines(HDC hdc, Camera& cam, double dc, int w_height, int w_width){
    int len = 100;

    POINT p1,p2;
    p1 = REDERED(0, 0, 0);

    //x
    SelectObject(hdc,CreatePen(PS_SOLID,1,RGB(255, 0, 0)));
    p2 = REDERED(len,0,0);

    MoveToEx(hdc,p1.x,p1.y,NULL);
    LineTo(hdc,p2.x,p2.y);

    //y
    SelectObject(hdc,CreatePen(PS_SOLID,1,RGB(0, 0, 255)));
    p2 = REDERED(0,len,0);
    MoveToEx(hdc,p1.x,p1.y,NULL);
    LineTo(hdc,p2.x,p2.y);

    //z
    SelectObject(hdc,CreatePen(PS_SOLID,1,RGB(0, 255, 0)));
    p2 = REDERED(0,0,len);
    MoveToEx(hdc,p1.x,p1.y,NULL);
    LineTo(hdc,p2.x,p2.y);
}