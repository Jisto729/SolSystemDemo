#include "Camera.h"
//rotate function from https://github.com/VictorGordan/opengl-tutorials/tree/main/YoutubeOpenGL%208%20-%20Camera

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


void Camera::moveX(float dx)
{
	movement = glm::vec3(dx, movement.y, movement.z);
}

void Camera::moveY(float dy)
{
	movement = glm::vec3(movement.x, dy, movement.z);
}

void Camera::moveZ(float dz)
{
	movement = glm::vec3(movement.x, movement.y, dz);
}


void Camera::stopX()
{
	movement = glm::vec3(0, movement.y, movement.z);
}

void Camera::stopY()
{
	movement = glm::vec3(movement.x, 0, movement.z);
}

void Camera::stopZ()
{
	movement = glm::vec3(movement.x, movement.y, 0);
}

void Camera::rotate(float dx, float dy) {
	float rotateX = sensitivityX * dx;
	float rotateY = sensitivityY * dy;

	// vertical rotation
	glm::vec3 rotAxis = glm::normalize(glm::cross(orientation, Up));
	glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotateX), rotAxis);

	// rotates vertically only if able
	if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(80.0f))
	{
		orientation = newOrientation;
	}

	// horizontal rotation
	orientation = glm::rotate(orientation, glm::radians(-rotateY), Up);
}

void Camera::move() {
	glm::vec3 dx = movement.x * glm::normalize(glm::cross(orientation, Up));
	glm::vec3 dy = movement.y * Up;
	glm::vec3 dz = movement.z * glm::normalize(glm::vec3(orientation.x, 0.0f, orientation.z));
	position += dx + dy + dz;
}