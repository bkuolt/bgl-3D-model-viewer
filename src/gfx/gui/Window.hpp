// Copyright 2021 Bastian Kuolt
#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <QMainWindow>
#include <iostream>

#include "GLViewport.hpp"


namespace bgl {

class Window : public QMainWindow {
 public:
    Window()
        : _viewport { this } {
        this->setWindowTitle("BGL Demo");
        this->setFixedSize(1200, 800);

        // creates GL context
        _viewport.resize(1200, 800);
        _viewport.show();
        this->setCentralWidget(&_viewport);

        this->show();
    }

   bool event(QEvent *event) override {
      if (event->type() == QEvent::KeyPress) {
         std::cout << "key pressed" << std::endl;
         return true;
      }

      return false;  // TODO(bkuolt): implement
   }

 private:
    GLViewport _viewport;
     // TODO(bkuolt): handle events
};

}  // namespace bgl
#endif  // WINDOW_HPP_
