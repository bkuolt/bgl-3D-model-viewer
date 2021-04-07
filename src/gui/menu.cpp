#include <QFileDialog>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QProgressBar>

#include <optional>
#include <filesystem>
#include <memory>

#include "menu.hpp"
#include "../gfx/model.hpp"  // TODO


namespace {

inline std::optional<std::filesystem::path> chooseFile() {
    const QString fileName { QFileDialog::getOpenFileName(nullptr, "Load 3D Model", "", "All Files (*)") };
    return fileName.isEmpty() ? std::filesystem::path{} : std::filesystem::path { fileName.toStdString() };
}

/**
 * @brief Selects and loads a 3D model.
 */
void load3DModel() {
    std::optional<std::filesystem::path> path { chooseFile() };
    if (!path.has_value()) {
        QMessageBox::information(nullptr, "Warning", "No file chosen.");
        return;
    }

    QProgressBar progressBar;
    progressBar.show();
    progressBar.setValue(0);

    std::shared_ptr<bgl::Model> model;
    try {
        model = bgl::LoadModel(path.value());
        // TODO: update progress bar
    } catch (std::exception &exception) {
        QMessageBox::critical(nullptr, "Error", exception.what());
        progressBar.reset();
        progressBar.hide();
        return;
    }

    // TODO: update model statistics panel
    progressBar.setValue(100);
    progressBar.hide();
    // TODO: update model in viewport
}

inline void showAboutBox() {
    QMessageBox::about(nullptr, "About,", "A simple Qt OpenGL demo.");
}

}  // anonymous namespace

namespace bgl {

MenuBar::MenuBar(QMainWindow &window)
    : _window { window } {
    QMenu * const fileMenu { this->addMenu("&File") };
    fileMenu->addAction("Load", this, &load3DModel);
    fileMenu->addAction("Exit", [this] () { _window.close(); });

    QMenu * const helpMenu { this->addMenu("&Help") };
    helpMenu->addAction("About", &showAboutBox);
}

void MenuBar::onLoadModel(const std::filesystem::path &path) {
    QMessageBox::warning(nullptr, "Warning,", "Not implemented yet.");
}

}  // namespace bgl
