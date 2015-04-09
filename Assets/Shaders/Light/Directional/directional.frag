#version 130	

struct Camera
{
	vec3 position;
};
uniform Camera camera;

struct DirectionalLight
{
	mat4 projection, view, projectionView, biasedProjectionView;
	vec3 direction, color;
	float intensity, shadow;
};
uniform DirectionalLight light;

uniform sampler2D GColor, GPosition, GNormal, GMaterialTexture, GMaterialShininess, GDepth;
uniform sampler2D shadowDepthBuffer;
uniform float shadowMapWidth, shadowMapHeight;

in vec2 screenuv;

out vec4 outGColor;

float shadowBias = 0.05;
int blurRadius = 0;

float GetShadow(vec4 worldPosition)
{
	if( dot(-light.direction, texture(GNormal, screenuv).xyz) < 0 ) return 0.0; //facing away from light

        vec4 shadowCoord = light.biasedProjectionView * worldPosition;
	shadowCoord /= shadowCoord.w;
        if(shadowCoord.z > texture(shadowDepthBuffer, shadowCoord.xy).z + shadowBias) return light.shadow;

	float xoffset = 1.0/shadowMapWidth, yoffset = 1.0/shadowMapHeight;
	float shadowOffset = (1.0-light.shadow) / (((blurRadius*2+1) * (blurRadius*2+1)));
	float shadow = light.shadow;

	for(int i = -blurRadius; i <= blurRadius; ++i)
	{
		for(int j = -blurRadius; j <= blurRadius; ++j)
		{
			vec2 offset = vec2(xoffset * i, yoffset * j);
			if(shadowCoord.z < texture(shadowDepthBuffer, shadowCoord.xy + offset).z + shadowBias)
				shadow += shadowOffset;
		}
	}
	return shadow;
}

void main()  
{
    if(texture(GDepth, screenuv).x > 0.9999)
    {
	outGColor = vec4(texture(GColor, screenuv).rgb, 1);
 	return;
    }

    vec4 worldPosition = vec4(texture(GPosition, screenuv).xyz, 1);

    vec3 normal = texture(GNormal, screenuv).xyz;
    float brightness = max( 0.0, dot(-light.direction, normal) );
    float specLight = dot( reflect(light.direction, normal), normalize(worldPosition.xyz - camera.position) );
    specLight = pow( max(0.0, specLight) , texture(GMaterialShininess, screenuv).x);
  
    float shadow = GetShadow(worldPosition);

    vec3 textureColor  = texture(GMaterialTexture, screenuv).rgb;
    vec3 diffuseLight  = textureColor * light.color * light.intensity * brightness * shadow;
    vec3 specularLight = max(vec3(0), light.color * specLight * brightness * shadow);
    outGColor = vec4( texture(GColor, screenuv).rgb + diffuseLight + specularLight * 0, 1.0);
}






