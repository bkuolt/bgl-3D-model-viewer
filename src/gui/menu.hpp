#pragma once

/**
 * @file menu.hpp
 * @brief Custom MenuBar for model viewer actions.
 */

#include <QMainWindow>
#include <QMenuBar>
#include <filesystem>

#include "gfx/gl.hpp"

namespace bgl {

class MenuBar : public QMenuBar {
  public:
    explicit MenuBar(QMainWindow& window);

    MenuBar(MenuBar&&) noexcept = default;
    MenuBar& operator=(MenuBar&&) noexcept = default;

    MenuBar(const MenuBar&) = delete;
    MenuBar& operator=(const MenuBar&) = delete;

    ~MenuBar() noexcept override = default;

  protected:
    virtual void onLoadModel(const std::filesystem::path& path);

  private:
    void loadModel() noexcept;

    QMainWindow& _window;
};

}  // namespace bgl
