#include "EventHandler.h"
#include <QKeyEvent>


#include <iostream>
#include <glm/gtx/string_cast.hpp>

using namespace ssd;

EventHandler::EventHandler(std::shared_ptr<Camera> camera_ptr)
{
	camera = camera_ptr;
}

bool EventHandler::event(QEvent* event) {
	switch (event->type())
	{
	case QEvent::KeyPress:
	{
		QKeyEvent* keyEvent = (static_cast<QKeyEvent*>(event));
		this->handleKeyboard(keyEvent->key(), true);
		return true;
	}
	case QEvent::KeyRelease:
	{
		QKeyEvent* keyEvent = (static_cast<QKeyEvent*>(event));
		this->handleKeyboard(keyEvent->key(), false);
		return true;
	}
	default:
		return false;
	}
}

bool EventHandler::event(QEvent* event, int windowWidth, int windowHeight) {
    switch (event->type())
    {
    case QEvent::MouseMove:
    {
        QMouseEvent* mouseEvent = (static_cast<QMouseEvent*>(event));
        this->handleMouse(mouseEvent->x(), mouseEvent->y(), windowWidth, windowHeight);
        return true;
    }
    default:
        return false;
    }
}

void EventHandler::handleKeyboard(int key, bool isPressed) {
	float speed = camera->getSpeed();
	if (key == 'W')
	{
		isPressed ? camera->moveZ(speed) : camera->stopZ();
	}
	if (key == 'A')
	{
		isPressed ? camera->moveX(-speed) : camera->stopX();
	}
	if (key == 'S')
	{
		isPressed ? camera->moveZ(-speed) : camera->stopZ();
	}
	if (key == 'D')
	{
		isPressed ? camera->moveX(speed) : camera->stopX();
	}
	if (key == ' ')
	{
		isPressed ? camera->moveY(speed) : camera->stopY();
	}
	if (key == 'F')
	{
		isPressed ? camera->moveY(-speed) : camera->stopY();
	}
	if (key == 0x01000000)
	{
		exit(0);
	}
}

void EventHandler::handleMouse(int x, int y, int windowWidth, int windowHeight) {
	camera->rotate((float)(y - (windowHeight / 2)) / windowHeight, (float)(x - (windowWidth / 2)) / windowWidth);
	// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
}