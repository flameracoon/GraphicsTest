#pragma once
#include "Shader.h"
#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "Framebuffer.h"
#include "Camera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct TmpGO {
	glm::vec3 position;
	glm::vec3 scale;
	Model* model;
};

class GraphicsManager {

public:
	void InitializeGraphicsManager();
	void Render();
private:
//House basic meshes
	Cube cube;
	Sphere sphere;
	DebugCube debugCube;
	DebugCircle debugCircle;
	FrameBuffer frameBuffer;
	GBuffer gBuffer;
	DepthBuffer depthBuffer;
	GLFWwindow* window;

};
