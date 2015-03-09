#version 130	

uniform float time;
uniform sampler2D tex;

in vec2 uv;

void main()  
{  
    gl_FragColor = texture(tex, vec2(uv.x, 1.0 - uv.y)); //vec4(uv.x, uv.y, 0.0, 1.0);  
}

