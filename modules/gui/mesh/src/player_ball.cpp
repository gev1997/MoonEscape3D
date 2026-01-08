#pragma once

// project
#include "player_ball.h"

// third party
#include <QOpenGLFunctions>

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

    // Current position
    float x_pos = m_orbit_radius * cos(m_rotation_angle_y) * sin(m_rotation_angle_x);
    float y_pos = m_orbit_radius * sin(m_rotation_angle_y);
    float z_pos = m_orbit_radius * cos(m_rotation_angle_y) * cos(m_rotation_angle_x);
    
    Eigen::Vector3f current_position(x_pos, y_pos, z_pos);
    
    // Track angular velocity for smooth rolling
    static Eigen::Vector3f angular_velocity = Eigen::Vector3f::Zero();
    static float roll_angle = 0.0f;
    static Eigen::Vector3f prev_position = current_position;
    
    // Calculate velocity
    Eigen::Vector3f velocity = current_position - prev_position;
    
    // Create local transform
    Eigen::Matrix4f local_transform = Eigen::Matrix4f::Identity();
    
    // Scale
    local_transform(0, 0) = 0.1f;
    local_transform(1, 1) = 0.1f;
    local_transform(2, 2) = 0.1f;
    
    // Position
    local_transform(0, 3) = x_pos;
    local_transform(1, 3) = y_pos;
    local_transform(2, 3) = z_pos;
    
    // Calculate rolling if moving
    if (velocity.norm() > 0.001f) {
        // Surface normal (points from big ball center to current position)
        Eigen::Vector3f surface_normal = current_position.normalized();
        
        // Rolling axis is perpendicular to both velocity and surface normal
        Eigen::Vector3f roll_axis = surface_normal.cross(velocity);
        
        if (roll_axis.norm() > 0.001f) {
            roll_axis.normalize();
            
            // Angular speed = linear speed / radius
            float linear_speed = velocity.norm();
            float angular_speed = linear_speed * 1.0f;  // radius = 0.1
            
            // Update angular velocity
            angular_velocity = roll_axis * angular_speed;
            
            // Accumulate roll angle
            roll_angle += angular_speed;
            
            // Keep roll angle in reasonable range
            if (roll_angle > 2 * M_PI) roll_angle -= 2 * M_PI;
            
            // Create rolling rotation
            Eigen::Matrix3f roll_rotation = Eigen::AngleAxisf(roll_angle, roll_axis).toRotationMatrix();
            
            // Apply mouse look rotation
            Eigen::Matrix3f look_rotation = 
                Eigen::AngleAxisf(m_rotation_angle_y, Eigen::Vector3f::UnitY()).toRotationMatrix() *
                Eigen::AngleAxisf(m_rotation_angle_x, Eigen::Vector3f::UnitX()).toRotationMatrix();
            
            // Combine
            Eigen::Matrix3f combined_rotation = look_rotation * roll_rotation;
            local_transform.block<3, 3>(0, 0) = combined_rotation * local_transform.block<3, 3>(0, 0);
        }
    } else {
        // Slow down rolling when not moving
        angular_velocity *= 0.95f;
        if (angular_velocity.norm() > 0.001f) {
            roll_angle += angular_velocity.norm();
            Eigen::Vector3f axis = angular_velocity.normalized();
            Eigen::Matrix3f roll_rotation = Eigen::AngleAxisf(roll_angle, axis).toRotationMatrix();
            
            Eigen::Matrix3f look_rotation = 
                Eigen::AngleAxisf(m_rotation_angle_y, Eigen::Vector3f::UnitY()).toRotationMatrix() *
                Eigen::AngleAxisf(m_rotation_angle_x, Eigen::Vector3f::UnitX()).toRotationMatrix();
            
            Eigen::Matrix3f combined_rotation = look_rotation * roll_rotation;
            local_transform.block<3, 3>(0, 0) = combined_rotation * local_transform.block<3, 3>(0, 0);
        }
    }
    
    // Update previous position
    prev_position = current_position;
    
    // Final transform
    Eigen::Matrix4f model_matrix = transform * local_transform;
    shader_prog->setUniformValue("model", utils::eigen_to_qmatrix(model_matrix));

    QOpenGLFunctions* gl_functions = QOpenGLContext::currentContext()->functions();
    gl_functions->glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
}