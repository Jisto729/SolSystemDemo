#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <geUtil/BasicCamera.h>


namespace ssd {
	class Camera : public ge::util::BasicManipulatorInterface
	{
	public:


		glm::vec3 position;
		glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

		float fov;
		float nearPlane;
		float farPlane;

		float speed = 0.05f;

		glm::mat4 cameraMatrix = glm::mat4(1.0f);

		Camera(glm::vec3 defaultPosition, float fov, float nearPlane, float farPlane);

		void Camera::updateMatrix(int windowWidth, int windowHeight);
		//TODO rename
		inline glm::mat4 Camera::getMatrix() { return cameraMatrix; };

		void updatePosition(glm::vec3 newPosition);
		void updateOrientation(glm::vec3 newOrientation);

		float getSpeed() { return speed; }

		float sensitivityX = 100;
		float sensitivityY = 100;

		//Matrix Interface
		glm::mat4 getMatrix() const override;

		//BasicManipulatorInterface
		void moveZ(float dz) override;
		void moveXY(float dx, float dy) override;
		void rotate(float dx, float dy) override;

		void zoom(float dz);
		void move(float dx, float dy);

		glm::vec3 getLocalUp() const override { return Up; }
		void setLocalUp(const glm::vec3& val) override
		{
			Up = glm::normalize(val);
		}

		//glm::vec3 getLocatUp() const override { return glm::vec3(1.0f); }

		//void setLocatUp(const glm::vec3& value) {}
	};
}


