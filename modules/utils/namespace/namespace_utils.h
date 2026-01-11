#pragma once

// project

// third party
#include <QMatrix4x4>
#include <QOpenGLWidget>
#include <Eigen/Core>

// std

namespace utils
{

QMatrix4x4 eigen_to_qmatrix(const Eigen::Matrix4f&);
void vertex_push_back(std::vector<GLfloat>&, const Eigen::Vector3f&);

}; // namespace utils
