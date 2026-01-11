#pragma once

// project
#include "namespace_gui.h"

// third party

// std
#include <memory>

enum class gui::mesh::object_type
{
    PlayerBall,
    Moon,
    Axis,
    MazePath
};

struct gui::mesh::factory
{
    static std::unique_ptr<object_base> create(object_type type);
};
