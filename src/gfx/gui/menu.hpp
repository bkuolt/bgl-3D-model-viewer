#ifndef GUI_MENU_BAR_HPP
#define GUI_MENU_BAR_HPP

#include <QMainWindow>
#include <QMenuBar>

#include <filesystem>


namespace bgl {

class MenuBar : public QMenuBar {
 public:
    MenuBar(QMainWindow &window);
    virtual ~MenuBar() = default;

 protected:
    virtual void onLoadModel(const std::filesystem::path &path);

 private:
    void loadModel() noexcept;

    QMainWindow &_window;
};

}  // namespace bgl

#endif  // GUI_MENU_BAR_HPP
