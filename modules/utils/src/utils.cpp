#pragma once

// project
#include "utils.h"

// third party

// std

QMatrix4x4 utils::eigen_to_qmatrix(const Eigen::Matrix4f& matrix)
{
    QMatrix4x4 result;
    std::copy(matrix.data(), matrix.data() + 16, result.data());
    return result;
}
