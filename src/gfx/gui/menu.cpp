#include "../model.hpp"
#include "menu.hpp"

#include <QFileDialog>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QProgressBar>

#include <filesystem>
#include <string>
#include <optional>


namespace bgl {

namespace {

inline std::optional<std::filesystem::path> chooseFile() {
    const std::filesystem::path fileName {
        QFileDialog::getOpenFileName(nullptr, "Load 3D Model", "", "All Files (*)").toStdString()
    };
    return fileName.empty() ?  std::nullopt : std::optional<std::filesystem::path>(fileName);
}

inline void showInfoBox() {
    QMessageBox::about(nullptr, "About,", "A simple Qt OpenGL demo.");
}

}  // anonymous namespace

void MenuBar::loadModel() noexcept {
    _window._viewport->makeCurrent();  // <---

    QProgressBar progressBar;  // TODO

    std::optional<std::filesystem::path> path { chooseFile() };
    if (!path.has_value()) {
        return;  // no file chosen
    }

    progressBar.show();
    progressBar.setValue(0);
    std::shared_ptr<bgl::Model> model;

    try {
        model = LoadModel(path.value());
        // TODO(bkuolt) update model statistics panel
    } catch (std::exception &exception) {
        QMessageBox::critical(nullptr, "Error", exception.what());
        progressBar.reset();
        progressBar.hide();
        return;
    }

    onLoadModel(model);
    progressBar.setValue(100);
    progressBar.hide();
}

MenuBar::MenuBar(Window &window)
    : _window { window } {
    QMenu * const fileMenu { this->addMenu("&File") };
    fileMenu->addAction("Load", this, &MenuBar::loadModel);
    fileMenu->addAction("Exit", [this] () { _window.close(); });

    QMenu * const helpMenu { this->addMenu("&Help") };
    helpMenu->addAction("About", &showInfoBox);
}

void MenuBar::onLoadModel(const std::shared_ptr<bgl::Model> &model) {
    QMessageBox::warning(nullptr, "Warning,", "Not implemented yet.");
}

}  // namespace bgl
