#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

layout(location=0)  uniform sampler2D gPosition;
layout(location=1)  uniform sampler2D gNormal;
layout(location=2)  uniform sampler2D gAlbedoSpec;
layout(location=3)  uniform sampler2D gReflect;  // Tangent-space light direction
layout(binding=4)   uniform sampler2D gMaterial;

layout(binding=5)   uniform samplerCube cubeTexture;




struct Light 
{
    vec3 position;      // Position of the light source in the world space
    vec3 color;
    vec3 La;            // Ambient light intensity
    vec3 Ld;            // Diffuse light intensity
    vec3 Ls;            // Specular light intensity
    float linear;
    float quadratic;
    float radius;
};
struct SpotLight 
{
    vec3 position;      // Position of the light source in the world space
    vec3 color;
    vec3 La;            // Ambient light intensity
    vec3 Ld;            // Diffuse light intensity
    vec3 Ls;            // Specular light intensity
    float linear;
    float quadratic;
    float radius;
    float cutOff;
    float outerCutOff;
    vec3 direction;      // Position of the light source in the world space

};

struct DirectionalLight 
{
    vec3 direction;      // Position of the light source in the world space
    vec3 color;
    vec3 La;            // Ambient light intensity
    vec3 Ld;            // Diffuse light intensity
    vec3 Ls;            // Specular light intensity

};

uniform int pointLightNo;
uniform int dirLightNo;
uniform int spotLightNo;

uniform Light light[32];
uniform DirectionalLight directionalLight[10];
uniform SpotLight spotLight[32];
uniform vec3 lightAmbience;


uniform mat4 view;  
vec3 diffuseColor;
float specularColor;


//Lighting
const float PI = 3.14159265358979323846;

//
// The Microgeometry Normal Distribution Function, based on GGX/Trowbrodge-Reitz, 
// that describes the relative concentration of microfacet normals 
// in the direction H. It has an effect on the size and shape 
// of the specular highlight.
//
// Parameter is cosine of the angle between the normal and H which is the halfway vector of 
// both the light direction and the view direction
//
float ggxDistribution(float nDotH,float roughness) 
{
    float alpha2 = roughness * roughness * roughness * roughness;
    float d = (nDotH * nDotH) * (alpha2 - 1.0f) + 1.0f;
    return alpha2 / (PI * d * d);
}

//
// The Smith Masking-Shadowing Function describes the probability that microfacets with 
// a given normal are visible from both the light direction and the view direction.
//
// Parameter is cosine of the angle between the normal vector and the view direction.
//
float geomSmith(float nDotL,float roughness) 
{
    float k = (roughness + 1.0f) * (roughness + 1.0f) / 8.0f;
    float denom = nDotL * (1.0f - k) + k;
    return 1.0f / denom;
}

//
// Schlick approximation for Fresnel reflection that defines the probability of light
// reflected from an optically flat surface.
//
// Parameter is cosine of the angle between the light direction vector and 
// the halfway vector of both the light direction and the view direction
//
vec3 schlickFresnel(float lDotH) 
{
    vec3 f0 = vec3(0.04f); // Dielectrics
    f0 = mix(f0, diffuseColor, specularColor);
    return f0 + (1.0f - f0) * pow(1.0f - lDotH, 5);
}

//
// Bidirectional Reflectance Distribution Function.
// This is the common way to model reflectance based on microfacet theory. 
// This theory was developed to describe reflection from general, non-optically flat surfaces. 
// It models the surface as consisting of small facets that are optically flat (mirrors) and 
// are oriented in various directions. Only those that are oriented correctly to reflect toward 
// the viewer can contribute.
//
// Parameters are the position of a fragment and the surface normal in the view space.
//


