#version 130	

uniform float time;
uniform float width, height;
uniform sampler2D finalColors, textureColors, pos, uvs, normals, depth;

in vec2 screenuv;

void main()
{  
    gl_FragColor = vec4(texture(finalColors, screenuv).rgb, 1.0);
}

