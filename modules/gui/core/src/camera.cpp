#pragma once

// project
#include "camera.h"

// third party

// std

void gui::camera::mouse_press(QMouseEvent* event)
{
    if (!(event->buttons() & Qt::LeftButton)) 
        return;
    
    m_last_mouse_pos = event->pos();
}

void gui::camera::mouse_move(QMouseEvent* event)
{
    if (!(event->buttons() & Qt::LeftButton)) 
        return;

    const float sensitivity = 0.5f;
    const QPoint pos_delta = event->pos() - m_last_mouse_pos;
    
    const float x_angle = pos_delta.y() * sensitivity * M_PI / 180.0f;
    const Eigen::Quaternionf x_rotation(Eigen::AngleAxisf(x_angle, Eigen::Vector3f::UnitX()));
    
    const float y_angle = pos_delta.x() * sensitivity * M_PI / 180.0f;
    const Eigen::Quaternionf y_rotation(Eigen::AngleAxisf(y_angle, Eigen::Vector3f::UnitY()));
    
    m_rotation = y_rotation * x_rotation * m_rotation;
    m_rotation.normalize();
    
    m_last_mouse_pos = event->pos();
}

void gui::camera::mouse_wheel(QWheelEvent* event)
{
    const float sensitivity = 0.001f;
    const float zoom_delta = event->angleDelta().y() * sensitivity;
    m_zoom = std::max(0.5f, std::min(10.0f, m_zoom - zoom_delta));
    
    // TODO: set zoom bounds
    // std::cout << m_zoom << std::endl;
}

void gui::camera::key_press(QKeyEvent* event)
{

}

void gui::camera::window_resized(int width, int height)
{
    m_aspect_ratio = (width > 0) ? (static_cast<float>(width) / static_cast<float>(height)) : 1.0f;
}

Eigen::Matrix4f gui::camera::get_projection_matrix()
{
    const float fov = 45.0f;
    const float f = 1.0f / std::tan(fov * 0.5f * M_PI / 180.0f);
    const float z_near = 0.1f;
    const float z_far = 100.0f;

    Eigen::Matrix4f projection = Eigen::Matrix4f::Zero();    
    projection(0,0) = f / m_aspect_ratio;
    projection(1,1) = f;
    projection(2,2) = (z_far + z_near) / (z_near - z_far);
    projection(3,2) = -1.0f;
    projection(2,3) = (2.0f * z_far * z_near) / (z_near - z_far);

    return projection;
}

Eigen::Matrix4f gui::camera::get_view_matrix()
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();
    view(2, 3) = -m_zoom;

    return view;
}

Eigen::Matrix4f gui::camera::get_model_matrix()
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();
    Eigen::Matrix3f mouse_rotation = m_rotation.toRotationMatrix();
    model.block<3,3>(0,0) = mouse_rotation;

    return model;
}
