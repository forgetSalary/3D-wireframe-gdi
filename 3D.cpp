#include "obj_parser.cpp"
#include "thread"
#include "atomic"

#define ATM(T) std::atomic<T>

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

static POINT3 GlobalPointToScreen(POINT3 world_point, SphericalCoords C) {
    double A = DegreeToRad(C.phi.angle);
    double B = DegreeToRad(C.theta.angle);

    double sin_A = sin(A);
    double sin_B = sin(B);
    double cos_A = cos(A);
    double cos_B = cos(B);

    double X = world_point.x, Y= world_point.y, Z= world_point.z;

    double M3[3] = {Y*cos_A - X*sin_A,
                    Z*sin_B - X*cos_A*cos_B - Y*sin_A*cos_B,
                    -X*cos_A*cos_B - Y*sin_A*sin_B - Z*cos_B + C.r.radius};

    return PointEx(M3[0], M3[1], M3[2]);
}

static POINT Perspective(POINT3 PT, Camera& cam, double dc){
    double sin_X = sin(DegreeToRad(cam.orientation.x));
    double sin_Y = sin(DegreeToRad(cam.orientation.y));
    double sin_Z = sin(DegreeToRad(cam.orientation.z));
    double cos_X = cos(DegreeToRad(cam.orientation.x));
    double cos_Y = cos(DegreeToRad(cam.orientation.y));
    double cos_Z = cos(DegreeToRad(cam.orientation.z));

    double X = PT.x,Y = PT.y, Z = PT.z;

    POINT3 d = PointEx(cos_Y*(sin_Z*Y + cos_Z*X) - sin_Y*Z,
                       sin_X*(cos_Y*Z + sin_Y*(sin_Z*Y + cos_Z*X)) + cos_X*(cos_Z*Y - sin_Z*X),
                       cos_X*(cos_Y*Z + sin_Y*(sin_Z*Y + cos_Z*X)) - sin_X*(cos_Z*Y - sin_Z*X));

    POINT projection = Point(Round((d.x)/(d.z)*dc),
                             Round((d.y)/(d.z)*dc));
    return projection;
}

static inline POINT ScreenPoint(POINT PT,Camera cam, int w_height, int w_width){
    return Point(PT.x + w_width/2 + cam.offsets.x,-PT.y + w_height/1.5 + cam.offsets.y);
}

void Object::update(Camera& cam, double dc,int w_height, int w_width) {
    for (int i = 0; i < vertices_count; ++i) {
        POINT3 EPT = GlobalPointToScreen(world_points[i],cam.center_position);
        screen_points[i] = ScreenPoint(Perspective(EPT,cam,dc),cam,w_height,w_width);
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

void draw_ground(HDC hdc,Camera& cam, double dc,int w_height, int w_width){
    SelectObject(hdc,CreatePen(PS_SOLID,0,RGB(156, 156, 156)));

    int range[2] = {-150,150};
    int step = 1;
    int lines_count = (-range[0]+range[1])/step;

    POINT p1,p2;
    //vertical
    for (int i = 0; i < lines_count; ++i) {
        p1 = ScreenPoint(
                Perspective(
                        GlobalPointToScreen(
                                PointEx(range[0]+step*i,range[0],0),cam.center_position),
                                cam,dc),cam,w_height,w_width);

        p2 = ScreenPoint(
                Perspective(
                        GlobalPointToScreen(
                                PointEx(range[0]+step*i,range[1],0),cam.center_position),
                        cam,dc),cam,w_height,w_width);

        MoveToEx(hdc,p1.x,p1.y, nullptr);
        LineTo(hdc,p2.x,p2.y);
    }

    //horizontal
    for (int i = 0; i < lines_count; ++i) {
        p1 = ScreenPoint(
                Perspective(
                        GlobalPointToScreen(
                                PointEx(range[0],range[0]+step*i,0),cam.center_position),
                        cam,
                        dc),cam, w_height,w_width);

        p2 = ScreenPoint(
                Perspective(
                        GlobalPointToScreen(
                                PointEx(range[1],range[0]+step*i,0),cam.center_position),
                        cam,
                        dc),cam,w_height,w_width);

        MoveToEx(hdc,p1.x,p1.y, nullptr);
        LineTo(hdc,p2.x,p2.y);
    }
}

void draw_coordinate_lines(HDC hdc, Camera& cam, double dc, int w_height, int w_width){
    int len = 100;

    POINT p1,p2;
    p1 = ScreenPoint(Perspective(GlobalPointToScreen(
                            PointEx(0,0,0),cam.center_position),
                    cam,dc),cam,w_height,w_width);

    //x
    SelectObject(hdc,CreatePen(PS_SOLID,1,RGB(255, 0, 0)));
    p2 = ScreenPoint(Perspective(GlobalPointToScreen(
                            PointEx(len,0,0),cam.center_position),
                    cam,dc),cam,w_height,w_width);
    MoveToEx(hdc,p1.x,p1.y,NULL);
    LineTo(hdc,p2.x,p2.y);

    //y
    SelectObject(hdc,CreatePen(PS_SOLID,1,RGB(0, 0, 255)));
    p2 = ScreenPoint(Perspective(GlobalPointToScreen(
            PointEx(0,len,0),cam.center_position),
                                 cam,dc),cam,w_height,w_width);
    MoveToEx(hdc,p1.x,p1.y,NULL);
    LineTo(hdc,p2.x,p2.y);

    //z
    SelectObject(hdc,CreatePen(PS_SOLID,1,RGB(0, 255, 0)));
    p2 = ScreenPoint(Perspective(GlobalPointToScreen(
            PointEx(0,0,len),cam.center_position),
                                 cam,dc),cam,w_height,w_width);
    MoveToEx(hdc,p1.x,p1.y,NULL);
    LineTo(hdc,p2.x,p2.y);
}