#version 150

in vec2 texCoord; 
in vec3 Color; 

out vec4 outColor;

uniform sampler2D tex; 

void main()
{
    outColor = vec4(texCoord, (texCoord.x + texCoord.y) /2, 1) * 0.5 + texture(tex, texCoord) * 0.5 ;
}

