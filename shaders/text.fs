#version 150

in vec2 texCoord;

out vec4 outColor;

uniform sampler2D tex;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(tex, texCoord).r);
    outColor = sampled ;
}  