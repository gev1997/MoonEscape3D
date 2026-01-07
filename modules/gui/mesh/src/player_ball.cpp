#pragma once

// project
#include "player_ball.h"

// third party
#include <QOpenGLFunctions>

// std

void gui::mesh::player_ball::draw(std::unique_ptr<gui::shader_program>& shader_prog, const Eigen::Matrix4f& transform)
{
    QOpenGLVertexArrayObject::Binder binder(&m_VAO);

    auto model_matrix = transform;        
    model_matrix(0, 3) += 1.0f;
    shader_prog->setUniformValue("model", utils::eigen_to_qmatrix(model_matrix));

    QOpenGLFunctions* gl_functions = QOpenGLContext::currentContext()->functions();
    gl_functions->glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
}
