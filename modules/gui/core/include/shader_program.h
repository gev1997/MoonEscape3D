#pragma once

// project
#include "namespace_gui.h"

// third party
#include <QOpenGLShaderProgram>

// std

class gui::shader_program : public QOpenGLShaderProgram
{
public:
    shader_program() = default;
    ~shader_program() = default;

    class binder
    {
    public:
        inline binder(shader_program* shader_prog)
            : m_shader_prog(shader_prog)
        {
            Q_ASSERT(m_shader_prog);
            m_shader_prog->bind();
        }

        inline ~binder()
        {
            m_shader_prog->release();
        }

    private:
        Q_DISABLE_COPY(binder)
        shader_program* m_shader_prog;
    };
};
