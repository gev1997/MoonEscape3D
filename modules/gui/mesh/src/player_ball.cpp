#pragma once

// project
#include "player_ball.h"

// third party
#include <QOpenGLFunctions>
#include <Eigen/Geometry>  // For rotation classes

// std

#include <Eigen/Geometry>  // For rotation classes

void gui::mesh::player_ball::update(float delta_time, const Eigen::Vector3f& movement_direction)
{
    // Update rolling based on movement
    if (movement_direction.norm() > 0.01f) {
        // Calculate rotation axis (perpendicular to movement and up)
        Eigen::Vector3f up(0.0f, 1.0f, 0.0f);
        m_rotation_axis = movement_direction.cross(up).normalized();
        
        // Angular velocity proportional to movement speed
        float movement_speed = movement_direction.norm();
        float angular_speed = movement_speed / 0.1f; // Divide by radius (0.1)
        m_angular_velocity = m_rotation_axis * angular_speed;
        
        // Update roll angle
        m_roll_angle += m_angular_velocity.norm() * delta_time;
    } else {
        // Slow down when not moving
        m_angular_velocity *= 0.9f;
        m_roll_angle += m_angular_velocity.norm() * delta_time;
    }
}

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