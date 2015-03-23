#version 130	

uniform mat4 lightProjection, lightView;
uniform vec3 lightDir, lightColor;
uniform float lightIntensity;
uniform sampler2D colors, pos, normals, depth, shadowDepthBuffer;

mat4 biasMatrix = mat4(0.5, 0.0, 0.0, 0.0,
		       0.0, 0.5, 0.0, 0.0,
		       0.0, 0.0, 0.5, 0.0,
		       0.5, 0.5, 0.5, 1.0);

in vec2 screenuv;

out vec4 outcolor;

void main()  
{  
    float isBg = texture(depth, screenuv).x > 0.9999 ? 1.0 : 0.0;
    if(isBg > 0.5)  { outcolor = vec4(0.2, 0.0, 0.0, 1.0); return; }

    vec3 worldPosition = texture(pos, screenuv).xyz;
    vec3 normal = normalize(texture(normals, screenuv).xyz);
    vec4 shadowCoord = biasMatrix * lightProjection * lightView * vec4(worldPosition, 1.0);
    
   // outcolor = vec4(vec3( shadowCoord.z ), 1); return;
    float shadow = 1.0;
    if(shadowCoord.z - texture(shadowDepthBuffer, shadowCoord.xy).z > 0.001 )
    {
	shadow = 0.1;
    }

    float brightness = max(0.0, dot(-normalize(lightDir), normal));
    outcolor = vec4(texture(colors, screenuv).xyz + texture(colors, screenuv).xyz * lightIntensity * lightColor * brightness * shadow, 1.0);
}

