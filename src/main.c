#include "config.h"
#include "world.h"
#include "render.h"

#include <GLFW/glfw3.h>

int main(void)
{
    Render_Init();
    World_Init(NUM_PARTICLES);

    double last = glfwGetTime();

    while (!Render_ShouldClose()) {
        double now = glfwGetTime();
        float dt = (float)(now - last);
        last = now;

        Render_HandleInput(dt);
        World_Update(dt);

        Render_Begin();
        Render_DrawParticles();
        Render_End();
    }

    Render_Shutdown();
    return 0;
}
