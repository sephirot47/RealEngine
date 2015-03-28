#version 130	

mat4 biasMatrix = mat4(0.5, 0.0, 0.0, 0.0,
		       0.0, 0.5, 0.0, 0.0,
		       0.0, 0.0, 0.5, 0.0,
		       0.5, 0.5, 0.5, 1.0);

uniform mat4 lightProjection, lightView;
uniform vec3 lightPosition, lightColor;
uniform float lightIntensity, lightShadow, lightRange;
uniform sampler2D finalColors, pos, uvs, normals, textureColors, depth;
uniform sampler2D shadowDepthBuffer;

in vec2 screenuv;

out vec4 outFinalColors;
out vec3 outpos;
out vec2 outuv;
out vec3 outnormal;
out vec4 outTextureColors;

void main()  
{  
    if(texture(depth, screenuv).x > 0.9999)
    {
	outFinalColors = vec4(texture(finalColors, screenuv).rgb, 1);
 	return;
    }

    vec4 worldPosition = vec4(texture(pos, screenuv).xyz, 1); 
    vec4 projectionCoord = lightProjection * lightView * worldPosition;
    projectionCoord /= projectionCoord.w;
    vec4 shadowCoord = biasMatrix * projectionCoord;

    float shadow = 1.0;
    if(texture(shadowDepthBuffer, shadowCoord.xy).z < shadowCoord.z - 0.005 ) shadow = lightShadow;

    vec3 lightDir = vec3(worldPosition.xyz) - lightPosition;
    vec3 normal = normalize(texture(normals, screenuv).xyz);
    float brightness = max(0.0, dot(-normalize(lightDir), normal));

    float distance = distance(worldPosition.xyz, lightPosition);
    float attenuation = clamp(lightRange/distance, 0.0, 1.0);
    vec3 colorAddition = texture(textureColors, screenuv).rgb * lightColor * shadow * lightIntensity * brightness;
    outFinalColors = vec4(
		   	   texture(finalColors, screenuv).rgb +
	       		   clamp(colorAddition, vec3(0,0,0), vec3(1,1,1)) * attenuation
		   	 ,1.0);

    outpos = texture(pos, screenuv).xyz;
    outuv = texture(uvs, screenuv).xy;
    outnormal = texture(normals, screenuv).xyz;
    outTextureColors = vec4(texture(textureColors, screenuv).rgb, 1);
}






