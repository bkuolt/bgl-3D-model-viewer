#ifndef GUI_MENU_BAR_HPP
#define GUI_MENU_BAR_HPP

#include <QMenuBar>


namespace bgl {
namespace gui {

class Window;

class MenuBar : public QMenuBar {
	Q_OBJECT
 public:
	explicit MenuBar(Window *mainWindow);

	virtual ~MenuBar() noexcept = default;

 public: signals:
 	void updateStatusBar(unsigned int progress);

};

}  // namespace gui
}  // namespace bgl

#endif  // GUI_MENU_BAR_HPP
