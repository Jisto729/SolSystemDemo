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
	case QEvent::UpdateRequest:
		return true;
	case QEvent::KeyPress:
	{
		QKeyEvent* keyEvent = (static_cast<QKeyEvent*>(event));
		this->handleKeyboard(keyEvent->key());
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

void EventHandler::handleKeyboard(int key) {
	float speed = camera->getSpeed();
	if (key == 'W')
	{
		camera->moveXY(0, speed);
	}
	if (key == 'A')
	{
		camera->moveXY(speed, 0);
	}
	if (key == 'S')
	{
		camera->moveXY(0, -speed);
	}
	if (key == 'D')
	{
		camera->moveXY(-speed, 0);
	}
	if (key == ' ')
	{
		camera->moveZ(speed);
	}
	if (key == 'F')
	{
		camera->moveZ(-speed);
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