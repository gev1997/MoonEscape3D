#pragma once

// project
#include "namespace_gui.h"

// third party
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <Eigen/Core>

// std

class gui::mesh::object_base
{
public:
    object_base() = default;
    virtual ~object_base() = default;

    void init();
    void release();
    virtual void draw(std::unique_ptr<shader_program>& shader_prog, const Eigen::Matrix4f& transform) = 0;

protected:
    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_VBO{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer m_EBO{QOpenGLBuffer::IndexBuffer};
    std::vector<GLfloat> m_vertices;
    std::vector<GLuint> m_indices;
};
