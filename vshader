#version 130

uniform mat4 projection, model;

in vec3 pos;
in vec2 inuv;

out vec2 uv; 

void main()  
{  
    gl_Position = projection * model * vec4(pos, 1.0);
    uv = inuv;
}

