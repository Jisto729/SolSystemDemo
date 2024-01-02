#include "Camera.h"

//TODO remove
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

using namespace ssd;

Camera::Camera(glm::vec3 defaultPosition, float defaultFov, float defaultNearPlane, float defaultFarPlane)
{
	position = defaultPosition;
	fov = defaultFov;
	nearPlane = defaultNearPlane;
	farPlane = defaultFarPlane;
}

void Camera::updateMatrix(int windowWidth, int windowHeight)
{
	viewMatrix = glm::lookAt(position, position + orientation, Up);
	projectionMatrix = glm::perspective(glm::radians(fov), (float)(windowWidth / windowHeight), nearPlane, farPlane);

	cameraMatrix = projectionMatrix * viewMatrix;
}


void Camera::updatePosition(glm::vec3 newPosition) {
	position = newPosition;
}
void Camera::updateOrientation(glm::vec3 newOrientation) {
	orientation = newOrientation;
}


void Camera::moveZ(float dz) {
	position += dz * Up;
}

void Camera::moveXY(float dx, float dy) {
	position += dx * -glm::normalize(glm::cross(orientation, Up)) + dy * orientation;
}

void Camera::rotate(float dx, float dy) {
	float rotX = sensitivityX * dx;
	float rotY = sensitivityY * dy;

	// vertical rotation
	glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, Up)));

	// rotates vertically only if able
	//TODO fix rotation when looing straight down
	if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(89.0f))
	{
		orientation = newOrientation;
	}

	// horizontal rotation
	orientation = glm::rotate(orientation, glm::radians(-rotY), Up);
}

void Camera::zoom(float dz) {
	moveXY(dz, 0);
}

void Camera::move(float dx, float dy) {
	moveXY(dx, dy);
}

//glm::mat4 Camera::getMatrix() const {
//	return glm::mat4(1.0f);
//}