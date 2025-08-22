#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

layout(location=0)  uniform sampler2D gPosition;
layout(location=1)  uniform sampler2D gNormal;
layout(location=2)  uniform sampler2D gAlbedoSpec;
layout(location=3)  uniform sampler2D gReflect;  // Tangent-space light direction
layout(binding=4)   uniform sampler2D gMaterial;

layout(binding=5)   uniform samplerCube cubeTexture;
layout(binding=6)   uniform sampler2D shadowMap;

float toonColorLevel=10.f;
float toonScaleFactor=2.f;
void main()
{   
    
    vec3 newMat=vec3(texture(gMaterial, TexCoords));
    if(newMat.b>0.f){
       vec4 testColor = texture(gAlbedoSpec, TexCoords);
       FragColor = vec4(0.0,1.0,0.0, 1.0);
    }
   // FragColor = vec4(1.0,1.0,1.0, 1.0);
    //Use re
}