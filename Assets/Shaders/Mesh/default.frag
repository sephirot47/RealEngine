#version 130	

uniform sampler2D tex;

in vec3 fpos;
in vec2 fuv;
in vec3 fnormal;

out vec4 outcolor;
out vec3 outpos;
out vec2 outuv;
out vec3 outnormal;

void main()  
{  
    outcolor = texture(tex, vec2(fuv.x, 1.0 - fuv.y)) * 0.1;
    outpos = fpos;
    outuv = fuv;
    outnormal = fnormal;
}

