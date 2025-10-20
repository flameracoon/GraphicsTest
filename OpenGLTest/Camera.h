#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
	glm::mat4 CalculateViewMtx();
	glm::mat4 GetVieMtx();
	glm::mat4 CalculatePerspMtx();
	glm::mat4 GetPerspMtx();
	void onCursor(double xoffset, double yoffset);
	void onScroll(double xoffset, double yoffset);


	float fov{ 45.0f };  // Field of view angle in degrees
	float near{0.05f};         // Near clipping plane
	float far{200.f};          // Far clipping plane
	glm::vec3 position; // Current position of the camera in 3D space
	glm::vec3 rotation; // Current position of the camera in 3D space
	glm::vec2 size{ 1600.f,900.f };
	glm::vec3 direction;
private:
	glm::vec3 target;   // Target point that the camera is looking at
	glm::mat4 viewMtx;
	glm::mat4 perspMtx;
};