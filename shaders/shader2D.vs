#version 150 

in vec2 texcoord; 
in vec2 position; 

out vec2 texCoord; 

uniform mat4 transform; 

void main()
{
    gl_Position = transform * vec4(position.xy, 0.0, 1.0);
    texCoord = texcoord;
}
