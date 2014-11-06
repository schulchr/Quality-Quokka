#version 330 core

smooth in vec2 texCoord;
out vec4 color;
uniform sampler2D texUnit;

void main()
{
    color = texture(texUnit, texCoord);
    
    //color = vec4(texCoord.x, texCoord.y, 0.0, 1.0);
    
}
