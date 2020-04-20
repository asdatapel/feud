#version 150 

in vec2 texcoord; 
in vec3 position; 
in vec3 incolor; 

out vec3 Color; 
out vec2 texCoord; 

uniform mat4 PVM; 

void main()
{
    gl_Position = PVM * vec4(position, 1.0);
    Color = incolor; 
    texCoord = texcoord;
}
