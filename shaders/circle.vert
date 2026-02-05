#version 330 core
layout (location = 0) in vec2 aPos;

uniform vec2 uCenter;
uniform float uRadius;
uniform mat4 uProj;

out vec2 vLocal;

void main()
{
    vec2 world = uCenter + aPos * uRadius;
    vLocal = aPos;
    gl_Position = uProj * vec4(world, 0.0, 1.0);
}
