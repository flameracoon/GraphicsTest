#pragma once
#ifndef  ENVONE_H
#define ENVONE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include "Shader.h"
#include "stb_image.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace EnvOne {
	void init();
	void update();
	void cleanUp();
}
#endif
