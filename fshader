#version 130	

uniform float time;
uniform sampler2D tex;

in vec3 pos;
in vec2 uv;
in vec3 normal;

vec3 lightPos = vec3(100*sin(time*3.0), -50, 100*cos(time*3.0)), lightDir = (0.0, -0.3, -1.5) - lightPos;

float discretisa(float light)
{
	if(light < 0.2) return 0.2;
	if(light < 0.6) return 0.6;
	return 0.9;
}

void main()  
{  
    gl_FragColor = texture(tex, vec2(uv.x, 1.0 - uv.y)); //vec4(uv.x, uv.y, 0.0, 1.0);  

    vec3 vertexToLight = normalize(lightPos - pos); 
    float lightIntensity = discretisa( clamp(dot(vertexToLight, normalize(normal)), 0.0, 1.0));// * 3.0;
    gl_FragColor = vec4(gl_FragColor.xyz * lightIntensity, 1.0);
}

