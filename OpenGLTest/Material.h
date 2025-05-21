#pragma once
#include "Shader.h"
#include "CubeMap.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Texture.h"
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

struct PBRMaterial {
	PBRMaterial(Texture* albedoTex=nullptr,
				Texture* specularTex = nullptr,
				Texture* roughnessTex = nullptr,
				Texture* aoTex = nullptr,
				Texture* normalTex = nullptr);
	Texture* albedo;
	Texture* specular;
	Texture* roughness;
	Texture* ao;
	Texture* normal;
};