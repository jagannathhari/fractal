#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IMPLEMENT_VECTOR
#include "./vector.h"

#include "./world.h"

#define rad(degrees) (PI/ 180) * (degrees)
#define ANGLE rad(90)
#define STR_EQL(s1, s2) strcmp(s1, s2) == 0

typedef char String;

static String *read_line(FILE *f) {
    String *buffer = Vector(*buffer);
    char c;
    while ((c = fgetc(f)) != EOF && c != '\n') {
        vector_append(buffer, (char)c);
    }
    if (c == EOF) {
        return NULL;
    }
    vector_append(buffer, '\0');
    return buffer;
}

void *load_world(char *file_name, World *world) {
    FILE *f = fopen(file_name, "r");
    memset(world,0,sizeof(*world));

    if (!f) {
        perror("Error in opening file");
        return NULL;
    }

    char command[20];
    String *line = NULL;
    while ((line = read_line(f))) {
        sscanf(line, "%19s", command);
        if (STR_EQL("initial_state", command)) {
            if (sscanf(line, "initial_state %f %f %f", &world->x,&world->y, &world->initital_angle) != 3) goto error;

        } else if (STR_EQL("length", command)) {
            if (sscanf(line, "length %f", &world->length) != 1) goto error;

        } else if (STR_EQL("angle", command)) {
            if (sscanf(line, "angle %f", &world->angle) != 1) goto error;

        } else if (STR_EQL("name", command)) {
            if (sscanf(line, "name %254s", world->world_name) != 1) goto error;

        } else if (STR_EQL("levels", command)) {
            if (sscanf(line, "levels %d", &world->level) != 1) goto error;

        } else if (STR_EQL("rules", command)) {

            String *axioms = read_line(f);
            char *temp = malloc(vector_length(axioms));
            memcpy(temp,axioms,vector_length(axioms));
            free_vector(axioms);

            world->axioms = temp;

            String *productions;
            while ((productions = read_line(f))) {

                char vertex = '\0';

                if (sscanf(productions, "%c%*c", &vertex) != 1){
                    free_vector(productions);
                    goto error;
                }

                if (vertex) {
                    char *temp = malloc(vector_length(productions));
                    memcpy(temp, &productions[2], vector_length(productions) - 2);
                    world->productions[vertex] = temp;
                    free_vector(productions);
                }
            }
        }else{
            printf("Ivalid command %s found\n",command); 
            goto error;
        }

        free_vector(line);
    }

    return world;
error:
    memset(world,0,sizeof(*world));
    free_vector(line);
    fclose(f);
    return NULL;
}

void print_world(World *world) {
    printf("initial_state: x = %f y = %f angle = %f\n",
           world->x, world->y, world->initital_angle);
    printf("Name: %s\n", world->world_name);
    printf("Levels: %d\n", world->level);
    printf("Angle: %f\n", world->angle);
    printf("Length: %f\n", world->length);
    printf("Axiom: %s\n", world->axioms);
    for (int i = 0; i < 256; i++) {
        if (world->productions[i]!=0) {
            printf("%c -> %s\n", i, world->productions[i]);
        }
    }
}

#if 0

int main() {
    World w;
    load_world("./world.txt", &w);
    print_world(&w);
    return 0;
}
#endif
