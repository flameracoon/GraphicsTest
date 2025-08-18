#pragma once
#include "glm/glm.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
struct BasicMesh {
	GLenum primitiveType;
	GLuint vaoId;
	GLint drawCount;

	//To be overwrittern, mark as deleted
	virtual void CreateMesh()=0;
	virtual void DrawMesh()=0;
};

struct Cube :BasicMesh {
	void CreateMesh();
	void DrawMesh();
};