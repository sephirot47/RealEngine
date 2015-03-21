#version 130	

uniform mat4 camView, camViewInverse, lightView, lightProjection;
uniform vec3 lightPosition, lightDir, lightColor;
uniform float lightIntensity;
uniform sampler2D colors, pos, uvs, normals, depth, shadowDepthBuffer;

in vec2 sceneuv;

out vec4 outcolor;

void main()  
{  
    vec3 position = vec4(texture(pos, sceneuv).xyz, 1.0).xyz;
    vec3 normal = normalize(texture(normals, sceneuv).xyz);

    vec3 projectedPositionFromLight = (lightProjection * lightView * vec4(position, 1.0)).xyz;
    float distLightToObstruding = texture( shadowDepthBuffer, projectedPositionFromLight.xy/2.0 + vec2(0.5, 0.5) ).z;
    //distLightToObstruding = texture(shadowDepthBuffer, sceneuv).z;
    float difOffset = 0.01;
	outcolor = vec4(vec3(normal), 1);
	return;
    if( abs(distLightToObstruding - projectedPositionFromLight.z) < difOffset)
    {
	outcolor = vec4(1, 1, 1, 1);
	return;
    }

    float brightness = max(0.0, dot(-normalize(lightDir), normal));
    float isBg = texture(depth, sceneuv).x > 0.99 ? 1.0 : 0.0;
    if(isBg > 0.98)  outcolor = texture(colors, sceneuv);
    else  outcolor = vec4(texture(colors, sceneuv).xyz + texture(colors, sceneuv).xyz * lightIntensity * lightColor * brightness, 1.0);
}

