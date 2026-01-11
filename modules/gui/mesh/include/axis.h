#pragma once

// project
#include "namespace_gui.h"
#include "object_base.h"
#include "shader_program.h"
#include "utils.h"

// third party

// std

class gui::mesh::axis : public gui::mesh::object_base
{
public:
    axis();
    virtual ~axis() = default;

    void draw(std::unique_ptr<gui::shader_program>& shader_prog, const Eigen::Matrix4f& transform) override;
};
