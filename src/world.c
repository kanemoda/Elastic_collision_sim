#include "world.h"
#include "particle.h"
#include "physics.h"
#include "config.h"
#include "grid.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>

/* -------- Utils -------- */

static int RandRange(int min, int max)
{
    return min + rand() % (max - min + 1);
}

static Color RandomColor(void)
{
    return (Color){
        .r = (float)RandRange(50, 255),
        .g = (float)RandRange(50, 255),
        .b = (float)RandRange(50, 255),
        .a = 255.0f
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

/* -------- World -------- */

void World_Init(int count)
{
    srand((unsigned int)time(NULL));

    particle_count = count;
    particles = malloc(sizeof(Particle) * particle_count);

    Grid_Init();

    for (int i = 0; i < particle_count; i++) {
        Particle *p = &particles[i];

        p->r = (float)RandRange(5, 15);

        int tries = 0;
        do {
            p->x = (float)RandRange((int)p->r, WORLD_WIDTH  - (int)p->r);
            p->y = (float)RandRange((int)p->r, WORLD_HEIGHT - (int)p->r);
            tries++;
        } while (DoesOverlap(p->x, p->y, p->r, i) && tries < MAX_TRIES);

        p->vx = (float)RandRange(-200, 200);
        p->vy = (float)RandRange(-200, 200);
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
    Physics_HandleParticleCollisionsGrid();
}
