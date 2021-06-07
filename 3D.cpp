#include "obj_parser.cpp"

POINT3 SphericalCoords::to_cartesian() {
    if (phi == 0){
        return PointEx(0,0,Round(r.value));
    }

    double sin_phi = sin(DegreeToRad(phi.value));
    double sin_theta = sin(DegreeToRad(phi.value));
    double cos_phi = cos(DegreeToRad(phi.value));
    double cos_theta = cos(DegreeToRad(phi.value));
    return PointEx(Round(r.value)*Round(sin_theta)*Round(cos_phi), Round(r.value * sin_theta * sin_phi), Round(r.value * cos_theta));
}

static POINT RenderPoint(POINT3 world_point,Camera cam, int w_height, int w_width ){
    //view transformation:
    double A = DegreeToRad(cam.center_position.phi.value);
    double B = DegreeToRad(cam.center_position.theta.value);

    double sin_A = sin(A);
    double sin_B = sin(B);
    double cos_A = cos(A);
    double cos_B = cos(B);

    double X = world_point.x, Y= world_point.y, Z= world_point.z;

    double screen_dot[3] = {Y*cos_A - X*sin_A,
                    Z*sin_B - X*cos_A*cos_B - Y*sin_A*cos_B,
                    -X*cos_A*sin_B - Y*sin_A*sin_B - Z*cos_B + cam.center_position.r.value};

    //perspective:
    double sin_X = sin(DegreeToRad(cam.orientation.x.value));
    double sin_Y = sin(DegreeToRad(cam.orientation.y.value));
    double sin_Z = sin(DegreeToRad(cam.orientation.z.value));
    double cos_X = cos(DegreeToRad(cam.orientation.x.value));
    double cos_Y = cos(DegreeToRad(cam.orientation.y.value));
    double cos_Z = cos(DegreeToRad(cam.orientation.z.value));

    X = screen_dot[0],Y = screen_dot[1], Z = screen_dot[2];

    double d[3] = {cos_Y*(sin_Z*Y + cos_Z*X) - sin_Y*Z,
                  sin_X*(cos_Y*Z + sin_Y*(sin_Z*Y + cos_Z*X)) + cos_X*(cos_Z*Y - sin_Z*X),
                  cos_X*(cos_Y*Z + sin_Y*(sin_Z*Y + cos_Z*X)) - sin_X*(cos_Z*Y - sin_Z*X)};

    POINT projected = Point(Round((d[0])/(d[2])*cam.dc),
                            Round((d[1])/(d[2])*cam.dc));

    //screen transformation
    return Point(projected.x + w_width/2 + cam.offsets.x,-projected.y + w_height/1.5 + cam.offsets.y);
}


void Object::update(Camera& cam, int w_height, int w_width) {
    for (int i = 0; i < vertices_count; ++i) {
        screen_points[i] = RenderPoint(world_points[i],cam,w_height,w_width);
    }
}

void Object::draw(HDC hdc,COLORREF color) {
    SelectObject(hdc,CreatePen(PS_SOLID,0,color));

    POINT p1,p2;
    for (int i = 0; i < edges_count; ++i) {
        p1 = screen_points[faces[i].order[0]];
        MoveToEx(hdc,p1.x,p1.y, nullptr);
        for (int j = 1; j < faces[i].vertices_count; ++j) {
            p2 = screen_points[faces[i].order[j]];
            LineTo(hdc,p2.x,p2.y);
        }
        LineTo(hdc,p1.x,p1.y);
    }
}

#define RENDER(x,y,z) RenderPoint(PointEx(x,y,z),cam,w_height,w_width)
void draw_ground(HDC hdc,Camera& cam,int w_height, int w_width){
    SelectObject(hdc,CreatePen(PS_SOLID,0,RGB(156, 156, 156)));

    int range[2] = {-150,150};
    int step = 5;
    int lines_count = (-range[0]+range[1])/step;

    POINT p1,p2;
    //vertical
    for (int i = 0; i < lines_count; ++i) {
        p1 = RENDER(range[0] + step * i, range[0], 0);
        p2 = RENDER(range[0] + step * i, range[1], 0);

        MoveToEx(hdc,p1.x,p1.y, nullptr);
        LineTo(hdc,p2.x,p2.y);
    }

    //horizontal
    for (int i = 0; i < lines_count; ++i) {
        p1 = RENDER(range[0], range[0] + step * i, 0);
        p2 = RENDER(range[1], range[0] + step * i, 0);

        MoveToEx(hdc,p1.x,p1.y, nullptr);
        LineTo(hdc,p2.x,p2.y);
    }
}

void draw_coordinate_lines(HDC hdc, Camera& cam, int w_height, int w_width){
    int len = 100;

    POINT p1,p2;
    p1 = RENDER(0, 0, 0);

    //x
    SelectObject(hdc,CreatePen(PS_SOLID,1,RGB(255, 0, 0)));
    p2 = RENDER(len, 0, 0);

    MoveToEx(hdc,p1.x,p1.y,NULL);
    LineTo(hdc,p2.x,p2.y);

    //y
    SelectObject(hdc,CreatePen(PS_SOLID,1,RGB(0, 0, 255)));
    p2 = RENDER(0, len, 0);
    MoveToEx(hdc,p1.x,p1.y,NULL);
    LineTo(hdc,p2.x,p2.y);

    //z
    SelectObject(hdc,CreatePen(PS_SOLID,1,RGB(0, 255, 0)));
    p2 = RENDER(0, 0, len);
    MoveToEx(hdc,p1.x,p1.y,NULL);
    LineTo(hdc,p2.x,p2.y);
}