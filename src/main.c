#include "raylib.h"
#include "config.h"
#include "world.h"
#include "render.h"

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Large World Particle Simulation");
    SetTargetFPS(FPS);

    World_Init(NUM_PARTICLES);
    Render_Init();

    Camera2D *cam = Render_GetCamera();

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // -------- Camera controls
        if (IsKeyDown(KEY_Q)) cam->zoom *= 1.01f;
        if (IsKeyDown(KEY_E)) cam->zoom *= 0.99f;

        if (IsKeyDown(KEY_W)) cam->target.y -= 500 * dt;
        if (IsKeyDown(KEY_S)) cam->target.y += 500 * dt;
        if (IsKeyDown(KEY_A)) cam->target.x -= 500 * dt;
        if (IsKeyDown(KEY_D)) cam->target.x += 500 * dt;

        // Clamp zoom
        if (cam->zoom < 0.02f) cam->zoom = 0.02f;
        if (cam->zoom > 5.0f)  cam->zoom = 5.0f;

        World_Update(dt);

        Render_Begin();
        Render_DrawParticles();
        Render_End();
    }

    CloseWindow();
    return 0;
}
