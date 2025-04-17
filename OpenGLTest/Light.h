#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
struct Light {
	glm::vec3 position{0.f,0.f,0.f};
	glm::vec3 color{1.f,1.f,1.f};
	static glm::vec3  ambientStrength;
	glm::vec3  diffuseStrength{1.f};
	glm::vec3  specularStrength{1.f};
	virtual void SetUniform(Shader* shader, size_t number);
};