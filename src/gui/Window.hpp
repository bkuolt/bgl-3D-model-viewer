/**
 * @file Window.hpp
 * @brief 
 */
#ifndef WINDOW_HPP_
#define WINDOW_HPP_

/* --------------- Config --------------- */

#define DEBUG           // TODO
#undef USE_CONCURRENCY  // TODO: Cannot make QOpenGLContext current in a different thread

/* ---------------------------------------*/

#include <QMainWindow>
#include <QFuture>

#include <filesystem>
#include <iostream>
#include <memory>

class Scene;
class QOpenGLWidget;

#define DEBUG

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

	void setScene(const std::shared_ptr<Scene> &scene) noexcept;
	std::shared_ptr<Scene> getScene() noexcept;

  // actions:
#ifdef DEBUG
	void loadImage(const std::filesystem::path &path) noexcept;
	void loadImage() noexcept;
#else
	void loadModel(const std::filesystem::path &path) noexcept;
	void loadModel() noexcept;
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
