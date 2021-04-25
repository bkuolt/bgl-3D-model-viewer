/**
 * @file Window.hpp
 * @brief 
 */
#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <QMainWindow>
#include <QFuture>

#include <filesystem>
#include <iostream>
#include <memory>

class Scene;
class QOpenGLWidget;


namespace bgl {

class Scene;

namespace gui {

class Window : public QMainWindow {
	Q_OBJECT
 public:
	Window();
	explicit Window(const QSize &size, const QString &title = "");
	virtual ~Window() noexcept = default;

	void setViewport(QOpenGLWidget *viewport);
	QOpenGLWidget* viewport() noexcept;

	void setScene(const std::shared_ptr<bgl::Scene> &scene) noexcept;
	std::shared_ptr<Scene> getScene() noexcept;

  // actions:
	void loadModel(const std::filesystem::path &path) noexcept;
	void loadModel() noexcept;

 protected:
	void keyPressEvent(QKeyEvent *event);
	void mouseMoveEvent(QMouseEvent *event) override;
	void closeEvent(QCloseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;

	QFuture<void> modelLoadingTask;
};

}  // namespace gui
}  // namespace bgl

#endif  // WINDOW_HPP_
