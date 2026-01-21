#ifndef GRID_H
#define GRID_H

#include "particle.h"
#include "config.h"

// --- GRID CONFIG ---


typedef struct {
    int count;
    int indices[MAX_PARTICLES_PER_CELL];
} GridCell;

void Grid_Init(void);
void Grid_Clear(void);
void Grid_InsertParticle(int particle_index);
GridCell *Grid_GetCell(int x, int y);

int Grid_GetWidth(void);
int Grid_GetHeight(void);

#endif
