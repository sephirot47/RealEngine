#version 130	

uniform mat4 camProjectionInverse, camViewInverse;
uniform mat4 lightProjection, lightView;
uniform vec3 lightDir, lightColor;
uniform float lightIntensity;
uniform sampler2D finalColors, textureColors, pos, uvs, normals, depth;
uniform sampler2D shadowDepthBuffer;

mat4 biasMatrix = mat4(0.5, 0.0, 0.0, 0.0,
		       0.0, 0.5, 0.0, 0.0,
		       0.0, 0.0, 0.5, 0.0,
		       0.5, 0.5, 0.5, 1.0);

in vec2 screenuv;

out vec4 outFinalColors;
out vec4 outTextureColors;
out vec3 outpos;
out vec2 outuv;
out vec3 outnormal;


void main()  
{  
    outFinalColors = texture(finalColors, screenuv);
    outTextureColors = texture(textureColors, screenuv);
    outpos = texture(pos, screenuv).xyz;
    outuv = texture(uvs, screenuv).xy;
    outnormal = texture(normals, screenuv).xyz;
}






