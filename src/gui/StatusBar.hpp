#ifndef STATUS_BAR_HPP_
#define STATUS_BAR_HPP_

#include <QStatusBar>

class QProgressBar;


namespace bgl {
namespace gui {

/**
 * @brief A Simple Status Bar used to indicate the progress of 
 *        the image loading process.
 */
class StatusBar : public QStatusBar {
	Q_OBJECT
 public:
   explicit StatusBar(QWidget *parent);
	virtual ~StatusBar() noexcept;

    QProgressBar* getProgressBar() noexcept;

 public slots:

    /**
     * @brief Updates the progress bar to given progress.
     * @param progress The progress in percent.
     */
    void updateProgress(unsigned int progress) noexcept;

 private:
    QProgressBar * const _progressBar {};
};

}  // namespace gui
}  // namespace bgl

#endif  // STATUS_BAR_HPP_
