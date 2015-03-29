#version 130	

mat4 biasMatrix = mat4(0.5, 0.0, 0.0, 0.0,
		       0.0, 0.5, 0.0, 0.0,
		       0.0, 0.0, 0.5, 0.0,
		       0.5, 0.5, 0.5, 1.0);

uniform mat4 lightProjection, lightView;
uniform vec3 lightDir, lightColor;
uniform float lightIntensity, lightShadow;
uniform sampler2D GColor, GPosition, GUv, GNormal, GMaterialTexture, GDepth;
uniform sampler2D shadowDepthBuffer;

in vec2 screenuv;

out vec4 outGColor;
out vec3 outGPosition;
out vec2 outGUv;
out vec3 outGNormal;
out vec4 outGMaterialTexture;

void main()  
{
    if(texture(GDepth, screenuv).x > 0.9999)
    {
	outGColor = vec4(texture(GColor, screenuv).rgb, 1);
 	return;
    }

    vec4 worldPosition = vec4(texture(GPosition, screenuv).xyz, 1);
    vec4 projectionCoord = lightProjection * lightView * worldPosition;
    projectionCoord /= projectionCoord.w;
    vec4 shadowCoord = biasMatrix * projectionCoord;

    float shadow = 1.0;
    if(texture(shadowDepthBuffer, shadowCoord.xy).z < shadowCoord.z - 0.005 ) shadow = lightShadow;

    vec3 normal = normalize(texture(GNormal, screenuv).xyz);
    float brightness = max(0.0, dot(-normalize(lightDir), normal));

    outGColor = vec4(
	texture(GColor, screenuv).rgb + 
	texture(GMaterialTexture, screenuv).rgb * lightColor * shadow * lightIntensity * brightness ,1.0);

    outGPosition = texture(GPosition, screenuv).xyz;
    outGUv = texture(GUv, screenuv).xy;
    outGNormal = texture(GNormal, screenuv).xyz;
    outGMaterialTexture = vec4(texture(GMaterialTexture, screenuv).rgb, 1);
}






