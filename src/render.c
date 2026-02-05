#include "render.h"
#include "particle.h"
#include "config.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

/* ================= GLOBALS ================= */

static GLFWwindow* window;
static unsigned int shader;
static unsigned int vao, vbo;
static int uCenter, uRadius, uColor, uProj;

/* -------- FPS counter -------- */
static double fps_last_time = 0.0;
static int fps_frames = 0;
static int current_fps = 0;

/* -------- Camera (Raylib Camera2D equivalent) -------- */
static float cam_x = WORLD_WIDTH  * 0.5f;
static float cam_y = WORLD_HEIGHT * 0.5f;
static float cam_zoom = 0.1f;

/* ================= UTILS ================= */

static char* load_file(const char* path)
{
    FILE* f = fopen(path, "rb");
    if (!f) {
        printf("Cannot open %s\n", path);
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);

    char* data = malloc(len + 1);
    size_t read = fread(data, 1, len, f);
    if (read != (size_t)len) {
        printf("File read error: %s\n", path);
        exit(1);
    }

    data[len] = 0;
    fclose(f);
    return data;
}

static unsigned int build_shader(const char* vsPath, const char* fsPath)
{
    char* vsSrc = load_file(vsPath);
    char* fsSrc = load_file(fsPath);

    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, (const char**)&vsSrc, NULL);
    glCompileShader(vs);

    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, (const char**)&fsSrc, NULL);
    glCompileShader(fs);

    unsigned int prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);

    glDeleteShader(vs);
    glDeleteShader(fs);
    free(vsSrc);
    free(fsSrc);

    return prog;
}

/* ================= INIT ================= */

void Render_Init(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(
        SCREEN_WIDTH, SCREEN_HEIGHT,
        "Elastic Collision",
        NULL, NULL
    );

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to init GLAD\n");
        exit(1);
    }

    glfwSwapInterval(0); // vsync off

    shader = build_shader(
        "shaders/circle.vert",
        "shaders/circle.frag"
    );

    float quad[] = {
        -1,-1,  1,-1,  1, 1,
        -1,-1,  1, 1, -1, 1
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glUseProgram(shader);

    uCenter = glGetUniformLocation(shader, "uCenter");
    uRadius = glGetUniformLocation(shader, "uRadius");
    uColor  = glGetUniformLocation(shader, "uColor");
    uProj   = glGetUniformLocation(shader, "uProj");
}

/* ================= INPUT ================= */

void Render_HandleInput(float dt)
{
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cam_zoom *= 1.01f;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cam_zoom *= 0.99f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam_y -= 500 * dt;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam_y += 500 * dt;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam_x -= 500 * dt;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam_x += 500 * dt;

    if (cam_zoom < 0.02f) cam_zoom = 0.02f;
    if (cam_zoom > 5.0f)  cam_zoom = 5.0f;
}

/* ================= FRAME ================= */

void Render_Begin(void)
{
    glfwPollEvents();
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    /* --- CORRECT camera math --- */
    float half_w = (SCREEN_WIDTH  * 0.5f) / cam_zoom;
    float half_h = (SCREEN_HEIGHT * 0.5f) / cam_zoom;

    float l = cam_x - half_w;
    float r = cam_x + half_w;
    float b = cam_y + half_h;
    float t = cam_y - half_h;

    float proj[16] = {
        2/(r-l), 0, 0, 0,
        0, 2/(t-b), 0, 0,
        0, 0, -1, 0,
        -(r+l)/(r-l), -(t+b)/(t-b), 0, 1
    };

    glUseProgram(shader);
    glUniformMatrix4fv(uProj, 1, GL_FALSE, proj);
}

void Render_DrawParticles(void)
{
    glBindVertexArray(vao);

    for (int i = 0; i < particle_count; i++) {
        Particle* p = &particles[i];

        glUniform2f(uCenter, p->x, p->y);
        glUniform1f(uRadius, p->r);
        glUniform4f(
            uColor,
            p->color.r / 255.0f,
            p->color.g / 255.0f,
            p->color.b / 255.0f,
            1.0f
        );

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void Render_End(void)
{
    glfwSwapBuffers(window);

    double now = glfwGetTime();
    fps_frames++;

    if (fps_last_time == 0.0)
        fps_last_time = now;

    double delta = now - fps_last_time;
    if (delta >= 0.5) {  // 0.5 saniyede bir güncelle
        current_fps = (int)(fps_frames / delta);
        fps_frames = 0;
        fps_last_time = now;

        char title[128];
        snprintf(title, sizeof(title),
                 "Elastic Collision | FPS: %d", current_fps);
        glfwSetWindowTitle(window, title);
    }
}


/* ================= SHUTDOWN ================= */

int Render_ShouldClose(void)
{
    return glfwWindowShouldClose(window);
}

void Render_Shutdown(void)
{
    glfwTerminate();
}
