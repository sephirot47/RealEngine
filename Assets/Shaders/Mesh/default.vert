#version 130

uniform mat4 projection, view, model, normalMatrix;
 
in vec3 inpos;
in vec2 inuv;
in vec3 innormal;

out vec4 fpos;
out vec2 fuv;
out vec3 fnormal;

void main()  
{    
    fpos = projection * view * model * vec4(inpos, 1.0);
    fuv = inuv;    
    fnormal = normalize((normalMatrix * vec4(innormal, 0.0)).xyz);

    gl_Position = projection * view * model * vec4(inpos, 1.0);
}

