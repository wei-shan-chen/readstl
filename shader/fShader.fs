#version 330 core
out vec4 FragColor;

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

uniform vec3 cubeColor;

void main()
{
    
    FragColor = vec4(cubeColor, 1.0);
	
} 
