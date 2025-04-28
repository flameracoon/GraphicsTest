#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

enum LightType {
	LIGHT		=0,
	DIRECTIONAL =1,
	POINT		=2,
	SPOTLIGHT	=3,
};

struct Light {
	glm::vec3 position{0.f,0.f,0.f};
	glm::vec3 color{1.f,1.f,1.f};
	static glm::vec3  ambientStrength;
	glm::vec3  diffuseStrength{1.f};
	glm::vec3  specularStrength{1.f};
	virtual void SetUniform(Shader* shader, size_t number);
	float linear{0.09f};
	float quadratic{ 0.032f };
};

struct DirectionalLight:public Light {
	
	glm::vec3 direction;
	void SetUniform(Shader* shader, size_t number);
private:
	glm::vec3 position;
	float linear;
	float quadratic;
};

struct SpotLight :public Light {

	glm::vec3 direction{0.f,0.f,1.f};
	float cutOff{5.5f};
	float outerCutOff{10.5f};
	void SetUniform(Shader* shader, size_t number);
};