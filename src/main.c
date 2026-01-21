#include "raylib.h"
#include "world.h"
#include "config.h"
#include "render.h"


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
