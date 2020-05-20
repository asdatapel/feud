#version 150 

in vec4 vertex;

out vec2 texCoord;

uniform mat4 Projection; 

void main()
{
    gl_Position = Projection * vec4(vertex.xy, 0.0, 1.0);
    texCoord = vertex.zw;
}