vec3 microfacetModel(vec3 position, vec3 n,vec3 color,float roughness,int i) 
{  
    vec3 lightI = light[i].color;
    vec3 lightPositionInView = (view * vec4(light[i].position, 1.0f)).xyz;

    vec3 l = lightPositionInView - position;
    float dist = length(l);
    l = normalize(l);
     if (dist < light[i].radius)
    {
    lightI =lightI*(1.f/ (dist * dist)); // Intensity is normalized, so scale up by 100 first

    vec3 v = normalize(-position);
    vec3 h = normalize(v + l);
    float nDotH = dot(n, h);
    float lDotH = dot(l, h);
    float nDotL = max(dot(n, l), 0.0f);
    float nDotV = dot(n, v);

            // Cook-Torrance BRDF   
        vec3 F    = schlickFresnel(lDotH);
           
        //vec3 numerator    = ggxDistribution(nDotH,roughness) * geomSmith(nDotL,roughness) *geomSmith(nDotV,roughness)* F; 
        //float denominator = 4.0 * nDotV * nDotL; // + 0.0001 to prevent divide by zero
        //vec3 specular = numerator / denominator;
        vec3 specBrdf = 0.25f * ggxDistribution(nDotH,roughness) * F 
                            * geomSmith(nDotL,roughness) * geomSmith(nDotV,roughness);
        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - specularColor;	  

        // scale light by NdotL

    return (kD*diffuseColor /PI + specBrdf) * lightI * nDotL;
        }
    return vec3(0.f);
}
vec3 spotlightMicrofacetModel(vec3 position, vec3 n,vec3 color,float roughness,int i) 
{  

    vec3 diffuseBrdf = color;

    vec3 lightI = spotLight[i].color;
    vec3 lightPositionInView = (view * vec4(spotLight[i].position, 1.0f)).xyz;
    vec3 dir = normalize(mat3(view) * spotLight[i].direction);

    vec3 l = lightPositionInView - position;
    float dist = length(l);
    l = normalize(l);
    if (dist < spotLight[i].radius||any(notEqual(n, vec3(0.0f, 0.0f, 0.0f))))
    {
    float theta = dot(l, dir); 
    float epsilon = (spotLight[i].cutOff - spotLight[i].outerCutOff);
    float intensity = clamp((theta - spotLight[i].outerCutOff) / epsilon, 0.0, 1.0);
    lightI*=intensity;
    lightI =lightI*(1.f/ (dist * dist)); // Intensity is normalized, so scale up by 100 first


    vec3 v = normalize(-position);
    vec3 h = normalize(v + l);
    float nDotH = dot(n, h);
    float lDotH = dot(l, h);
    float nDotL = max(dot(n, l), 0.0f);
    float nDotV = dot(n, v);

            // Cook-Torrance BRDF   
        vec3 F    = schlickFresnel(lDotH);
           
        //vec3 numerator    = ggxDistribution(nDotH,roughness) * geomSmith(nDotL,roughness) *geomSmith(nDotV,roughness)* F; 
        //float denominator = 4.0 * nDotV * nDotL; // + 0.0001 to prevent divide by zero
        //vec3 specular = numerator / denominator;
        vec3 specBrdf = 0.25f * ggxDistribution(nDotH,roughness) * F 
                            * geomSmith(nDotL,roughness) * geomSmith(nDotV,roughness);
        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - specularColor;	  

        // scale light by NdotL

    return (kD*diffuseColor /PI + specBrdf) * lightI * nDotL;
    }
    return lightI;
}

vec3 microFacetDirection(vec3 position, vec3 n,vec3 color,float roughness,int i) 
{  


    vec3 lightI = directionalLight[i].color;

    vec3 l = normalize((mat3(view) * directionalLight[i].direction));


    vec3 v = normalize(-position);
    vec3 h = normalize(v + l);
    float nDotH = dot(n, h);
    float lDotH = dot(l, h);
    float nDotL = max(dot(n, l), 0.0f);
    float nDotV = dot(n, v);

         vec3 F    = schlickFresnel(lDotH);
           
        //vec3 numerator    = ggxDistribution(nDotH,roughness) * geomSmith(nDotL,roughness) *geomSmith(nDotV,roughness)* F; 
        //float denominator = 4.0 * nDotV * nDotL; // + 0.0001 to prevent divide by zero
        //vec3 specular = numerator / denominator;
        vec3 specBrdf = 0.25f * ggxDistribution(nDotH,roughness) * F 
                            * geomSmith(nDotL,roughness) * geomSmith(nDotV,roughness);
        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - specularColor;	  

        // scale light by NdotL

    return (kD*diffuseColor /PI + specBrdf) * lightI * nDotL; 
}


void main()
{   
    
     vec4 testColor = texture(gAlbedoSpec, TexCoords);
    if (testColor.x==0&&testColor.y==0.f&&testColor.z==0.f&&testColor.a==0.f){
    
            discard;;   

    }
    diffuseColor =testColor.rgb;
    specularColor =testColor.a;
    //Create Reflection
    vec3 envMap = texture(cubeTexture, vec3(texture(gReflect, TexCoords)) ).rgb;
    vec3 newMat=vec3(texture(gMaterial, TexCoords));

    vec3 newLight=diffuseColor*lightAmbience*newMat.r;

    vec3 normalMap=normalize(vec3(texture(gNormal, TexCoords)));
    vec3 positionMap=vec3(texture(gPosition, TexCoords));

    for(int i=0;i<pointLightNo;i++){
        newLight+=microfacetModel(positionMap, normalMap,diffuseColor,newMat.g,i);
    }
    for(int i=0;i<spotLightNo;i++){
        newLight+=spotlightMicrofacetModel(positionMap, normalMap,diffuseColor,newMat.g,i);
    }
    for(int i=0;i<dirLightNo;i++){
        newLight+=microFacetDirection(positionMap, normalMap,diffuseColor,newMat.g,i);
    }
     vec3 color = newLight/ (newLight + vec3(1.0));
     //color=mix(color,envMap,specularColor);
     color = pow(color, vec3(1.0/2.2)); 
    //FragColor = vec4(color, 1.0);
    FragColor = vec4(color,1.0);

    //Use re
}