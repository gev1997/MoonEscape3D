#pragma once

// project
#include "factory.h"
#include "player_ball.h"
#include "moon.h"
#include "axis.h"

// third party

// std

std::unique_ptr<gui::mesh::object_base> gui::mesh::factory::create(object_type type)
{
    switch (type)
    {
        case object_type::PlayerBall:
            return std::make_unique<player_ball>();
        case object_type::Moon:
            return std::make_unique<moon>();
        case object_type::Axis:
            return std::make_unique<axis>();
        default:
            break;
    }

    return nullptr;
}
