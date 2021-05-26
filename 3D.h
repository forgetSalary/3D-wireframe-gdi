#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "framework.h"
#include "arena.h"

#define PI ((double)3.14159265359)

struct POINT3
{
    int x;
    int y;
    int z;
};

inline POINT Point(int x, int y){
    return { x,y };
}

inline POINT3 PointEx(int x, int y, int z) {
    return { x,y,z };
}

inline int Round(double x){
    if (x < 0) x -= 0.5; else x += 0.5;
    return (int)x;
}

struct ViewPoint{
    double R; //Расстояние от точки наблюдения до начала мировой системы координат
    double A;
    double B;
    double d; //Расстояние от точки наблюдения до экранной плоскости
};

struct Edge{
    uint32_t* order;
    uint32_t vertices_count;
};

class Object{
    HANDLE _heap;
    POINT3* world_points;
    POINT* screen_points;
    Edge* edges;
    uint32_t edges_count;
    uint32_t vertices_count;
public:
    Object(){
        _heap = NULL;
        world_points = nullptr;
        screen_points = nullptr;
        edges = nullptr;
        edges_count = 0;
        vertices_count = 0;
    }
    Object(HANDLE heap,POINT3* world_points,POINT* screen_points, Edge* edges,uint32_t vertices_count, uint32_t edges_count){
        this->_heap = heap;
        this->world_points = world_points;
        this->screen_points = screen_points;
        this->edges = edges;
        this->edges_count = edges_count;
        this->vertices_count = vertices_count;
    }
    void update(ViewPoint& vp, int w_height, int w_width);
    void draw(HDC hdc);
    HANDLE heap(){return _heap;}
    void set_heap(HANDLE heap){_heap = heap;}
};

POINT3 GlobalPointToScreen(POINT3 World_Point, double Angle_Horizontal, double Angle_Vertical, int R);
POINT Perspective(POINT3 PT, double d);
POINT ToScreenPoint(POINT PT, int w_height, int w_width);

void CreatePyramid(POINT3* WPT,
                   Edge* edges,
                   POINT3 surface_centre,
                   int height,
                   int edge_len,
                   int edges_count);

void DrawPiramid(HDC hdc, POINT* PT, int COUNT);

Object* parse_obj(FILE* file);

#endif FUNCTIONS_H