#version 130	

uniform samplerCube cubemap;
 
in vec3 fpos;

out vec4 outGColor;

void main()
{  
    float outline = 0.98;
/*
    if((abs(fpos.x) > outline && abs(fpos.y) > outline) || 
       (abs(fpos.x) > outline && abs(fpos.z) > outline) ||
       (abs(fpos.y) > outline && abs(fpos.z) > outline)) 
	 outGColor = vec4(0, 0, 0, 1);

    else */outGColor = vec4(texture(cubemap, fpos).rgb, 1);
}

