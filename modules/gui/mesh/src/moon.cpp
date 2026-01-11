#pragma once

// project
#include "moon.h"

// third party
#include <QOpenGLFunctions>
#include <Eigen/Geometry>

// std

gui::mesh::axis::axis()
{
    m_vertices = std::vector<GLfloat>{
    // ================= AXES =================
    // X axis (red)
    0.f, 0.f, 0.f,   1.f, 0.f, 0.f,
    2.f, 0.f, 0.f,   1.f, 0.f, 0.f,

    // Y axis (green)
    0.f, 0.f, 0.f,   0.f, 1.f, 0.f,
    0.f, 2.f, 0.f,   0.f, 1.f, 0.f,

    // Z axis (blue)
    0.f, 0.f, 0.f,   0.f, 0.f, 1.f,
    0.f, 0.f, 2.f,   0.f, 0.f, 1.f,

    // ================= LETTER X =================
    // X at (2.1, 0, 0)
    2.1f,  0.1f, 0.f,   1.f, 0.f, 0.f,
    2.3f, -0.1f, 0.f,   1.f, 0.f, 0.f,

    2.1f, -0.1f, 0.f,   1.f, 0.f, 0.f,
    2.3f,  0.1f, 0.f,   1.f, 0.f, 0.f,

    // ================= LETTER Y =================
    // Y at (0, 2.1, 0)
    -0.1f, 2.3f, 0.f,   0.f, 1.f, 0.f,
     0.f,  2.1f, 0.f,   0.f, 1.f, 0.f,

     0.1f, 2.3f, 0.f,   0.f, 1.f, 0.f,
     0.f,  2.1f, 0.f,   0.f, 1.f, 0.f,

     0.f,  2.1f, 0.f,   0.f, 1.f, 0.f,
     0.f,  1.9f, 0.f,   0.f, 1.f, 0.f,

    // ================= LETTER Z =================
    // Z at (0, 0, 2.1)
    -0.1f,  0.1f, 2.1f,   0.f, 0.f, 1.f,
     0.1f,  0.1f, 2.1f,   0.f, 0.f, 1.f,

     0.1f,  0.1f, 2.1f,   0.f, 0.f, 1.f,
    -0.1f, -0.1f, 2.1f,   0.f, 0.f, 1.f,

    -0.1f, -0.1f, 2.1f,   0.f, 0.f, 1.f,
     0.1f, -0.1f, 2.1f,   0.f, 0.f, 1.f,
};

}

void gui::mesh::axis::draw(std::unique_ptr<gui::shader_program>& shader_prog, const Eigen::Matrix4f& transform)
{
    QOpenGLVertexArrayObject::Binder binder(&m_VAO);

    shader_prog->setUniformValue("model", utils::eigen_to_qmatrix(transform));

    QOpenGLFunctions* gl_functions = QOpenGLContext::currentContext()->functions();
    gl_functions->glLineWidth(2.0f);
    // gl_functions->glDisable(GL_DEPTH_TEST);
    gl_functions->glDrawArrays(GL_LINES, 0, m_vertices.size() / 6);
    // gl_functions->glEnable(GL_DEPTH_TEST);
}

/////////////////////////////////////////
gui::mesh::maze_path::maze_path()
{
    init_maze();
    generate_dfs_maze();
    build_spherical_maze_mesh_option1();
    // build_spherical_maze_mesh_optionB();
}

void gui::mesh::maze_path::draw(std::unique_ptr<gui::shader_program>& shader_prog, const Eigen::Matrix4f& transform)
{
    QOpenGLVertexArrayObject::Binder binder(&m_VAO);

    shader_prog->setUniformValue("objectColor", 0.4f, 0.4f, 0.4f);
    shader_prog->setUniformValue("model", utils::eigen_to_qmatrix(transform));

    QOpenGLFunctions* gl_functions = QOpenGLContext::currentContext()->functions();
    gl_functions->glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
}

void gui::mesh::moon::draw(std::unique_ptr<gui::shader_program>& shader_prog, const Eigen::Matrix4f& transform)
{
    QOpenGLVertexArrayObject::Binder binder(&m_VAO);

    const Eigen::Matrix4f model_matrix = transform;
    shader_prog->setUniformValue("model", utils::eigen_to_qmatrix(model_matrix));

    QOpenGLFunctions* gl_functions = QOpenGLContext::currentContext()->functions();
    gl_functions->glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
}
