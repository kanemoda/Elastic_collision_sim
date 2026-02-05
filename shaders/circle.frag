#version 330 core
in vec2 vLocal;
out vec4 FragColor;

uniform vec4 uColor;

void main()
{
    if (length(vLocal) > 1.0)
        discard;
    FragColor = uColor;
}
