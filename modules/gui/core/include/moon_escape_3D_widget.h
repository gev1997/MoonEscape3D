#pragma once

// project
#include "namespace_gui.h"
#include "camera.h"
#include "renderer.h"

// third party
#include <QOpenGLWidget>
// #include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>

// std

class gui::moon_escape_3D_widget final : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    explicit moon_escape_3D_widget(QWidget* parent = nullptr);
    ~moon_escape_3D_widget();

protected:
    void init();
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    std::unique_ptr<camera> m_camera;
    std::unique_ptr<renderer> m_renderer;
};
