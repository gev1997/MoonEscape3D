#pragma once

// project
#include "namespace_gui.h"
#include "shader_program.h"
#include "object_base.h"
#include "camera.h"
#include "utils.h"

// third party

// std

class gui::renderer
{
public:
    renderer();
    ~renderer() = default;

    void init();
    void draw(const std::unique_ptr<camera>& camera);

private:
    void _init_shaders();
    void _init_meshes();
    void _init_vertex_attributes();

private:
    std::unique_ptr<shader_program> m_shader_prog;
    std::vector<std::unique_ptr<mesh::object_base>> m_meshes;
};
