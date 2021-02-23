#ifndef GFX_QUAD_HPP_
#define GFX_QUAD_HPP_

#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>


namespace bgl {

void DrawQuad(GLuint textureID) {
    constexpr GLuint indices[] { 3, 2, 1, 0};
    constexpr QVector2D vertices[] {
        { -1.0, -1.0 },
        { -1.0,  1.0 },
        {  1.0,  1.0 },
        {  1.0, -1.0 }
    };

    static QOpenGLShaderProgram program;
    static QOpenGLBuffer ibo(QOpenGLBuffer::Type::IndexBuffer);
    static QOpenGLBuffer vbo(QOpenGLBuffer::Type::VertexBuffer);
    static QOpenGLVertexArrayObject vao;
    static bool initialized { false };

    if (!initialized) {
        program.create();
        program.addShaderFromSourceFile(QOpenGLShader::Vertex, "./gfx/shaders/quad.vs");
        program.addShaderFromSourceFile(QOpenGLShader::Fragment, "./gfx/shaders/quad.fs");
        if (!program.link()) {
            throw std::runtime_error { "could not link QOpenGLShaderProgram" };
        }

        if (!ibo.create() || !vbo.create() || !vao.create()) {
            throw std::runtime_error { "could not create QOpenGLBuffer" };
        }

        vao.bind();
        vbo.bind();
        ibo.bind();
        vbo.allocate(&vertices, sizeof(vertices));
        ibo.allocate(&indices, sizeof(indices));
 
        const int location { program.attributeLocation("vertex") };
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 0 /* stride*/, 0 /** offset*/);

        initialized = true;
    }

    const int textureUnit { 0 };
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    program.bind();
    program.setUniformValue("texture", textureUnit);

    vao.bind();
    vbo.bind();
    ibo.bind();
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, nullptr);  // Warning: GL_QUADS is  deprecated
    if (glGetError() != GL_NO_ERROR) {
        throw std::runtime_error { "glDrawElements() failed" };
    }

    program.release();
    vao.release();
    vbo.release();
    ibo.release();
}

}  // namespace bgl

#endif  // GFX_QUAD_HPP_
