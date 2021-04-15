#ifndef GUI_MENU_BAR_HPP
#define GUI_MENU_BAR_HPP

#include <QMainWindow>
#include <QMenuBar>

#include <filesystem>

#include "../model.hpp"
#include "window.hpp"


namespace bgl {

class MenuBar : public QMenuBar {
 public:
    MenuBar(Window &window);
    virtual ~MenuBar() = default;

 protected:
    virtual void onLoadModel(const std::shared_ptr<bgl::Model> &model);

 private:
    void loadModel() noexcept;

    Window &_window;
};

}  // namespace bgl

#endif  // GUI_MENU_BAR_HPP
