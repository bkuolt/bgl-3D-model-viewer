#include "gui/menu.hpp"

#include <QFileDialog>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QProgressBar>
#include <filesystem>
#include <memory>
#include <optional>

#include "gfx/gl.hpp"
#include "gfx/importer.hpp"
#include "gfx/model.hpp"

namespace {

inline std::optional<std::filesystem::path> chooseFile() {
    const QString fileName{QFileDialog::getOpenFileName(
        nullptr, "Load 3D Model", "", "3D Models (*.obj *.ply *.fbx);;All Files (*)")};
    return fileName.isEmpty() ? std::nullopt
                              : std::make_optional(std::filesystem::path{fileName.toStdString()});
}

void load3DModel() {
    std::optional<std::filesystem::path> path{chooseFile()};
    if (!path.has_value()) {
        QMessageBox::information(nullptr, "Notice", "No 3D model file selected.");
        return;
    }

    QProgressBar progressBar;
    progressBar.show();
    progressBar.setValue(0);

    std::shared_ptr<bgl::Model> model;
    try {
        model = bgl::LoadModel(path.value());
    } catch (const std::exception& exception) {
        QMessageBox::critical(nullptr, "Error Loading Model", exception.what());
        progressBar.reset();
        progressBar.hide();
        return;
    }

    progressBar.setValue(100);
    progressBar.hide();
}

inline void showAboutBox() {
    QMessageBox::about(nullptr, "About BGL 3D Model Viewer",
                       "BGL 3D Model Viewer\nA modern C++23 & OpenGL 3D Model Viewer.");
}

}  // anonymous namespace

namespace bgl {

MenuBar::MenuBar(QMainWindow& window) : _window{window} {
    QMenu* const fileMenu{this->addMenu("&File")};
    fileMenu->addAction("Load Model...", &load3DModel);
    fileMenu->addAction("Exit", [this]() { _window.close(); });

    QMenu* const helpMenu{this->addMenu("&Help")};
    helpMenu->addAction("About", &showAboutBox);
}

void MenuBar::loadModel() noexcept {
    load3DModel();
}

void MenuBar::onLoadModel(const std::filesystem::path& /*path*/) {
    QMessageBox::warning(nullptr, "Warning", "Feature not implemented yet.");
}

}  // namespace bgl
