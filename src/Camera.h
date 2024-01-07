#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <geUtil/BasicCamera.h>


namespace ssd {
	class Camera
	{
	public:


		glm::vec3 position;
		glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.0f);

		float fov;
		float nearPlane;
		float farPlane;

		float speed = 0.05f;

		glm::mat4 cameraMatrix = glm::mat4(1.0f);
		glm::mat4 projectionMatrix = glm::mat4(1.0f);
		glm::mat4 viewMatrix = glm::mat4(1.0f);

		Camera(glm::vec3 defaultPosition, float fov, float nearPlane, float farPlane);

		void Camera::updateMatrix(int windowWidth, int windowHeight);
		//TODO rename
		inline glm::mat4 Camera::getProjectionMatrix() { return projectionMatrix; };
		inline glm::mat4 Camera::getViewMatrix() { return viewMatrix; };

		inline glm::vec3 getPosition() { return position; }
		void updatePosition(glm::vec3 newPosition);
		void updateOrientation(glm::vec3 newOrientation);

		float getSpeed() { return speed; }

		float sensitivityX = 100;
		float sensitivityY = 100;

		glm::mat4 Camera::getMatrix() const { return cameraMatrix; };

		void rotate(float dx, float dy);

		void moveX(float dx);
		void moveY(float dy);
		void moveZ(float dz);

		void stopX();
		void stopY();
		void stopZ();

		void move();

		glm::vec3 getLocalUp() const { return Up; }
		void setLocalUp(const glm::vec3& val)
		{
			Up = glm::normalize(val);
		}
	};
}


