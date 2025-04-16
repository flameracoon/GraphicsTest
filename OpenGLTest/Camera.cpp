#include "Camera.h"

glm::mat4 Camera::CalculateViewMtx() {
    target = { 0.f,0.f,0.f };
	viewMtx= glm::lookAt(position, target, glm::vec3{ 0.0f, 1.0f, 0.0f });
	return viewMtx;
}
glm::mat4 Camera::GetVieMtx() { return viewMtx;; }

glm::mat4 Camera::CalculatePerspMtx() {
	perspMtx= glm::perspective(glm::radians(fov), size.x/size.y, near, far);
	return perspMtx;;
}
glm::mat4 Camera::GetPerspMtx() { return perspMtx;; }

// Handles cursor movement to adjust camera orientation
void Camera::onCursor(double xoffset, double yoffset)
{
    const float PI05 = glm::pi<float>() / 2.0f; // Half of pi for clamping

        // Calculate spherical coordinates for orbiting movement
        const float r = glm::sqrt(position.x * position.x +
            position.y * position.y + position.z * position.z);
        float alpha = glm::asin(position.y / r); // Vertical angle
        float betta = std::atan2f(position.x, position.z); // Horizontal angle

        // Adjust angles based on cursor offset
        if (yoffset < 0.0)
            alpha += -0.02f;
        else if (yoffset > 0.0)
            alpha += 0.02f;

        if (xoffset < 0.0)
            betta += 0.05f;
        else if (xoffset > 0.0)
            betta += -0.05f;

        // Clamp vertical angle
        alpha = glm::clamp(alpha, -PI05 + 0.01f, PI05 - 0.01f);

        // Update position based on spherical coordinates
        position.x = r * glm::cos(alpha) * glm::sin(betta);
        position.y = r * glm::sin(alpha);
        position.z = r * glm::cos(alpha) * glm::cos(betta);
    
}

void Camera::onScroll(double xoffset, double yoffset)
{

        // Calculate the distance from the origin and adjust it based on scroll input
        float r = glm::sqrt(position.x * position.x + position.y * position.y + position.z * position.z);
        const float alpha = glm::asin(position.y / r);
        const float betta = std::atan2f(position.x, position.z);

        r += yoffset > 0.0f ? -1.0f : 1.0f; // Zoom in or out
        if (r < 1.0f) r = 1.0f; // Clamp minimum distance

        // Update position based on new distance
        position.x = r * glm::cos(alpha) * glm::sin(betta);
        position.y = r * glm::sin(alpha);
        position.z = r * glm::cos(alpha) * glm::cos(betta);

}
