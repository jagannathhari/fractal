#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "./world.h"
#include "raylib.h"

#define rad(degrees) (PI / 180) * (degrees)
#define SCALE 100
#define WIDTH 16*SCALE
#define HEIGHT 9*SCALE
#define MAX_LEVEL 20
#define MIN_LEVEL 0

#define max(a, b) ((a) > (b)) ? (a) : (b)
#define min(a, b) ((a) < (b)) ? (a) : (b)

typedef struct {
    float x;
    float y;
    float angle;
} State;

float length = 10;
float angle = 0;
char **productions = NULL;

void process_char(char p, State *s) {
    if(p>='a' && p<='z'){
        s->x += length * cos(s->angle);
        s->y += length * sin(s->angle);
        return;
    }
    if(p>='A' && p<='Z'){
        DrawLineEx(
            (Vector2){s->x, s->y},
            (Vector2){s->x + length * cos(s->angle), s->y + length * sin(s->angle)}, 3,
            RED);
        s->x += length * cos(s->angle);
        s->y += length * sin(s->angle);
        return;
    }
    switch (p) {
    case '-':
        s->angle -= angle;
        break;
    case '+':
        s->angle += angle;
        break;
    }
}

void process_string(char *p, State *s) {
    while (*p) {
        process_char(*p++, s);
    }
}

void expand_production(char *p, int level, State *s) {
    if (!level) {
        process_string(p, s);
        return;
    }
    for (int i = 0; p[i]; i++) {
        if (productions[p[i]]) {
            expand_production(productions[p[i]], level - 1, s);
        } else {
            process_char(p[i], s);
        }
    }
}

int main(int argc, char **argv) {
    if(argc != 2){
        printf("Uses fractal <fractal file (.frc)>\n");
        return 0;
    }
    World world;
    load_world(argv[1], &world);
    angle = rad(world.angle);
    length = world.length;
    productions = world.productions;
    InitWindow(WIDTH, HEIGHT, world.world_name);
    State state = {world.x, world.y, rad(world.initital_angle)};

    Camera2D camera = {0};
    camera.target = (Vector2){world.x + 20.0f, world.y + 20.0f};
    camera.offset = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Vector2 test = (Vector2){100, 100};

    Vector2 previousMousePosition = {0, 0};
    bool isPanning = false;
    while (!WindowShouldClose()) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePosition = GetMousePosition();
            if (!isPanning) {
                previousMousePosition = mousePosition; 
                isPanning = true;
            }

            Vector2 mouseDelta = (Vector2){mousePosition.x - previousMousePosition.x, mousePosition.y - previousMousePosition.y};
            camera.target.x -= mouseDelta.x / camera.zoom; 
            camera.target.y -= mouseDelta.y / camera.zoom;
            previousMousePosition = mousePosition; 

        } else {
            isPanning = false;
        }

        camera.zoom += ((float)GetMouseWheelMove() * 0.05f);

        if (IsKeyPressed(KEY_R)) {
            camera.zoom = 1.0f;
            camera.rotation = 0.0f;
            camera.target = (Vector2){world.x + 20.0f, world.y + 20.0f}; // Reset to initial position
        }

        if(IsKeyReleased(KEY_J)){
            world.level = max(MIN_LEVEL,world.level-1);
        }

        if(IsKeyReleased(KEY_K)){
            world.level = min(MAX_LEVEL,world.level+1);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);

            expand_production(world.axioms, world.level, &state);
            state = (State){world.x, world.y, rad(world.initital_angle)};
        

        EndMode2D();

        EndDrawing();
    }
    CloseWindow();
    free(world.axioms);
    for(int i = 0; i< 256 ; i++){
        if(world.productions[i]){
            free(world.productions[i]);
        }
    }
    // todo fix segmentaion fault
    // todo handle error 
    return 0;
}
