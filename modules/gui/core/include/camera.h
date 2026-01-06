#pragma once

// project
#include "namespace_gui.h"

// third party
#include <QMouseEvent>
#include <Eigen/Core>
#include <Eigen/Geometry>

// std

class gui::camera
{
public:
    camera() = default;
    ~camera() = default;

    void mouse_press(QMouseEvent* event);
    void mouse_move(QMouseEvent* event);
    void mouse_wheel(QWheelEvent* event);
    void key_press(QKeyEvent* event);
    void window_resized(int width, int height);

    Eigen::Matrix4f get_projection_matrix();
    Eigen::Matrix4f get_view_matrix();
    Eigen::Matrix4f get_model_matrix();

private:
    float m_aspect_ratio = 1.0f;
    float m_zoom = 3.0f;
    QPoint m_last_mouse_pos;
    Eigen::Quaternionf m_rotation = Eigen::Quaternionf::Identity();
};
