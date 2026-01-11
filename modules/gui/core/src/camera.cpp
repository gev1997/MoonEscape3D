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
    const float angleStep = 0.05f; // ~3 degrees
    Eigen::AngleAxisf rotation;
    Eigen::Vector3f right = m_rotation * Eigen::Vector3f::UnitX();
    
    switch (event->key())
    {
        case Qt::Key_Left:
            // rotate left around world up (Y)
            rotation = Eigen::AngleAxisf(angleStep, Eigen::Vector3f::UnitY());
            m_rotation = rotation * m_rotation;
            break;
        case Qt::Key_Right:
            // rotate right around world up (Y)
            rotation = Eigen::AngleAxisf(-angleStep, Eigen::Vector3f::UnitY());
            m_rotation = rotation * m_rotation;
            break;
        case Qt::Key_Up:
            // rotate up (pitch) around camera local right
            rotation = Eigen::AngleAxisf(-angleStep, right);
            m_rotation = rotation * m_rotation;
            break;
        case Qt::Key_Down:
            // rotate down (pitch) around camera local right            
            rotation = Eigen::AngleAxisf(angleStep, right);
            m_rotation = rotation * m_rotation;
            break;
    }
}

void gui::camera::window_resized(int width, int height)
{
    m_aspect_ratio = (width > 0) ? (static_cast<float>(width) / static_cast<float>(height)) : 1.0f;
}

Eigen::Matrix4f gui::camera::get_projection_matrix()
{
    const float fov = 45.0f;
    const float focal = 1.0f / std::tan(fov * 0.5f * M_PI / 180.0f);
    const float z_near = 0.1f;
    const float z_far = 100.0f;

    Eigen::Matrix4f projection = Eigen::Matrix4f::Zero();
    projection(0,0) = focal / m_aspect_ratio;
    projection(1,1) = focal;
    projection(2,2) = (z_far + z_near) / (z_near - z_far);
    projection(3,2) = -1.0f;
    projection(2,3) = (2.0f * z_far * z_near) / (z_near - z_far);

    return projection;
}

Eigen::Matrix4f gui::camera::get_view_matrix()
{
    Eigen::Vector3f center(0.f, 0.f, 0.f);
    Eigen::Vector3f up(0.f, 1.f, 0.f);
    Eigen::Vector3f eye = center + Eigen::Vector3f(m_zoom, m_zoom, m_zoom);

    Eigen::Vector3f f = (center - eye).normalized();
    Eigen::Vector3f s = f.cross(up).normalized();
    Eigen::Vector3f u = s.cross(f);

    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    view(0,0) = s.x(); view(0,1) = s.y(); view(0,2) = s.z(); view(0,3) = -s.dot(eye);
    view(1,0) = u.x(); view(1,1) = u.y(); view(1,2) = u.z(); view(1,3) = -u.dot(eye);
    view(2,0) = -f.x(); view(2,1) = -f.y(); view(2,2) = -f.z(); view(2,3) = f.dot(eye);

    return view;
}

Eigen::Matrix4f gui::camera::get_model_matrix()
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();
    Eigen::Matrix3f mouse_rotation = m_rotation.toRotationMatrix();
    model.block<3,3>(0,0) = mouse_rotation;

    return model;
}
