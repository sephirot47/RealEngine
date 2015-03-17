#version 130	

uniform float time;
uniform sampler2D tex;

in vec3 fpos;
in vec2 fuv;
in vec3 fnormal;

vec3 lightPos = vec3(100*sin(time*3.0), -50, 100*cos(time*3.0));
vec3 lightDir = (0.0, -0.3, -1.5) - lightPos;

out vec4 outcolor;
out vec3 outpos;
out vec2 outuv;
out vec3 outnormal;

float discretisa(float light)
{
	if(light < 0.2) return 0.2;
	if(light < 0.6) return 0.6;
	return 0.9;
}

void main()  
{  
    outcolor = texture(tex, vec2(fuv.x, 1.0 - fuv.y));

    vec3 vertexToLight = normalize(lightPos - fpos); 
    float lightIntensity = discretisa( clamp(dot(vertexToLight, normalize(fnormal)), 0.0, 1.0));// * 3.0;
    
    outcolor = vec4(outcolor.xyz * lightIntensity, 1.0);
    outpos = fpos;
    outuv = fuv;
    outnormal = fnormal;
}

