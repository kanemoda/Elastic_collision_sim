#include "raylib.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define WIDTH 1600
#define HEIGHT 900
#define FPS 60

#define NUM_PARTICLES 2000
#define MAX_TRIES 1000

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

bool doesOverlaps(float x, float y, float r, int count)
{
    for (int i = 0; i < count; i++)
    {
        float dx = x - particles[i].x;
        float dy = y - particles[i].y;
        float minDist = r + particles[i].r;

        if (dx*dx + dy*dy < minDist * minDist)
        {
            return true;
        }
        
    }
    return false;
    
}

void InitParticles()
{

    SetRandomSeed(time(NULL));

    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        Particle *p = &particles[i];
        p->r = GetRandomValue(5, 15);

        int tries = 0;
        do
        {
            p->x = GetRandomValue(p->r, WIDTH - p->r);
            p->y = GetRandomValue(p->r, HEIGHT - p->r);
            tries++;
        } while (doesOverlaps(p->x, p->y, p->r, i) && tries < MAX_TRIES);

        if (tries == MAX_TRIES)
        {
            p->x = GetRandomValue(p->r, WIDTH - p->r);
            p->y = GetRandomValue(p->r, HEIGHT - p->r);
        }
        
        

        
        p->vx = GetRandomValue(-200, 200);
        p->vy = GetRandomValue(-200, 200);
        p->color = RandomColor();
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

            //Vector2 c1 = {p1->x, p1->y};
            //Vector2 c2 = {p2->x, p2->y};
            float dx = (p1->x - p2->x);
            float dy = (p1->y - p2->y);
            
            float dist = dx*dx + dy*dy;
            float minDist = p1->r + p2->r;

            
            if (dist < minDist * minDist)
            {
                // Move apart two colliding particles
                

                float abs_d = sqrt(dist);

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

                float overlaping = minDist - abs_d;

                float w1 = p2->r / (p1->r + p2->r);
                float w2 = p1->r / (p1->r + p2->r);

                p1->x += nx * overlaping * w1;
                p1->y += ny * overlaping * w1;
                p2->x -= nx * overlaping * w2;
                p2->y -= ny * overlaping * w2;

                //Reflect particles (Hrad physics part god help me)
                float m1 = p1->r * p1->r;
                float m2 = p2->r * p2->r;

                float rvx = p1->vx - p2->vx;
                float rvy = p1->vy - p2->vy;

                float velAlongNormal = rvx * nx + rvy * ny;

                // already separating
                if (velAlongNormal > 0)
                    continue;

                float e = 1.0f; // perfectly elastic

                float j = -(1.0f + e) * velAlongNormal;
                j /= (1.0f / m1 + 1.0f / m2);

                float impulseX = j * nx;
                float impulseY = j * ny;

                p1->vx += impulseX / m1;
                p1->vy += impulseY / m1;

                p2->vx -= impulseX / m2;
                p2->vy -= impulseY / m2;

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
        DrawParticles();
        DrawFPS(5, 5);
        EndDrawing();

        UpdateParticles(dt);
        collideAllParticles();
    }

    CloseWindow();

    return 0;
}