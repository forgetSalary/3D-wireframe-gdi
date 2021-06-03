#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "framework.h"
#include "arena.h"

#define PI ((double)3.14159265359)

struct POINT3{
    double x;
    double y;
    double z;
};

inline POINT Point(int x, int y){
    return { x,y };
}

inline POINT3 PointEx(double x, double y, double z) {
    return { x,y,z };
}

inline int Round(double x){
    if (x < 0) x -= 0.5; else x += 0.5;
    return (int)x;
}

inline double DegreeToRad(double Angle) {
    return (((double)Angle * PI) / 180);
}

#define ANGLE_UPPER_LIMIT(deg) ((deg) < 360 ? (deg) : (deg)-360)
#define ANGLE_LOWER_LIMIT(deg) ((deg) > 0 ? (deg) : (deg)+360)

struct Radius{
    double radius;

    inline double operator =(double r){
        radius = (r > 0 ? r : 0);
        return radius;
    }

    inline void operator +=(double r){
        radius += r;
    }
    inline void operator -=(double r){
        radius = (radius-r > 0 ? radius-r : 0);
    }
    inline void operator *=(double r){
        radius *= r;
    }
    inline void operator /=(double r){
        radius /= r;
    }
    inline bool operator ==(double r){
        return radius == r;
    }
    inline bool operator !=(double r){
        return radius != r;
    }
};

struct Angle{
    double angle;

    void operator +=(double deg){
        angle = ANGLE_UPPER_LIMIT(angle+deg);
    }
    void operator -=(double deg){
        angle = ANGLE_LOWER_LIMIT(angle-deg);
    }
    void operator *=(double deg){
        angle = ANGLE_UPPER_LIMIT(angle*deg);
    }
    void operator /=(double deg){
        angle /= deg;
    }
    inline bool operator ==(double deg){
        return angle == deg;
    }
    inline bool operator !=(double deg){
        return angle != deg;
    }
};

struct SphericalCoords{
    Radius r;
    Angle  phi;
    Angle  theta;

    POINT3 to_cartesian();
};

struct TaitBryanAngles{
    double x;
    double y;
    double z;
};

struct Camera{
    SphericalCoords center_position;
    TaitBryanAngles orientation;
    POINT offsets;
};

struct Edge{
    uint32_t* order;
    uint32_t vertices_count;
};

class Object{
private:
    POINT3* world_points;
    POINT* screen_points;
    Edge* edges;
    uint32_t edges_count;
    uint32_t vertices_count;
public:
    Object(){
        world_points = nullptr;
        screen_points = nullptr;
        edges = nullptr;
        edges_count = 0;
        vertices_count = 0;
    }
    Object(POINT3* world_points,POINT* screen_points, Edge* edges,uint32_t vertices_count, uint32_t edges_count){
        this->world_points = world_points;
        this->screen_points = screen_points;
        this->edges = edges;
        this->edges_count = edges_count;
        this->vertices_count = vertices_count;
    }
    void update(Camera& cam, double dc, int w_height, int w_width);
    void draw(HDC hdc,COLORREF color);
};

void draw_ground(HDC hdc,Camera& cam, double dc,int w_height, int w_width);
void draw_coordinate_lines(HDC hdc, Camera& cam, double dc, int w_height, int w_width);

Object* parse_obj(arena_t* arena,const char* fname);

#endif FUNCTIONS_H