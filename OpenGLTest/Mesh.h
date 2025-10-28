#pragma once
#include "glm/glm.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#define PI 3.1415
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
struct Sphere :BasicMesh {
	int sectorCount{ 36 }, stackCount{ 18 };
	float radius{1.f};
	void CreateMesh();
	void DrawMesh();
};
struct DebugCube :BasicMesh {
	float lineWidth{ 2.0 };
	void CreateMesh();
	void DrawMesh();
};
struct DebugCircle :BasicMesh {
	float radius{ 25.f };
	void CreateMesh();
	void DrawMesh();
	glm::mat4 RotateZtoV(glm::vec3 V) {
		glm::vec3 U = glm::cross(V, glm::vec3{ 0, 0, 1 });
		//if (glm::dot(U, U) < 1e-6f) return glm::mat4(1.f);

		glm::vec3 C = glm::normalize(V);
		glm::vec3 A = glm::normalize(glm::cross(C, U));
		glm::vec3 B = glm::cross(A, C);

		return glm::mat4{
		glm::vec4{A, 0.f},
		glm::vec4{B, 0.f},
		glm::vec4{C, 0.f},
		glm::vec4{0, 0, 0, 1} };
	}
};