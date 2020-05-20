#version 150

in vec3 normal; 
in vec2 texCoord1; 
in vec2 texCoord2; 

out vec4 outColor;

uniform sampler2D tex1; 
uniform sampler2D tex2; 

void main()
{
    outColor =  texture(tex1, texCoord1) + texture(tex2, texCoord2);
}

