#include "raylib.h"
#include "world.h"
#include "render.h"

#define WIDTH 1600
#define HEIGHT 900
#define FPS 60
#define NUM_PARTICLES 2000

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "Elastic Particle Simulation");
    SetTargetFPS(FPS);

    World_Init(NUM_PARTICLES);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        World_Update(dt);

        BeginDrawing();
        ClearBackground(BLACK);
        Render_DrawParticles();
        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
