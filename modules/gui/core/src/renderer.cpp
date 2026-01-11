#pragma once

// project
#include "renderer.h"
#include "factory.h"

// third party
#include <QOpenGLFunctions>

// std

gui::renderer::renderer()
{
    m_shader_prog = std::make_unique<shader_program>();
    m_meshes.push_back(mesh::factory::create(mesh::object_type::PlayerBall));
    m_meshes.push_back(mesh::factory::create(mesh::object_type::Moon));
    m_meshes.push_back(mesh::factory::create(mesh::object_type::Axis));
}

void gui::renderer::init()
{
    _init_shaders();
    _init_meshes();
}

void gui::renderer::draw(const std::unique_ptr<camera>& camera)
{
    shader_program::binder binder(m_shader_prog.get());

    QOpenGLFunctions* gl_functions = QOpenGLContext::currentContext()->functions();
    gl_functions->glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    const auto projection = utils::eigen_to_qmatrix(camera->get_projection_matrix());
    const auto view = utils::eigen_to_qmatrix(camera->get_view_matrix());
    
    // TODO: need to call every time since we have one shader. ?//////
    m_shader_prog->setUniformValue("lightPos", 0.0f, 5.0f, 0.0f);/////
    m_shader_prog->setUniformValue("objectColor", 0.8f, 0.8f, 0.8f);//
    //////////////////////////////////////////////////////////////////
    
    m_shader_prog->setUniformValue("projection", projection);
    m_shader_prog->setUniformValue("view", view);
    
    for (auto& mesh : m_meshes)
    {
        mesh->draw(m_shader_prog, camera->get_model_matrix());
    }
}

void gui::renderer::_init_shaders()
{
    m_shader_prog->addShaderFromSourceFile(QOpenGLShader::Vertex, "D:\\Project\\MoonEscape3D\\shaders\\basic\\vertex.glsl");
    m_shader_prog->addShaderFromSourceFile(QOpenGLShader::Fragment, "D:\\Project\\MoonEscape3D\\shaders\\basic\\fragment.glsl");
    m_shader_prog->link();
}

void gui::renderer::_init_meshes()
{
    for (auto& mesh : m_meshes)
    {
        mesh->init();
        _init_vertex_attributes();
        mesh->release();
    }
}

void gui::renderer::_init_vertex_attributes()
{
    shader_program::binder binder(m_shader_prog.get());

    m_shader_prog->setAttributeBuffer(0, GL_FLOAT, 0, 3, 6 * sizeof(GLfloat));
    m_shader_prog->enableAttributeArray(0);
    m_shader_prog->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));
    m_shader_prog->enableAttributeArray(1);
}
