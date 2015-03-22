#version 130

in vec3 pos;
out vec2 screenuv; 

void main()
{  
    gl_Position = vec4(pos, 1.0);
    screenuv = (pos.xy + vec2(1.0, 1.0))/2.0;
}

