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

void utils::vertex_push_back(std::vector<GLfloat>& vertices, const Eigen::Vector3f& vertex)
{
    const Eigen::Vector3f normal = vertex.normalized();
    vertices.insert(vertices.end(), {vertex.x(), vertex.y(), vertex.z()});
    vertices.insert(vertices.end(), {normal.x(), normal.y(), normal.z()});
};
