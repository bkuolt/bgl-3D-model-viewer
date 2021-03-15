#include "menu.hpp"

#include <QFileDialog>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>


namespace bgl {

void MenuBar::loadModel() noexcept {
    const QString fileName { QFileDialog::getOpenFileName(nullptr, "Load 3D Model", "", "All Files (*)") };
    if (fileName.isEmpty()) {
        return;  // nothing to do
    }

    try {
        onLoadModel(fileName.toStdString());
    } catch (std::exception &exception) {
        QMessageBox::critical(nullptr, "Error", exception.what());
    }
}

MenuBar::MenuBar(QMainWindow &window)
    : _window { window } {
    QMenu * const fileMenu { this->addMenu("&File") };
    fileMenu->addAction("Load", this, &MenuBar::loadModel);
    fileMenu->addAction("Exit", [this] () {
        _window.close();
    });

    QMenu * const helpMenu { this->addMenu("&Help") };
    helpMenu->addAction("About", [] () {
        QMessageBox::about(nullptr, "About,", "A simple Qt OpenGL demo.");
    });
}

void MenuBar::onLoadModel(const std::filesystem::path &path) {
    QMessageBox::warning(nullptr, "Warning,", "Not implemented yet.");
}

}  // namespace bgl
