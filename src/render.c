#include "render.h"
#include "particle.h"
#include "raylib.h"

void Render_DrawParticles(void)
{
    for (int i = 0; i < particle_count; i++) {
        DrawCircle(
            particles[i].x,
            particles[i].y,
            particles[i].r,
            particles[i].color
        );
    }
}
