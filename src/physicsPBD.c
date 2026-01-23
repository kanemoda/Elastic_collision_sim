#include "physics.h"
#include "particle.h"
#include "config.h"
#include "grid.h"
#include <math.h>
#include <stdio.h>


void Physics_HandleParticleCollisionsGrid_PBD(void)
{
    /* -------- Build grid -------- */
    Grid_Clear();
    for (int i = 0; i < particle_count; i++) {
        Grid_InsertParticle(i);
    }

    /* -------- PBD collision solve -------- */
    for (int i = 0; i < particle_count; i++) {

        Particle *p1 = &particles[i];
        int cx = (int)(p1->x / CELL_SIZE);
        int cy = (int)(p1->y / CELL_SIZE);

        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {

                GridCell *cell = Grid_GetCell(cx + dx, cy + dy);
                if (!cell) continue;

                for (int k = 0; k < cell->count; k++) {

                    int j = cell->indices[k];
                    if (j <= i) continue;

                    g_pair_checks++;

                    Particle *p2 = &particles[j];

                    /* ---- Relative position ---- */
                    float dxp = p1->x - p2->x;
                    float dyp = p1->y - p2->y;

                    float dist2 = dxp * dxp + dyp * dyp;
                    float minDist = p1->r + p2->r;
                    float minDist2 = minDist * minDist;

                    /* ---- No overlap → no constraint ---- */
                    if (dist2 >= minDist2)
                        continue;

                    g_overlap_pass++;

                    /* ---- Constraint solve ---- */
                    float dist = sqrtf(dist2);
                    if (dist == 0.0f)
                        continue;

                    g_sqrt_calls++;

                    float nx = dxp / dist;
                    float ny = dyp / dist;

                    float overlap = minDist - dist;

                    /* Compliance / softness */
                    const float stiffness = 0.9f;   // [0,1]
                    overlap *= stiffness;

                    /* Mass weights (radius^2 ≈ mass) */
                    float m1 = p1->r * p1->r;
                    float m2 = p2->r * p2->r;
                    float invMassSum = (1.0f / m1) + (1.0f / m2);

                    float corr1 = (1.0f / m1) / invMassSum;
                    float corr2 = (1.0f / m2) / invMassSum;

                    /* ---- Position correction ONLY ---- */
                    p1->x += nx * overlap * corr1;
                    p1->y += ny * overlap * corr1;
                    p2->x -= nx * overlap * corr2;
                    p2->y -= ny * overlap * corr2;

                    g_impulses_applied++; /* now = constraint solves */
                }
            }
        }
    }

    /* -------- Debug print -------- */
    static int frame = 0;
    frame++;

    if (frame % 60 == 0) {
        printf(
            "[PBD ] pairs=%lld overlap=%lld sqrt=%lld constraints=%lld\n",
            g_pair_checks,
            g_overlap_pass,
            g_sqrt_calls,
            g_impulses_applied
        );

        g_pair_checks = 0;
        g_overlap_pass = 0;
        g_sqrt_calls = 0;
        g_impulses_applied = 0;
    }
}