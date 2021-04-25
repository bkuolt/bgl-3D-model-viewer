#ifndef GUI_MENU_BAR_HPP
#define GUI_MENU_BAR_HPP

#include <QMainWindow>
#include <QMenuBar>

#include <filesystem>


namespace bgl {
namespace gui {

class MenuBar : public QMenuBar {
 public:
	explicit MenuBar(QMainWindow &window);  // NOLINT

	MenuBar(MenuBar&&) = default;
	MenuBar& operator=(MenuBar&&) = default;

	MenuBar(const MenuBar&) = delete;
	MenuBar& operator=(const MenuBar&) = delete;

	virtual ~MenuBar() noexcept = default;

 protected:
	virtual void onLoadModel(const std::filesystem::path &path);

	private:
	void loadModel() noexcept;

	QMainWindow &_window;
};

}  // namespace gui
}  // namespace bgl

#endif  // GUI_MENU_BAR_HPP
