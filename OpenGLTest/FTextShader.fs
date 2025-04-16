#version 330 core
out vec4 FragColor;  

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D texture1;

void main()
{
    FragColor = vec4(1.f,1.f,1.f,1.f);  
}