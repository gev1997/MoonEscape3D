#pragma once

// project

// third party
#include <QMatrix4x4>
#include <Eigen/Core>

// std

namespace utils
{

QMatrix4x4 eigen_to_qmatrix(const Eigen::Matrix4f&);

}; // namespace utils
