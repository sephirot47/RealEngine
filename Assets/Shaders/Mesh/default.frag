#version 130	

uniform sampler2D tex;

in vec3 fpos;
in vec2 fuv;
in vec3 fnormal;

out vec4 outFinalColors;
out vec4 outTextureColors;
out vec3 outpos;
out vec2 outuv;
out vec3 outnormal;

void main()
{  
    outTextureColors = vec4(texture(tex, vec2(fuv.x, 1.0 - fuv.y)).rgb, 1.0);
    outFinalColors = vec4(texture(tex, vec2(fuv.x, 1.0 - fuv.y)).rgb * 0.05, 1.0);
    outpos = fpos;
    outuv = fuv;
    outnormal = fnormal;
}

