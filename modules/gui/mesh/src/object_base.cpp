#pragma once

// project
#include "object_base.h"

// third party

// std

void gui::mesh::object_base::init()
{
    m_VAO.create();
    m_VAO.bind();
    
    m_VBO.create();
    m_VBO.bind();
    m_VBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_VBO.allocate(m_vertices.data(), m_vertices.size() * sizeof(GLfloat));
    
    m_EBO.create();
    m_EBO.bind();
    m_EBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_EBO.allocate(m_indices.data(), m_indices.size() * sizeof(GLuint));
}

void gui::mesh::object_base::release()
{
    m_VAO.release();
}
