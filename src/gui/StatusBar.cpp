#include <iostream>
#include <algorithm>  // std::min()
#include <sstream>    // std::ostringstream

#include "StatusBar.hpp"

#include <QStatusBar>
#include <QProgressBar>
#include <QMainWindow>



namespace bgl {
namespace gui {

StatusBar::StatusBar(QWidget *parent)
    : QStatusBar { parent }, _progressBar { new QProgressBar } {
}

StatusBar::~StatusBar() noexcept {
    delete _progressBar;
}

QProgressBar* StatusBar::getProgressBar() noexcept {
    assert(_progressBar != nullptr);
    return _progressBar;
}

void StatusBar::updateProgress(unsigned int progress) noexcept {
    progress = std::min(progress, 100u);  // keep progress within [0 ; 100]

    if (progress == 0) {
        this->addWidget(_progressBar);
        this->show();
    } else if (progress == 100) {
        this->removeWidget(_progressBar);
        this->showMessage("Done");
    } else {
        QMainWindow * const window { reinterpret_cast<QMainWindow*>(parentWidget()) };

        std::ostringstream oss;
        oss << progress << " %" << std::endl;
        window->setWindowTitle(oss.str().c_str());

        _progressBar->setValue(progress);
    }

    std::cout << "updated status bar to " << progress   << std::endl;
}

}  // namespace gui
}  // namespace bgl
