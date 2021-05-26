#include "3D.h"

const int FormWidth = 200;    //Ширина формы
const int FormHeight = 200;  //Высота формы

#define edges_count 7
POINT3 WPT[edges_count + 1]; //Мировые координаты углов пирамиды
POINT SPT[edges_count + 1];  //Экранные координаты углов пирамиды

uint32_t surface[edges_count+1];
uint32_t pyramid_edges[edges_count][2];
Edge edges[edges_count+1] = {surface, edges_count};

Object main_object(WPT,SPT,edges,edges_count+1,edges_count+1);

void lab5(){
    for (int i = 0; i < edges_count; ++i) {
        edges[i+1] = {pyramid_edges[i],1};
    }

    POINT3 centre = { -100,0,-100};
    CreatePyramid(WPT,edges,centre, FormHeight, FormWidth/2, edges_count);
}