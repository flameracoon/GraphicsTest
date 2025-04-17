#pragma once
#include "Shader.h"
#include "CubeMap.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Material {
	glm::vec3 ambience{1.f,1.f,1.f};
	glm::vec3 diffuse{ 1.f,1.f,1.f };
	glm::vec3 specular{ 1.f,1.f,1.f };
	float shininess{100.f};

	float reflectivity;
	float refractivity;

	CubeMap* envMap{nullptr};
	void SetUniform(Shader*);
};