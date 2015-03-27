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
    vec4 worldPosition = vec4(texture(pos, screenuv).xyz, 1);
    vec4 projectionCoord = lightProjection * lightView * worldPosition;
    projectionCoord /= projectionCoord.w;
    vec4 shadowCoord = biasMatrix * projectionCoord;

    float shadow = 1.0;
    if(texture(shadowDepthBuffer, shadowCoord.xy).z < shadowCoord.z - 0.005 ) shadow = 0.3;

    vec3 normal = normalize(texture(normals, screenuv).xyz);
    float brightness = max(0.0, dot(-normalize(lightDir), normal));

    outFinalColors = vec4(
		   	   texture(finalColors, screenuv).xyz +
	       		   texture(textureColors, screenuv).xyz * lightColor * shadow * lightIntensity * brightness
		   	 ,1.0);

    outTextureColors = texture(textureColors, screenuv);
    outpos = texture(pos, screenuv).xyz;
    outuv = texture(uvs, screenuv).xy;
    outnormal = texture(normals, screenuv).xyz;
}






