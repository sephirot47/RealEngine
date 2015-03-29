#version 130	

struct DirectionalLight
{
	mat4 projection, view, projectionView, biasedProjectionView;
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
    vec4 shadowCoord = light.biasedProjectionView * worldPosition;
    shadowCoord /= shadowCoord.w;

    float shadow = 1.0;
    if(texture(shadowDepthBuffer, shadowCoord.xy).z < shadowCoord.z - 0.005 ) shadow = light.shadow;

    vec3 normal = texture(GNormal, screenuv).xyz;
    float brightness = max( 0.0, dot(-light.direction, normal) );

    vec3 colorAddition = texture(GMaterialTexture, screenuv).rgb * light.color * light.intensity * brightness * shadow;
    outGColor = vec4( texture(GColor, screenuv).rgb + colorAddition, 1.0);
}






