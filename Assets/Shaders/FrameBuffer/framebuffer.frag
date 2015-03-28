#version 130	

uniform sampler2D finalColors, pos, uvs, normals, textureColors, depth;
in vec2 screenuv;

void main()
{ 
    gl_FragColor = vec4(texture(finalColors, screenuv).rgb, 1);
}

