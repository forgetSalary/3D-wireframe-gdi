#include "stdio.h"
#include "string"

//tokens:
//zoom '+-DOUBLE'
//rotate left,right,up,down '+-DOUBLE'
//move x,y '+-DOUBLE'

#define TOKEN_ZOOM "zoom"
#define TOKEN_ROTATE "rotate"
#define TOKEN_R_LEFT "left"
#define TOKEN_R_RIGHT "right"
#define TOKEN_R_UP "up"
#define TOKEN_R_DOWN "down"
#define TOKEN_MOVE "move"

enum rotate_dir{
    phi,
    theta
};

enum command_kind{
    rotate,
    move,
    zoom
};

struct command{
    command_kind kind;
    union{
        struct{
            double angle;
            rotate_dir dir;
        }rotate;
        double zoom;
    };
};

void read_command(command* cm_struct,const char* command){
    std::string format;
    format = command;

    int token_index = format.find(TOKEN_ROTATE,0);
    if (token_index != std::string::npos){
        cm_struct->kind = rotate;

        int rotate_dir = std::string::npos;
        rotate_dir = format.find(TOKEN_R_RIGHT,token_index);
        if(rotate_dir != std::string::npos){
            cm_struct->rotate.dir = phi;
        }
        rotate_dir = format.find(TOKEN_R_LEFT,token_index);
        if(rotate_dir != std::string::npos){
            cm_struct->rotate.dir = phi;
        }
        rotate_dir = format.find(TOKEN_R_DOWN,token_index);
        if(rotate_dir != std::string::npos){
            cm_struct->rotate.dir = theta;
        }rotate_dir = format.find(TOKEN_R_UP,token_index);
        if(rotate_dir != std::string::npos){
            cm_struct->rotate.dir = theta;
        }

        printf("Unknown command\n");
    }
    token_index = format.find(TOKEN_ZOOM,0);
    if (token_index != std::string::npos){
        cm_struct->kind = zoom;

        int rotate_dir = std::string::npos;
        rotate_dir = format.find(,token_index);
        if(rotate_dir != std::string::npos){
            cm_struct->rotate.dir = phi;
        }
        rotate_dir = format.find(TOKEN_R_LEFT,token_index);
        if(rotate_dir != std::string::npos){
            cm_struct->rotate.dir = phi;
        }
        rotate_dir = format.find(TOKEN_R_DOWN,token_index);
        if(rotate_dir != std::string::npos){
            cm_struct->rotate.dir = theta;
        }rotate_dir = format.find(TOKEN_R_UP,token_index);
        if(rotate_dir != std::string::npos){
            cm_struct->rotate.dir = theta;
        }

        printf("Unknown command\n");
    }


}