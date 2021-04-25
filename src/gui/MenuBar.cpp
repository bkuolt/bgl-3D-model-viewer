#include "MenuBar.hpp"

#include <QCoreApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QMainWindow>
#include <QStatusBar>

#include <iostream>
#include <filesystem>
#include <memory>
#include <chrono>

#include "Window.hpp"
#include "StatusBar.hpp"


namespace bgl {
namespace gui {

namespace {

struct Scene {};

inline void ShowAboutBox() noexcept {
    QMessageBox::about(nullptr, "About,", "A simple Qt demo.");
}

}  // anonymous namespace


MenuBar::MenuBar(Window *mainWindow)
    : QMenuBar { reinterpret_cast<QWidget*>(mainWindow) } {
    QMenu * const fileMenu { this->addMenu("&File") };
    fileMenu->addAction("Load", [this, mainWindow] () { mainWindow->loadImage(); });
    fileMenu->addAction("Exit", [this] () { QCoreApplication::quit(); });

    QMenu * const helpMenu { this->addMenu("&Help") };
    helpMenu->addAction("About", &ShowAboutBox);

    connect(this, &MenuBar::updateStatusBar,
            dynamic_cast<StatusBar*>(mainWindow->statusBar()), &StatusBar::updateProgress);
}

}  // namespace gui
}  // namespace bgl
