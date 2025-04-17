#include "Light.h"
#include <sstream>

glm::vec3 Light::ambientStrength{ 0.1f };

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

	shader->Disuse();
}