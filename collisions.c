#include "raylib.h"

#define WIDTH 900
#define HEIGHT 600
#define FPS 60

#define NUM_PARTICLES 15

typedef struct
{
    float x,y,r;
    float vx,vy;
} Particle;

void DrawParticle(const Particle *p)
{
    DrawCircle(p->x,p->y,p->r,WHITE);
}

void UpdateParticle(Particle *p, float dt)
{
    p->x += p->vx * dt;
    p->y += p->vy * dt;

    //Horizontal Collision With Wall
    if (p->x <= p->r) 
    {
        p->x = p->r;
        p->vx *= -1;
    }
    else if (p->x >= WIDTH - p->r) 
    {
        p->x = WIDTH - p->r;
        p->vx *= -1;
    }

    //Vertical Collision With Wall
    if (p->y <= p->r) 
    {
        p->y = p->r;
        p->vy *= -1;
    }
    else if (p->y >= HEIGHT - p->r) 
    {
        p->y = HEIGHT - p->r;
        p->vy *= -1;
    }
    
}

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "Elastic Collisions");
    SetTargetFPS(FPS);
    Particle particle = {300,300,20,200,200};
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        BeginDrawing();
            DrawFPS(5,5);
            ClearBackground(BLACK);
            UpdateParticle(&particle, dt);
            DrawParticle(&particle);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}