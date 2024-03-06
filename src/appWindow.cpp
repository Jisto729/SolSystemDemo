#include "appWindow.h"
#include <SolDemoWindow.h>

using namespace ssd;

AppWindow::AppWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Create the button, make "this" the parent
    // set size and location of the button
    //generateButton->setGeometry(QRect(QPoint(100, 100), QSize(200, 50)));

    // Connect button signal to appropriate slot
    //connect(generateButton, &QPushButton::released, this, &AppWindow::handleButton);
    dockWidget = new QDockWidget(tr("Generation Settings"), this);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
        Qt::RightDockWidgetArea);
   
    container = new QWidget();
    dockWidget->setWidget(container);

    layout = new QVBoxLayout(container);

    sunTempLabel = new QLabel("10000");
    layout->addWidget(sunTempLabel);
    sunTempSlider = new QSlider(Qt::Horizontal);
    sunTempSlider->setMaximum(40000);
    sunTempSlider->setMinimum(5000);
    sunTempSlider->setTickPosition(QSlider::TicksBothSides);
    sunTempSlider->setTickInterval(5000);
    sunTempSlider->setSliderPosition(10000);
    layout->addWidget(sunTempSlider);
    connect(sunTempSlider, &QSlider::valueChanged, this, &AppWindow::handleSlider);
    generateButton = new QPushButton("Generate");
    layout->addWidget(generateButton);
    connect(generateButton, &QPushButton::released, this, &AppWindow::handleButton);
    addDockWidget(Qt::LeftDockWidgetArea, dockWidget);

    renderWindow = new ssd::SolDemoWindow(this);
    renderWindow->setFocusPolicy(Qt::StrongFocus);
    renderWindow->setFocus();

    setCentralWidget(renderWindow);

    
    std::cout << "done" << std::endl;
}

void AppWindow::handleButton()
{
    // change the text
    std::cout << "button pressed" << sunTempSlider->value() << std::endl;
    attribs.sunTemperature = sunTempSlider->value();
    renderWindow->generate(attribs);
    // resize button
    //generateButton->resize(100, 100);
}

void AppWindow::handleSlider()
{
    sunTempLabel->setText(QString::number(sunTempSlider->value()));
    std::cout << "slider moved" << sunTempSlider->value() << std::endl;
}


AppWindow::~AppWindow()
{
}