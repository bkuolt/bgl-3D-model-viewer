#include <QStatusBar>
#include <QProgressBar>

#include <iostream>
#include <algorithm>
#include <sstream>
#include "StatusBar.hpp"


namespace bgl {
namespace gui {

StatusBar::StatusBar(QWidget *parent)
    : QStatusBar { parent }, _progressBar { new QProgressBar } {
    this->addWidget(_progressBar);
}

QProgressBar* StatusBar::getProgressBar() noexcept {
    return _progressBar;
}

void StatusBar::updateProgress(unsigned int progress) noexcept {
    progress = std::min(progress, 100);  // keep progress in [0 ; 100]

    std::stringstream oss;
    oss << progress << " %" << std::endl;

    if (progress == 100) {
        // TODO: Hide progress bar
        // TODO: Set status bar message to "Done."
    } else {
        QMainWindow * window const { reinterpret_cast<QMainWindow*>(parentWidget()) };
        window->setWindowTitle(oss.str().c_str());
        _progressBar->setValue(progress);
        std::cout << "updated status bar to " << progress   << std::endl;
    }
}

}  // namespace gui
}  // namespace bgl
