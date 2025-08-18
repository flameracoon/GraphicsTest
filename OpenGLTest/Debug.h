#pragma once
#include "glm/glm.hpp"

class DebugShape {
public:
	virtual void Draw();
private:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

};