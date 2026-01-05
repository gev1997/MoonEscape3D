// project
#include "moon_escape_3D_widget.h"

// third party

// std

gui::moon_escape_3D_widget::moon_escape_3D_widget(QWidget* parent)
    : QOpenGLWidget{parent}
{}

gui::moon_escape_3D_widget::~moon_escape_3D_widget()
{
    makeCurrent();
    doneCurrent();
}

void gui::moon_escape_3D_widget::init()
{
    setFocusPolicy(Qt::StrongFocus);

    m_camera = std::make_unique<camera>();
    m_renderer = std::make_unique<renderer>();
}

void gui::moon_escape_3D_widget::initializeGL()
{
    initializeOpenGLFunctions();
    init();
    
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    
    m_renderer->init();
}

void gui::moon_escape_3D_widget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    m_camera->window_resized(width, height);
}

void gui::moon_escape_3D_widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_renderer->draw(m_camera);
}

void gui::moon_escape_3D_widget::mousePressEvent(QMouseEvent* event)
{
    m_camera->mouse_press(event);
    update();
}

void gui::moon_escape_3D_widget::mouseMoveEvent(QMouseEvent* event)
{
    m_camera->mouse_move(event);
    update();
}

void gui::moon_escape_3D_widget::wheelEvent(QWheelEvent* event)
{
    m_camera->mouse_wheel(event);
    update();
}

void gui::moon_escape_3D_widget::keyPressEvent(QKeyEvent* event)
{
    m_camera->key_press(event);
    update();
}
