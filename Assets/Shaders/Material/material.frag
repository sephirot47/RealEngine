#version 130	

struct Material
{
	sampler2D texture;
	vec3 diffuse, specular;
	float shininess;
};
uniform Material material;
uniform sampler2D tex;

in vec3 fpos;
in vec2 fuv;
in vec3 fnormal;

out vec4 outFinalColors;
out vec3 outpos;
out vec2 outuv;
out vec3 outnormal;
out vec4 outTextureColors;

void main()
{  
    outFinalColors = vec4(texture(tex, vec2(fuv.x, 1.0 - fuv.y)).rgb * 0.05, 1);
    outpos = fpos;
    outuv = fuv;
    outnormal = fnormal;
    outTextureColors = vec4(texture(material.texture, vec2(fuv.x, 1.0 - fuv.y)).rgb, 1);
}

