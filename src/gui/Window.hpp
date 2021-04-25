/**
 * @file Window.hpp
 * @brief 
 * 
 *  
 */
#ifndef WINDOW_HPP_
#define WINDOW_HPP_

// -----------------------------
#define DEBUG  // TODO
// -----------------------------

#include <QMainWindow>

#include <filesystem>
#include <memory>

#include "Scene.hpp"

class QOpenGLWidget;


namespace bgl {
namespace gui {

class Window : public QMainWindow {
	Q_OBJECT
 public:
	Window();
	explicit Window(const QSize &size, const QString &title = "");
	virtual ~Window() noexcept = default;

	void setViewport(QOpenGLWidget *viewport);
	QOpenGLWidget* viewport() noexcept;

	void setScene(const std::shared_ptr<Scene> &scene);
	// TODO: get scene

  // actions:
#if DEBUG
	void loadImage(const std::filesystem::path &path = "./example.png");
	void loadImage();
#else
	void loadModel(const std::filesystem::path &path)  { /* TODO */ }
	void loadModel() { /* TODO */ }
#endif  // DEBUG

 protected:
	void keyPressEvent(QKeyEvent *event);
	void mouseMoveEvent(QMouseEvent *event) override;
	void closeEvent(QCloseEvent *event) override;

	QFuture<void> _imageLoadingTask;
};

}  // namespace gui
}  // namespace bgl

#endif  // WINDOW_HPP_
