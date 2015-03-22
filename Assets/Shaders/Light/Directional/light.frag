#version 130	

uniform mat4 inverseCamView, camView, lightView, lightProjection;
uniform vec3 lightDir, lightColor;
uniform float lightIntensity;
uniform sampler2D colors, pos, normals, depth, shadowDepthBuffer;

in vec2 screenuv;

out vec4 outcolor;

void main()  
{  
    float isBg = texture(depth, screenuv).x > 0.9999 ? 1.0 : 0.0;
    if(isBg > 0.5)  { outcolor = texture(colors, screenuv); return; }

    vec3 worldPosition = texture(pos, screenuv).xyz;
    vec3 normal = normalize(texture(normals, screenuv).xyz);

    vec3 projectedPositionFromLight = (lightProjection * lightView * vec4(worldPosition, 1.0)).xyz;
    vec2 shadowBufferUv = vec2(projectedPositionFromLight.x * 0.5 + 0.5, projectedPositionFromLight.y * 0.5 + 0.5);
    float distLightToBlocking = texture2D( shadowDepthBuffer, shadowBufferUv ).x;
    float distLightToPixel = -projectedPositionFromLight.z * 0.5 + 0.5;
    float difOffset = 0.000001;

    //outcolor = texture(shadowDepthBuffer, sceneuv); return;
   // outcolor = vec4( vec3(texture(shadowDepthBuffer, shadowBufferUv).z), 1); return;
    //outcolor = vec4(vec3(distLightToPixel), 1.0);return;
    //if(distLightToPixel > 0.6) outcolor = vec4(1, 0, 0, 1.0);
    //else outcolor = vec4(1,1,1,1);
    //return;

    if(distLightToPixel - distLightToBlocking > difOffset)
    {
	outcolor = vec4(texture(colors, screenuv).xyz * 0.9, 1.0);
	//outcolor = vec4(1, 0, 0, 0);
	return;
    }

    float brightness = max(0.0, dot(-normalize(lightDir), normal));
    outcolor = vec4(texture(colors, screenuv).xyz + texture(colors, screenuv).xyz * lightIntensity * lightColor * brightness, 1.0);
}

