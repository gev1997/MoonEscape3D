#pragma once

// project
#include "namespace_gui.h"
#include "ball.h"
#include "shader_program.h"
#include "utils.h"

// third party
#include <Eigen/Core>
#include <Eigen/Geometry>

// std
#include <stack>
#include <random>
#include <algorithm>

class gui::mesh::axis : public gui::mesh::object_base
{
public:
    axis();
    virtual ~axis() = default;

    void draw(std::unique_ptr<gui::shader_program>& shader_prog, const Eigen::Matrix4f& transform) override;
};

class gui::mesh::maze_path : public gui::mesh::object_base
{
public:
    maze_path();
    virtual ~maze_path() = default;

    void draw(std::unique_ptr<gui::shader_program>& shader_prog, const Eigen::Matrix4f& transform) override;

    #define GRID 24         // maze resolution
    bool maze[GRID][GRID] = {};
    
    void init_maze()
    {
        for (int i = 0; i < GRID; ++i)
            for (int j = 0; j < GRID; ++j)
                maze[i][j] = true; // wall everywhere
    }

    void generate_dfs_maze(int sx = 1, int sy = 1)
{
    static const int dx[4] = { 0,  2,  0, -2 };
    static const int dy[4] = { -2, 0,  2,  0 };

    std::random_device rd;
    std::mt19937 rng(rd());

    std::stack<std::pair<int,int>> st;

    maze[sx][sy] = false; // start cell
    st.push({ sx, sy });

    while (!st.empty())
    {
        auto [x, y] = st.top();

        std::vector<int> dirs = { 0, 1, 2, 3 };
        std::shuffle(dirs.begin(), dirs.end(), rng);

        bool moved = false;

        for (int d : dirs)
        {
            int nx = x + dx[d];
            int ny = y + dy[d];

            if (nx <= 0 || ny <= 0 || nx >= GRID - 1 || ny >= GRID - 1)
                continue;

            if (maze[nx][ny]) // unvisited
            {
                // remove wall between
                maze[x + dx[d] / 2][y + dy[d] / 2] = false;
                maze[nx][ny] = false;

                st.push({ nx, ny });
                moved = true;
                break;
            }
        }

        if (!moved)
            st.pop();
    }
}

    void build_spherical_maze_mesh_option1()
{
const float sphere_radius = 1.0f;
const float wall_height   = 0.1f; // how far wall extrudes outward
    m_vertices.clear();
    m_indices.clear();

    // cell angles
    const float du = 2.0f * float(M_PI) / GRID;
    const float dv = float(M_PI) / GRID;

    auto push_vertex = [&](const Eigen::Vector3f& pos)
    {
        Eigen::Vector3f n = pos.normalized();
        m_vertices.push_back(pos.x());
        m_vertices.push_back(pos.y());
        m_vertices.push_back(pos.z());
        m_vertices.push_back(n.x());
        m_vertices.push_back(n.y());
        m_vertices.push_back(n.z());
    };

    GLuint base_index = 0;

    for (int i = 0; i < GRID; ++i)
    {
        for (int j = 0; j < GRID; ++j)
        {
            if (!maze[i][j])
                continue;

            // UV corners of this cell
            float theta0 = i * du;
            float theta1 = (i + 1) * du;
            float phi0   = j * dv;
            float phi1   = (j + 1) * dv;

            // Wall height along the normal
            float r0 = sphere_radius;
            float r1 = sphere_radius + wall_height;

            // 8 vertices of the curved wall (lower and upper)
            Eigen::Vector3f v[8];

            // Lower quad (on sphere surface)
            v[0] = Eigen::Vector3f(r0 * sin(phi0) * cos(theta0),
                                   r0 * cos(phi0),
                                   r0 * sin(phi0) * sin(theta0));
            v[1] = Eigen::Vector3f(r0 * sin(phi0) * cos(theta1),
                                   r0 * cos(phi0),
                                   r0 * sin(phi0) * sin(theta1));
            v[2] = Eigen::Vector3f(r0 * sin(phi1) * cos(theta1),
                                   r0 * cos(phi1),
                                   r0 * sin(phi1) * sin(theta1));
            v[3] = Eigen::Vector3f(r0 * sin(phi1) * cos(theta0),
                                   r0 * cos(phi1),
                                   r0 * sin(phi1) * sin(theta0));

            // Upper quad (extruded along normal)
            v[4] = Eigen::Vector3f(r1 * sin(phi0) * cos(theta0),
                                   r1 * cos(phi0),
                                   r1 * sin(phi0) * sin(theta0));
            v[5] = Eigen::Vector3f(r1 * sin(phi0) * cos(theta1),
                                   r1 * cos(phi0),
                                   r1 * sin(phi0) * sin(theta1));
            v[6] = Eigen::Vector3f(r1 * sin(phi1) * cos(theta1),
                                   r1 * cos(phi1),
                                   r1 * sin(phi1) * sin(theta1));
            v[7] = Eigen::Vector3f(r1 * sin(phi1) * cos(theta0),
                                   r1 * cos(phi1),
                                   r1 * sin(phi1) * sin(theta0));

            // Push vertices
            for (int k = 0; k < 8; ++k)
                push_vertex(v[k]);

            // Indices: 6 faces of a box
            GLuint idx[36] =
            {
                0,1,2, 2,3,0, // bottom
                4,5,6, 6,7,4, // top
                0,1,5, 5,4,0, // front
                1,2,6, 6,5,1, // right
                2,3,7, 7,6,2, // back
                3,0,4, 4,7,3  // left
            };

            for (int k = 0; k < 36; ++k)
                m_indices.push_back(base_index + idx[k]);

            base_index += 8;
        }
    }
}

