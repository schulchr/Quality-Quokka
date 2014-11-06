#version 330 core

in vec4 position;
smooth out vec2 texCoord;
void main()
{
    texCoord = position.zw;
    gl_Position = vec4(position.x, position.y, 0.0, 1.0);
}
