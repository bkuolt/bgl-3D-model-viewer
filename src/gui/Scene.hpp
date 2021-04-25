/**
 * @file Scene.hpp
 * @brief 
 * 
 *  
 */
#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <QImage>
#include <QOpenGLTexture>

namespace bgl {

class Scene {
 public:
    Scene() = default;
    virtual ~Scene() = default;

    void setBackground(const std::shared_ptr<QOpenGLTexture> &background);
    std::shared_ptr<QOpenGLTexture> getBackground() noexcept;

    virtual void render();

 private:
    std::shared_ptr<QOpenGLTexture> _background;
};

}  // namespace bgl

#endif // SCENE_HPP_
