#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>
#include <SolDemoWindow.h>

#include "GenAttributes.h"

namespace ssd
{
    class AppWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        AppWindow(QWidget* parent = nullptr);
        ~AppWindow();
    private slots:
        void handleButton();
        void handleSlider();
    private:
        QPushButton* generateButton;
        ssd::SolDemoWindow* renderWindow;
        QWidget* container;
        QDockWidget* dockWidget;
        QVBoxLayout* layout;
        QLabel* sunTempLabel;
        QSlider* sunTempSlider;
        GenAttributes attribs;
    };
}