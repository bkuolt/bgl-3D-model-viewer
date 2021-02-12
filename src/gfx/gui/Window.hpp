// Copyright 2021 Bastian Kuolt
#ifndef GUI_WINDOW_HPP_
#define GUI_WINDOW_HPP_

#include <QMainWindow>
#include "GLViewport.hpp"


namespace bgl {

class Window : public QMainWindow {
 public:
	Window();
	bool event(QEvent *event) override;

 private:
	GLViewport _viewport;
	// TODO(bkuolt): handle events
};

}  // namespace bgl
#endif  // GUI_WINDOW_HPP_
