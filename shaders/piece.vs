#version 150 

in vec3 inPosition; 
in vec3 inNormal; 
in vec2 inTexCoord1; 
in vec2 inTexCoord2; 

out vec3 normal; 
out vec2 texCoord1; 
out vec2 texCoord2; 

uniform mat4 PVM; 

void main()
{
    gl_Position = PVM * vec4(inPosition, 1.0);
    normal = inNormal; 
    texCoord1 = inTexCoord1;
    texCoord2 = inTexCoord2;
}
