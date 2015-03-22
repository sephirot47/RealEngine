#version 130	

uniform float time;
uniform float width, height;
uniform sampler2D colors, pos, uvs, normals, depth;

in vec2 screenuv;

void main()
{  
    gl_FragColor = vec4(texture(colors, screenuv).xyz, 1.0);
}

