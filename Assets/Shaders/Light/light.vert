#version 130

in vec3 pos;
out vec2 sceneuv;

void main()
{  
    gl_Position = vec4(pos, 1.0);
    sceneuv = (pos.xy + vec2(1.0, 1.0))/2.0;
}

