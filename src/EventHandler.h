#pragma once

#include "Camera.h"
#include <QEvent>

namespace ssd {
	class EventHandler {
	public:
		EventHandler(std::shared_ptr<Camera> cameraPtr);
		bool event(QEvent* event);
		//bool event(QEvent* event, int windowWidth, int windowHeight);
		bool checkFirstMove();
		bool isMousePressed();
		void handleMouse(int windowWidth, int windowHeight);
	private:
		void handleKeyboard(int key, bool isPressed);
		bool firstMove = true;
		bool mousePressed;
		int mouseX;
		int mouseY;
		std::shared_ptr<Camera> camera;
	};
}