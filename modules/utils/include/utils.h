#pragma once

// project
#include "namespace_utils.h"

// third party

// std

QMatrix4x4 utils::eigen_to_qmatrix(const Eigen::Matrix4f& matrix);
void utils::vertex_push_back(std::vector<GLfloat>& vertices, const Eigen::Vector3f& vertex);
