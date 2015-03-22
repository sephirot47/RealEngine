#version 130

uniform mat4 lightProjection, lightView, modelMatrix;

in vec3 pos;	

void main()
{  
    gl_Position = lightProjection * lightView * modelMatrix * vec4(pos, 1.0);
}

