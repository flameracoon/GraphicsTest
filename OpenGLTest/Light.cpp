#include "Light.h"
#include <sstream>

glm::vec3 Light::ambientStrength{ 0.1f };

 
float CaluclateRadius(glm::vec3 color, float linear, float quadratic) {
    return (-linear + std::sqrt(linear * linear - 4 * quadratic * (1.0f - (256.0f / 5.0f) * std::fmaxf(std::fmaxf(color.r, color.g), color.b)))) / (2.0f * quadratic);;;
}
void Light::SetUniform(Shader* shader,size_t number) {
	shader->Use();

    std::stringstream s;

    // Set the position of the light
    s << "light[" << number << "].position";
    shader->SetVec3(s.str(), this->position);

    s.str("");
    s << "light[" << number << "].color";
    shader->SetVec3(s.str(), this->color);

    s.str("");
    s << "light[" << number << "].La";
    shader->SetVec3(s.str(), this->ambientStrength);

    s.str("");
    s << "light[" << number << "].Ld";
    shader->SetVec3(s.str(), this->diffuseStrength);

    s.str("");
    s << "light[" << number << "].Ls";
    shader->SetVec3(s.str(), this->specularStrength);


    s.str("");
    s << "light[" << number << "].linear";
    shader->SetFloat(s.str(), this->linear);

    s.str("");
    s << "light[" << number << "].quadratic";
    shader->SetFloat(s.str(), this->quadratic);

    s.str("");
    s << "light[" << number << "].radius";
    shader->SetFloat(s.str(), CaluclateRadius(this->color,linear,quadratic));
    shader->Disuse();
}

void SpotLight::SetUniform(Shader* shader, size_t number) {

    shader->Use();

    std::stringstream s;

    // Set the position of the light
    s << "spotLight[" << number << "].position";
    shader->SetVec3(s.str(), this->position);

    s.str("");
    s << "spotLight[" << number << "].color";
    shader->SetVec3(s.str(), this->color);

    s.str("");
    s << "spotLight[" << number << "].La";
    shader->SetVec3(s.str(), this->ambientStrength);

    s.str("");
    s << "spotLight[" << number << "].Ld";
    shader->SetVec3(s.str(), this->diffuseStrength);

    s.str("");
    s << "spotLight[" << number << "].Ls";
    shader->SetVec3(s.str(), this->specularStrength);


    s.str("");
    s << "spotLight[" << number << "].linear";
    shader->SetFloat(s.str(), this->linear);

    s.str("");
    s << "spotLight[" << number << "].quadratic";
    shader->SetFloat(s.str(), this->quadratic);

    s.str("");
    s << "spotLight[" << number << "].radius";
    shader->SetFloat(s.str(), CaluclateRadius(this->color, linear, quadratic));

    
    s.str("");
    s << "spotLight[" << number << "].direction";
    shader->SetVec3(s.str(), normalize(direction));

    s.str("");
    s << "spotLight[" << number << "].cutOff";
    shader->SetFloat(s.str(), glm::cos(glm::radians(this->cutOff)));

    s.str("");
    s << "spotLight[" << number << "].outerCutOff";
    shader->SetFloat(s.str(), glm::cos(glm::radians(this->outerCutOff)));

    shader->Disuse();

}
void DirectionalLight::SetUniform(Shader* shader, size_t number) {
    shader->Use();

    std::stringstream s;

    // Set the position of the light
    s << "directionalLight[" << number << "].direction";
    shader->SetVec3(s.str(), normalize(-direction));

    s.str("");
    s << "directionalLight[" << number << "].color";
    shader->SetVec3(s.str(), this->color);

    s.str("");
    s << "directionalLight[" << number << "].La";
    shader->SetVec3(s.str(), this->ambientStrength);

    s.str("");
    s << "directionalLight[" << number << "].Ld";
    shader->SetVec3(s.str(), this->diffuseStrength);

    s.str("");
    s << "directionalLight[" << number << "].Ls";
    shader->SetVec3(s.str(), this->specularStrength);

    shader->Disuse();
}