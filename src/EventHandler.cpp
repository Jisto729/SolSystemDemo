#include "EventHandler.h"
#include <QKeyEvent>


#include <iostream>
#include <glm/gtx/string_cast.hpp>

using namespace ssd;

EventHandler::EventHandler(std::shared_ptr<Camera> camera_ptr)
{
	camera = camera_ptr;
	mousePressed = false;
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
	case QEvent::MouseButtonPress:
	{
		std::cout << "button pressed" << std::endl;
		mousePressed = true;
		std::cout << mousePressed << std::endl;
		return true;
	}
	case QEvent::MouseButtonRelease:
	{
		std::cout << "button released" << std::endl;
		mousePressed = false;
		std::cout << mousePressed << std::endl;
		return true;
	}
	case QEvent::MouseMove:
	{
		QMouseEvent* mouseEvent = (static_cast<QMouseEvent*>(event));
		mouseX = mouseEvent->pos().x();
		mouseY = mouseEvent->pos().y();
		//this->handleMouse(mouseEvent->x(), mouseEvent->y(), windowWidth, windowHeight);
		//this->handleMouse((mouseEvent->pos()).x(), (mouseEvent->pos()).y(), windowWidth, windowHeight);
		//std::cout << (mouseEvent->pos()).x() << ", " << mouseEvent->x() << std::endl;
		//std::cout << (mouseEvent->pos()).y() << ", " << mouseEvent->y() << std::endl;
		return true;
	}
	default:
		return false;
	}
}

//bool EventHandler::event(QEvent* event, int windowWidth, int windowHeight) {
//    switch (event->type())
//    {
//    case QEvent::MouseMove:
//    {
//        QMouseEvent* mouseEvent = (static_cast<QMouseEvent*>(event));
//		mouseX = mouseEvent->pos().x();
//		mouseY = mouseEvent->pos().y();
//        //this->handleMouse(mouseEvent->x(), mouseEvent->y(), windowWidth, windowHeight);
//        //this->handleMouse((mouseEvent->pos()).x(), (mouseEvent->pos()).y(), windowWidth, windowHeight);
//		std::cout << (mouseEvent->pos()).x() << ", " << mouseEvent->x() << std::endl;
//		std::cout << (mouseEvent->pos()).y() << ", " << mouseEvent->y() << std::endl;
//        return true;
//    }
//    default:
//        return false;
//    }
//}

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

void EventHandler::handleMouse(int windowWidth, int windowHeight) {
	if (checkFirstMove())
	{
		mouseX = windowWidth / 2.0;
		mouseY = windowHeight / 2.0;
	}
	if (mousePressed)
	{
		//std::cout << "x: " << (float)(mouseY - (windowHeight / 2.0)) / windowHeight << ", mouseY: " << (float)(mouseX - (windowWidth / 2.0)) / windowWidth << std::endl;

		//std::cout << "cursor mouseX: " << mouseX << ", center mouseX: " << windowWidth / 2.0 << std::endl;
		camera->rotate((float)(mouseY - (windowHeight / 2.0)) / windowHeight, (float)(mouseX - (windowWidth / 2.0)) / windowWidth);
		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
	}
}

bool EventHandler::checkFirstMove()
{
	if (firstMove == true)
	{
		firstMove = false;
		return true;
	}
	return false;
}

bool EventHandler::isMousePressed()
{
	return mousePressed;
}