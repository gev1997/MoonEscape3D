#pragma once

// project
#include "namespace_gui.h"
#include "ball.h"
#include "shader_program.h"
#include "utils.h"

// third party

// std

class gui::mesh::moon : public gui::mesh::ball
{
public:
    moon() = default;
    virtual ~moon() = default;

    void draw(std::unique_ptr<gui::shader_program>& shader_prog, const Eigen::Matrix4f& transform) override;
};
