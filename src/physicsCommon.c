#include "physics.h"
#include "particle.h"
#include "config.h"
#include "grid.h"
#include <math.h>
#include <stdio.h>


long long g_pair_checks = 0;
long long g_overlap_pass = 0;
long long g_impulse_tests = 0;
long long g_sqrt_calls = 0;
long long g_impulses_applied = 0;


void Physics_HandleWallCollisions(void)
{
    for (int i = 0; i < particle_count; i++) {
        Particle *p = &particles[i];

        if (p->x <= p->r) {
            p->x = p->r;
            p->vx *= -1.0f;
        } else if (p->x >= WORLD_WIDTH - p->r) {
            p->x = WORLD_WIDTH - p->r;
            p->vx *= -1.0f;
        }

        if (p->y <= p->r) {
            p->y = p->r;
            p->vy *= -1.0f;
        } else if (p->y >= WORLD_HEIGHT - p->r) {
            p->y = WORLD_HEIGHT - p->r;
            p->vy *= -1.0f;
        }
    }
}







