#pragma once

// project
#include "namespace_gui.h"
#include "ball.h"
#include "shader_program.h"
#include "utils.h"

// third party

// std

class gui::mesh::player_ball : public gui::mesh::ball
{
public:
    player_ball() = default;
    virtual ~player_ball() = default;

    void draw(std::unique_ptr<gui::shader_program>& shader_prog, const Eigen::Matrix4f& transform) override;

    public:
    Eigen::Vector3f m_angular_velocity;  // Rolling rotation speed
    Eigen::Vector3f m_rotation_axis;     // Current rotation axis
    float m_roll_angle = 0.0f;           // Current roll angle
    
    void update(float delta_time, const Eigen::Vector3f& movement_direction);
    // ... existing members ...
    float m_rotation_angle_x = 0.0f;  // Rotation around X axis
    float m_rotation_angle_y = 0.0f;  // Rotation around Y axis
    float m_orbit_radius = 1.1f;      // Distance from big ball (1.0 + 0.2 gap)
    
    void set_rotation(float angle_x, float angle_y);
    void add_rotation(float delta_x, float delta_y);
};
