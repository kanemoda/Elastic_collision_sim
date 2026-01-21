#include "world.h"
#include "particle.h"
#include "physics.h"
#include "config.h"
#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>



static Color RandomColor(void)
{
    return (Color){
        .r = GetRandomValue(50, 255),
        .g = GetRandomValue(50, 255),
        .b = GetRandomValue(50, 255),
        .a = 255
    };
}

static int DoesOverlap(float x, float y, float r, int count)
{
    for (int i = 0; i < count; i++) {
        float dx = x - particles[i].x;
        float dy = y - particles[i].y;
        float minDist = r + particles[i].r;

        if (dx * dx + dy * dy < minDist * minDist)
            return 1;
    }
    return 0;
}

void World_Init(int count)
{
    SetRandomSeed(time(NULL));

    particle_count = count;
    particles = malloc(sizeof(Particle) * particle_count);

    for (int i = 0; i < particle_count; i++) {
        Particle *p = &particles[i];
        p->r = GetRandomValue(5, 15);

        int tries = 0;
        do {
            p->x = GetRandomValue(p->r, WIDTH - p->r);
            p->y = GetRandomValue(p->r, HEIGHT - p->r);
            tries++;
        } while (DoesOverlap(p->x, p->y, p->r, i) && tries < MAX_TRIES);

        p->vx = GetRandomValue(-200, 200);
        p->vy = GetRandomValue(-200, 200);
        p->color = RandomColor();
    }
}

void World_Update(float dt)
{
    for (int i = 0; i < particle_count; i++) {
        particles[i].x += particles[i].vx * dt;
        particles[i].y += particles[i].vy * dt;
    }

    Physics_HandleWallCollisions();
    Physics_HandleParticleCollisions();
}
