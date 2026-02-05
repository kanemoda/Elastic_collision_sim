#ifndef RENDER_H
#define RENDER_H

void Render_Init(void);
void Render_Begin(void);
void Render_DrawParticles(void);
void Render_End(void);
void Render_Shutdown(void);

int  Render_ShouldClose(void);
void Render_HandleInput(float dt);

#endif