    void build_spherical_maze_mesh_optionB()
{
    m_vertices.clear();
    m_indices.clear();

    const float sphere_radius  = 1.0f;
    const float wall_height    = 0.1f; // extrusion along normal
    const float cell_size      = 0.15f; // desired wall width along surface
    const float dv             = float(M_PI) / GRID; // latitude step

    auto push_vertex = [&](const Eigen::Vector3f& pos)
    {
        Eigen::Vector3f n = pos.normalized();
        m_vertices.push_back(pos.x());
        m_vertices.push_back(pos.y());
        m_vertices.push_back(pos.z());
        m_vertices.push_back(n.x());
        m_vertices.push_back(n.y());
        m_vertices.push_back(n.z());
    };

    GLuint base_index = 0;

    for (int j = 0; j < GRID; ++j) // latitude
    {
        float phi0 = j * dv;
        float phi1 = (j+1) * dv;
        float phi_center = (phi0 + phi1) / 2.0f;

        // Prevent du from exploding near poles
        float sin_phi = std::max(0.01f, std::sin(phi_center));

        // longitude step for this latitude row
        float du = cell_size / (sphere_radius * sin_phi);

        float theta = 0.0f;
        for (int i = 0; i < GRID; ++i)
        {
            if (!maze[i][j])
            {
                theta += du;
                continue;
            }

            float theta0 = theta;
            float theta1 = theta + du;
            theta += du;

            // radius
            float r0 = sphere_radius;
            float r1 = sphere_radius + wall_height;

            // 8 vertices (lower & upper)
            Eigen::Vector3f v[8];

            // lower quad
            v[0] = Eigen::Vector3f(r0 * sin(phi0) * cos(theta0),
                                   r0 * cos(phi0),
                                   r0 * sin(phi0) * sin(theta0));
            v[1] = Eigen::Vector3f(r0 * sin(phi0) * cos(theta1),
                                   r0 * cos(phi0),
                                   r0 * sin(phi0) * sin(theta1));
            v[2] = Eigen::Vector3f(r0 * sin(phi1) * cos(theta1),
                                   r0 * cos(phi1),
                                   r0 * sin(phi1) * sin(theta1));
            v[3] = Eigen::Vector3f(r0 * sin(phi1) * cos(theta0),
                                   r0 * cos(phi1),
                                   r0 * sin(phi1) * sin(theta0));

            // upper quad
            v[4] = Eigen::Vector3f(r1 * sin(phi0) * cos(theta0),
                                   r1 * cos(phi0),
                                   r1 * sin(phi0) * sin(theta0));
            v[5] = Eigen::Vector3f(r1 * sin(phi0) * cos(theta1),
                                   r1 * cos(phi0),
                                   r1 * sin(phi0) * sin(theta1));
            v[6] = Eigen::Vector3f(r1 * sin(phi1) * cos(theta1),
                                   r1 * cos(phi1),
                                   r1 * sin(phi1) * sin(theta1));
            v[7] = Eigen::Vector3f(r1 * sin(phi1) * cos(theta0),
                                   r1 * cos(phi1),
                                   r1 * sin(phi1) * sin(theta0));

            for (int k = 0; k < 8; ++k)
                push_vertex(v[k]);

            // indices for 6 faces
            GLuint idx[36] =
            {
                0,1,2, 2,3,0, // bottom
                4,5,6, 6,7,4, // top
                0,1,5, 5,4,0, // front
                1,2,6, 6,5,1, // right
                2,3,7, 7,6,2, // back
                3,0,4, 4,7,3  // left
            };

            for (int k = 0; k < 36; ++k)
                m_indices.push_back(base_index + idx[k]);

            base_index += 8;
        }
    }
}


};


class gui::mesh::moon : public gui::mesh::ball
{
public:
    moon() = default;
    virtual ~moon() = default;

    void draw(std::unique_ptr<gui::shader_program>& shader_prog, const Eigen::Matrix4f& transform) override;
};
