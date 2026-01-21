#include "render.h"
#include "particle.h"
#include "raylib.h"
#include "config.h"

static Camera2D camera;

void Render_Init(void)
{
    camera.target = (Vector2){
        WORLD_WIDTH / 2.0f,
        WORLD_HEIGHT / 2.0f
    };

    camera.offset = (Vector2){
        SCREEN_WIDTH / 2.0f,
        SCREEN_HEIGHT / 2.0f
    };

    camera.rotation = 0.0f;
    camera.zoom = 0.1f;   
}

Camera2D *Render_GetCamera(void)
{
    return &camera;
}

void Render_Begin(void)
{
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(camera);
}

void Render_End(void)
{
    EndMode2D();
    DrawFPS(10, 10);
    EndDrawing();
}

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
