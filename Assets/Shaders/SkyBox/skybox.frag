#version 130	

uniform samplerCube cubemap;
 
in vec3 fpos;

out vec4 outGColor;

void main()
{  
    outGColor = vec4(fpos, 1);
    outGColor = texture(cubemap, fpos);
}

