#include "physics.h"
#include "particle.h"
#include "config.h"
#include "grid.h"
#include <math.h>



void Physics_HandleWallCollisions(void)
{
    for (int i = 0; i < particle_count; i++) {
        Particle *p = &particles[i];

        if (p->x <= p->r) {
            p->x = p->r;
            p->vx *= -1;
        } else if (p->x >= WORLD_WIDTH - p->r) {
            p->x = WORLD_WIDTH - p->r;
            p->vx *= -1;
        }

        if (p->y <= p->r) {
            p->y = p->r;
            p->vy *= -1;
        } else if (p->y >= WORLD_HEIGHT - p->r) {
            p->y = WORLD_HEIGHT - p->r;
            p->vy *= -1;
        }
    }
}

void Physics_HandleParticleCollisionsNaive(void)
{
    for (int i = 0; i < particle_count; i++) {
        for (int j = i + 1; j < particle_count; j++) {

            Particle *p1 = &particles[i];
            Particle *p2 = &particles[j];

            float dx = p1->x - p2->x;
            float dy = p1->y - p2->y;

            float dist2 = dx * dx + dy * dy;
            float minDist = p1->r + p2->r;

            if (dist2 >= minDist * minDist)
                continue;

            float dist = sqrtf(dist2);
            float nx = (dist > 0.0f) ? dx / dist : 1.0f;
            float ny = (dist > 0.0f) ? dy / dist : 0.0f;

            float overlap = minDist - dist;

            float w1 = p2->r / (p1->r + p2->r);
            float w2 = p1->r / (p1->r + p2->r);

            p1->x += nx * overlap * w1;
            p1->y += ny * overlap * w1;
            p2->x -= nx * overlap * w2;
            p2->y -= ny * overlap * w2;

            float m1 = p1->r * p1->r;
            float m2 = p2->r * p2->r;

            float rvx = p1->vx - p2->vx;
            float rvy = p1->vy - p2->vy;

            float velAlongNormal = rvx * nx + rvy * ny;
            if (velAlongNormal > 0)
                continue;

            float e = 1.0f;
            float j = -(1.0f + e) * velAlongNormal;
            j /= (1.0f / m1 + 1.0f / m2);

            float ix = j * nx;
            float iy = j * ny;

            p1->vx += ix / m1;
            p1->vy += iy / m1;
            p2->vx -= ix / m2;
            p2->vy -= iy / m2;
        }
    }
}

void Physics_HandleParticleCollisionsGrid(void)
{
    Grid_Clear();

    
    for (int i = 0; i < particle_count; i++) {
        Grid_InsertParticle(i);
    }
    
    for (int i = 0; i < particle_count; i++) {

        Particle *p1 = &particles[i];

        int cx = (int)(p1->x / CELL_SIZE);
        int cy = (int)(p1->y / CELL_SIZE);

        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {

                int nx = cx + dx;
                int ny = cy + dy;

                GridCell *cell = Grid_GetCell(nx, ny);
                if (!cell) continue;

                for (int k = 0; k < cell->count; k++) {
                    int j = cell->indices[k];
                    if (j <= i) continue;

                    Particle *p2 = &particles[j];

                    float dxp = p1->x - p2->x;
                    float dyp = p1->y - p2->y;

                    float dist2 = dxp * dxp + dyp * dyp;
                    float minDist = p1->r + p2->r;

                    if (dist2 >= minDist * minDist)
                        continue;

                    float dist = sqrtf(dist2);
                    float nxn = (dist > 0.0f) ? dxp / dist : 1.0f;
                    float nyn = (dist > 0.0f) ? dyp / dist : 0.0f;

                    float overlap = minDist - dist;

                    float w1 = p2->r / (p1->r + p2->r);
                    float w2 = p1->r / (p1->r + p2->r);

                    p1->x += nxn * overlap * w1;
                    p1->y += nyn * overlap * w1;
                    p2->x -= nxn * overlap * w2;
                    p2->y -= nyn * overlap * w2;

                    float m1 = p1->r * p1->r;
                    float m2 = p2->r * p2->r;

                    float rvx = p1->vx - p2->vx;
                    float rvy = p1->vy - p2->vy;

                    float velAlongNormal = rvx * nxn + rvy * nyn;
                    if (velAlongNormal > 0)
                        continue;

                    float e = 1.0f;
                    float jimp = -(1.0f + e) * velAlongNormal;
                    jimp /= (1.0f / m1 + 1.0f / m2);

                    float ix = jimp * nxn;
                    float iy = jimp * nyn;

                    p1->vx += ix / m1;
                    p1->vy += iy / m1;
                    p2->vx -= ix / m2;
                    p2->vy -= iy / m2;
                }
            }
        }
    }
}
