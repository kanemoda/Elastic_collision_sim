#include "grid.h"
#include "config.h"
#include <stdlib.h>

static GridCell *grid = NULL;
static int grid_width = 0;
static int grid_height = 0;

void Grid_Init(void)
{
    grid_width  = WORLD_WIDTH  / CELL_SIZE + 1;
    grid_height = WORLD_HEIGHT / CELL_SIZE + 1;

    grid = malloc(sizeof(GridCell) * grid_width * grid_height);
    Grid_Clear();
}

void Grid_Clear(void)
{
    int total = grid_width * grid_height;
    for (int i = 0; i < total; i++) {
        grid[i].count = 0;
    }
}

static int Grid_Index(int x, int y)
{
    return y * grid_width + x;
}

void Grid_InsertParticle(int particle_index)
{
    Particle *p = &particles[particle_index];

    int cx = (int)(p->x / CELL_SIZE);
    int cy = (int)(p->y / CELL_SIZE);

    if (cx < 0 || cy < 0 || cx >= grid_width || cy >= grid_height)
        return;

    GridCell *cell = &grid[Grid_Index(cx, cy)];

    if (cell->count < MAX_PARTICLES_PER_CELL) {
        cell->indices[cell->count++] = particle_index;
    }
    // dolarsa: sessizce ignore (ileride büyütülebilir)
}

GridCell *Grid_GetCell(int x, int y)
{
    if (x < 0 || y < 0 || x >= grid_width || y >= grid_height)
        return NULL;

    return &grid[Grid_Index(x, y)];
}

int Grid_GetWidth(void)
{
    return grid_width;
}

int Grid_GetHeight(void)
{
    return grid_height;
}
