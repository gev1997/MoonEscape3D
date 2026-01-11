#pragma once

// project
#include "player_ball.h"

// third party
#include <QOpenGLFunctions>
#include <Eigen/Geometry>

// std

void gui::mesh::player_ball::draw(std::unique_ptr<gui::shader_program>& shader_prog, const Eigen::Matrix4f& transform)
{
    QOpenGLVertexArrayObject::Binder binder(&m_VAO);
    
    Eigen::Affine3f model = Eigen::Affine3f::Identity();
    model.translate(Eigen::Vector3f(1.1f, 1.1f, 1.1f));
    model.scale(0.1f);

    Eigen::Matrix4f model_matrix = model.matrix();

    shader_prog->setUniformValue("model", utils::eigen_to_qmatrix(model_matrix));

    QOpenGLFunctions* gl_functions = QOpenGLContext::currentContext()->functions();
    gl_functions->glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
}
