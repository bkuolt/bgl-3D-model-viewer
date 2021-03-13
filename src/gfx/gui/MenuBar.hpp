// Copyright 2021 Bastian Kuolt
#ifndef GUI_MENU_BAR_HPP
#define GUI_MENU_BAR_HPP

#include <QDockWidget>
#include <QFileDialog>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>

#include <filesystem>


namespace bgl {

class MenuBar : public QMenuBar {
 public:
    MenuBar(QMainWindow &window);
    virtual ~MenuBar() = default;

    virtual void onLoadModel(const std::filesystem::path &path);

 private:
    void loadModel() noexcept;

    QMainWindow &_window;
};

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

MenuBar::loadModel() noexcept{
    const QString fileName { QFileDialog::getOpenFileName(nullptr, "Load 3D Model", "", "All Files (*)") };
    if (fileName.isEmpty()) {
        return;  // nothing to do
    }

    try {
        onLoadModel(fileName.toStdString());
    } catch (std::exception &exception) {
        QMessageBox::error(nullptr, "Error", exception.what());
    }
}

MenuBar::MenuBar(QMainWindow &window)
    : _window { window } {
    QMenu * const fileMenu { this->addMenu("&File") };
    fileMenu->addAction("Load", this, &loadModel);
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
    // TODO(kkuolt)
}

}  // namespace bgl

#endif  // GUI_MENU_BAR_HPP
