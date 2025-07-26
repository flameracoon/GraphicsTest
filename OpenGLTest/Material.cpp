#include "Material.h"
#include <sstream>

void Material::SetUniform(Shader* shader) {
    shader->Use();

    std::stringstream s;

    // Set the position of the light
    s << "material.Ka";
    shader->SetVec3(s.str(), this->ambience);

    s.str("");
    s << "material.Kd";
    shader->SetVec3(s.str(), this->diffuse);

    s.str("");
    s << "material.Ks";
    shader->SetVec3(s.str(), this->specular);

    s.str("");
    s << "material.shininess";
    shader->SetFloat(s.str(), this->shininess);

    s.str("");
    s << "material.reflectivity";
    shader->SetFloat(s.str(), this->reflectivity);

    shader->Disuse();
}
PBRMaterial::PBRMaterial(Texture* albedoTex ,
                         Texture* specularTex ,
                         Texture* roughnessTex ,
                         Texture* aoTex ,
                         Texture* normalTex ) :
                          albedo{albedoTex}, specular{ specularTex }, roughness{ roughnessTex }, ao{ aoTex }, normal{ normalTex }
{}