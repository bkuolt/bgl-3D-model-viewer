#ifndef GUI_HPP_
#define GUI_HPP_

/* --------------------------------- Config -------------------------------------------- */
#define DEBUG           // TODO
#undef USE_CONCURRENCY  // TODO: Cannot make QOpenGLContext current in a different thread
/* ------------------------------------------------------------------------------------- */

#include <QApplication>
#include <QMessageBox>
#include <QVersionNumber>

#include <iostream>
#include <string>

#include "OpenGL.hpp"
#include "Window.hpp"
#include "Scene.hpp"


namespace bgl {

using App = QApplication;

// TODO: QSize GetScreenSize() 
// TOOD: std::optional<std::filesystem::path> ChooseFile() 


namespace {  // NOLINT: Do not use unnamed namespaces in header files

inline void prompt_error_message(const std::exception &error) noexcept {
    QMessageBox::critical(nullptr, "Error", error.what());
}

inline void prompt_error_message(const std::string &error) noexcept {
    QMessageBox::critical(nullptr, "Error", error.c_str());
}

void print_debug_info() {
    std::cout << "Using" << std::endl
                << "\t" << "QT: " << QVersionNumber::fromString(QT_VERSION_STR).toString().toStdString() << std::endl
                << "\t" << "OpenGL: " << glGetString(GL_VERSION) << std::endl
                << "\t" << "GLEW: " << glewGetString(GLEW_VERSION) << std::endl;
}

}  // anonymous namespace
}  // namespace bgl

#endif  // GUI_HPP_
