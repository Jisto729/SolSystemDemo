#pragma once

#include "Camera.h"
#include <QEvent>

namespace ssd {
	class EventHandler {
	public:
		EventHandler(std::shared_ptr<Camera> cameraPtr);
		bool event(QEvent* event);
		bool event(QEvent* event, int windowWidth, int windowHeight);
	private:
		void handleKeyboard(int key);
		void EventHandler::handleMouse(int x, int y, int windowWidth, int windowHeight);
		std::shared_ptr<Camera> camera;
	};
}