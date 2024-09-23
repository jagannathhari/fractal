#ifndef _INCLUDE_WORLD_H_
#define _INCLUDE_WORLD_H_

typedef struct {
    char world_name[255];
    float x, y, initital_angle, length, angle;
    int level;
    char *axioms;
    char *productions[256];
} World;

void print_world(World *world);
void load_world(char *file_name, World *world);
#endif // _INCLUDE_WORLD_H_
