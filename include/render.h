#include "raylib.h"
#ifndef RENDER_H
#define RENDER_H

void Render_Init(void);
void Render_Begin(void);
void Render_End(void);
void Render_DrawParticles(void);
Camera2D *Render_GetCamera(void);

#endif
