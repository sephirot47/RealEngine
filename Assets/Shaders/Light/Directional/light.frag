#version 130	

uniform mat4 camProjectionInverse, camViewInverse;
uniform mat4 lightProjection, lightView;
uniform vec3 lightDir, lightColor;
uniform float lightIntensity;
uniform sampler2D colors, pos, normals, depth;
uniform sampler2D shadowDepthBuffer;

mat4 biasMatrix = mat4(0.5, 0.0, 0.0, 0.0,
		       0.0, 0.5, 0.0, 0.0,
		       0.0, 0.0, 0.5, 0.0,
		       0.5, 0.5, 0.5, 1.0);

in vec2 screenuv;

out vec4 outcolor;

void main()  
{  
    vec4 worldPosition = camViewInverse * camProjectionInverse * texture(pos, screenuv);
    vec4 shadowCoord = biasMatrix * lightProjection * lightView * worldPosition;

    vec4 projectionCoord = lightProjection * lightView * worldPosition;
    //shadowCoord /= shadowCoord.w;

    //if(projectionCoord.x < 0.0 || projectionCoord.y < 0.0 || projectionCoord.x > 1.0 || projectionCoord.y > 1.0) { outcolor = vec4(0,1,0,1); return; }
   // if(projectionCoord.x == 1.0 || projectionCoord.y == 1.0) { outcolor = vec4(0,1,0,1); return; }
  // if(projectionCoord.x == 0.0 || projectionCoord.y == 0.0) { outcolor = vec4(0,0,1,1); return; }

//    outcolor = vec4(vec3(shadowCoord.xy, 0),1); return;

    float shadow = 1.0;
    if(texture(shadowDepthBuffer, shadowCoord.xy).z < shadowCoord.z - 0.0005 ) shadow = 0.1;

    vec3 normal = normalize(texture(normals, screenuv).xyz);
    float brightness = max(0.0, dot(-normalize(lightDir), normal));
    outcolor = vec4(texture(colors, screenuv).xyz + texture(colors, screenuv).xyz * lightIntensity * lightColor * brightness * shadow, 1.0);
}

