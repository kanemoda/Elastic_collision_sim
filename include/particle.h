#ifndef PARTICLE_H
#define PARTICLE_H

typedef struct {
    float r, g, b, a;
} Color;

typedef struct {
    float x, y, r;
    float vx, vy;
    Color color;
} Particle;

extern Particle *particles;
extern int particle_count;

#endif
