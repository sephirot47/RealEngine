#version 130	

mat4 biasMatrix = mat4(0.5, 0.0, 0.0, 0.0,
		       0.0, 0.5, 0.0, 0.0,
		       0.0, 0.0, 0.5, 0.0,
		       0.5, 0.5, 0.5, 1.0);

struct DirectionalLight
{
	mat4 projection, view;
	vec3 direction, color;
	float intensity, shadow;
};
uniform DirectionalLight light;
uniform sampler2D GColor, GPosition, GUv, GNormal, GMaterialTexture, GDepth;
uniform sampler2D shadowDepthBuffer;

in vec2 screenuv;

out vec4 outGColor;

void main()  
{
    if(texture(GDepth, screenuv).x > 0.9999)
    {
	outGColor = vec4(texture(GColor, screenuv).rgb, 1);
 	return;
    }

    vec4 worldPosition = vec4(texture(GPosition, screenuv).xyz, 1);
    vec4 projectionCoord = light.projection * light.view * worldPosition;
    projectionCoord /= projectionCoord.w;
    vec4 shadowCoord = biasMatrix * projectionCoord;

    float shadow = 1.0;
    if(texture(shadowDepthBuffer, shadowCoord.xy).z < shadowCoord.z - 0.005 ) shadow = light.shadow;

    vec3 normal = normalize(texture(GNormal, screenuv).xyz);
    float brightness = max(0.0, dot(-normalize(light.direction), normal));

    vec3 colorAddition = texture(GMaterialTexture, screenuv).rgb * light.color * light.intensity * brightness * shadow;
    outGColor = vec4( texture(GColor, screenuv).rgb + colorAddition, 1.0);
}






