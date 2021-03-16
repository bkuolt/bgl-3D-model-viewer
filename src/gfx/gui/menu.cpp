#include "menu.hpp"

#include <QFileDialog>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>

#include <optional>

namespace bgl {

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------

namespace {
namespace actions {

inline std::optional<std:filesystem::path> chooseFile() {
    const QString fileName { QFileDialog::getOpenFileName(nullptr, "Load 3D Model", "", "All Files (*)") };
    return fileName.isEmpty() ? {} : fileName.toStdString();
}

/**
 * @brief 
 * 
 * @param progressBar 
 * @return std::shared_ptr<Model> 
 */
std::optional<Model> load3DModel(QProgressBar &progressBar) {
    std::optional<std:filesystem::path> path { chooseFile() };
    if (!path.has_value()) {
        return;  // there was no file chosen
    }
    // TODO: check whether file changed -> avoid unnecessary reload

    progressBar.show();
    progressBar.setValue(0);

    try {
        model = bgl::io::Load3DModel(path);
    } catch (std::exception &exception) {
        QMessageBox::critical(nullptr, "Error", exception.what());
        progressBar.reset();
        progressBar.hide();
        return {};
    }

    // TODO: update model statistics panel
    progressBar.setValue(100);
    progressBar.hide();
    return model;
}


void showInfoBox() {
    // TODO
}


#if 1
void main() {
    // TODO program flow
}
#endif  // 1

}  // namespace actions
}  // anonymous namespace

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------


void MenuBar::loadModel() noexcept 
{}

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
