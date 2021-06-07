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
    double value;

    inline double operator =(double r){
        value = (r > 0 ? r : 0);
        return value;
    }

    inline void operator +=(double r){
        value += r;
    }
    inline void operator -=(double r){
        value = (value - r > 0 ? value - r : 0);
    }
    inline void operator *=(double r){
        value *= r;
    }
    inline void operator /=(double r){
        value /= r;
    }
    inline bool operator ==(double r){
        return value == r;
    }
    inline bool operator !=(double r){
        return value != r;
    }
};

struct Angle{
    double value;

    void operator +=(double deg){
        value = ANGLE_UPPER_LIMIT(value + deg);
    }
    void operator -=(double deg){
        value = ANGLE_LOWER_LIMIT(value - deg);
    }
    void operator *=(double deg){
        value = ANGLE_UPPER_LIMIT(value * deg);
    }
    void operator /=(double deg){
        value /= deg;
    }
    inline bool operator ==(double deg){
        return value == deg;
    }
    inline bool operator !=(double deg){
        return value != deg;
    }
};

struct SphericalCoords{
    Radius r;
    Angle  phi;
    Angle  theta;

    POINT3 to_cartesian();
};

struct TaitBryanAngles{
    Angle x;
    Angle y;
    Angle z;
};

struct Camera{
    SphericalCoords center_position;
    TaitBryanAngles orientation;
    POINT offsets;
    double dc = 1000;//distance from screen to camera centre

    void log(FILE* stream){
        fprintf(stream,"Camera phi angle:\t\t%f\n",center_position.phi.value);
        fprintf(stream,"Camera theta angle:\t\t%f\n",center_position.theta.value);
        fprintf(stream,"Camera distance to center:\t%f\n",center_position.r.value);
        TaitBryanAngles C = orientation;
        fprintf(stream,"Camera orientation:\t\t%f,%f,%f\n",C.x,C.y,C.z);
        fprintf(stream,"Distance from camera to screen:\t%f\n\n",dc);
    }
};

struct Face{
    uint32_t* order;
    uint32_t vertices_count;
};

class Object{
private:
    POINT3* world_points;
    POINT* screen_points;
    Face* faces;
    uint32_t edges_count;
    uint32_t vertices_count;
public:
    Object(){
        world_points = nullptr;
        screen_points = nullptr;
        faces = nullptr;
        edges_count = 0;
        vertices_count = 0;
    }
    Object(POINT3* world_points, POINT* screen_points, Face* edges, uint32_t vertices_count, uint32_t edges_count){
        this->world_points = world_points;
        this->screen_points = screen_points;
        this->faces = edges;
        this->edges_count = edges_count;
        this->vertices_count = vertices_count;
    }
    void update(Camera& cam, int w_height, int w_width);
    void draw(HDC hdc,COLORREF color);
};

void draw_ground(HDC hdc,Camera& cam,int w_height, int w_width);
void draw_coordinate_lines(HDC hdc, Camera& cam, int w_height, int w_width);

Object* parse_obj(arena_t* arena,const char* fname);

#endif FUNCTIONS_H