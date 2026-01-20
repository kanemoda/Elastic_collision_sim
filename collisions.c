#include "raylib.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define WIDTH 900
#define HEIGHT 600
#define FPS 60

#define NUM_PARTICLES 35

typedef struct
{
    float x, y, r;
    float vx, vy;
    Color color;
} Particle;

Particle particles[NUM_PARTICLES];

void DrawParticle(const Particle *p)
{
    DrawCircle(p->x, p->y, p->r, p->color);
}

void UpdateParticle(Particle *p, float dt)
{
    p->x += p->vx * dt;
    p->y += p->vy * dt;

    // Horizontal Collision With Wall
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

    // Vertical Collision With Wall
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

    // Check Collision of p to p
}

void DrawParticles()
{
    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        DrawParticle(particles + i);
    }
}

void UpdateParticles(float dt)
{
    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        UpdateParticle(particles + i, dt);
    }
}

Color RandomColor(void)
{
    return (Color){
        .r = GetRandomValue(50, 255),
        .g = GetRandomValue(50, 255),
        .b = GetRandomValue(50, 255),
        .a = 255};
}

void InitParticles()
{

    SetRandomSeed(time(NULL));

    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        Particle particle = particles[i];
        particles[i].r = GetRandomValue(5, 15);
        particles[i].x = GetRandomValue(particles[i].r, WIDTH - particles[i].r);
        particles[i].y = GetRandomValue(particles[i].r, HEIGHT - particles[i].r);
        particles[i].vx = GetRandomValue(-200, 200);
        particles[i].vy = GetRandomValue(-200, 200);
        particles[i].color = RandomColor();
    }
}

void collideAllParticles()
{
    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        for (int j = i + 1; j < NUM_PARTICLES; j++)
        {

            Particle *p1 = &particles[i];
            Particle *p2 = &particles[j];

            Vector2 c1 = {p1->x, p1->y};
            Vector2 c2 = {p2->x, p2->y};
            bool collide = CheckCollisionCircles(c1, p1->r, c2, p2->r);
            if (collide)
            {
                // Move apart two colliding particles
                float dx = (p1->x - p2->x);
                float dy = (p1->y - p2->y);

                float abs_d = sqrt(dx * dx + dy * dy);

                float nx, ny;
                if (abs_d > 0.00f)
                {
                    nx = dx / abs_d;
                    ny = dy / abs_d;
                }
                else
                {
                    nx = 1.00f;
                    ny = 0.00f;
                }

                float overlaping = (p1->r + p2->r) - abs_d;

                float w1 = p2->r / (p1->r + p2->r);
                float w2 = p1->r / (p1->r + p2->r);

                p1->x += nx * overlaping * w1;
                p1->y += ny * overlaping * w1;
                p2->x -= nx * overlaping * w2;
                p2->y -= ny * overlaping * w2;
            }
        }
    }
}

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "Elastic Collisions");
    InitParticles();
    SetTargetFPS(FPS);

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        BeginDrawing();
        ClearBackground(BLACK);
        DrawFPS(5, 5);
        DrawParticles();
        EndDrawing();

        UpdateParticles(dt);
        collideAllParticles();
    }

    CloseWindow();

    return 0;
}