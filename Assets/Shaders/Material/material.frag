#version 130	

struct Material
{
	sampler2D texture;
	float shininess, shineStrength;
};
uniform Material material;

in vec3 fpos;
in vec2 fuv;
in vec3 fnormal;

out vec4 outGColor;
out vec3 outGPosition;
out vec2 outGUv;
out vec3 outGNormal;

out vec4 outGMaterialTexture;
out vec3 outGMaterialShininess;

void main()
{  
    outGColor = vec4(vec3(texture(material.texture, vec2(fuv.x, 1.0 - fuv.y))) * 0.3, 1);
    outGPosition = fpos;
    outGUv = fuv;
    outGNormal = normalize(fnormal);

    outGMaterialTexture = vec4(texture(material.texture, vec2(fuv.x, 1.0 - fuv.y)).rgb, 1);
    outGMaterialShininess = vec3(material.shininess);
}

