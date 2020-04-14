#pragma once

#include <QWidget>
#include <memory>

#include "pipeline.h"
#include "../Camera/camera.h"
#include "../Camera/fpscamera.h"
#include "../Camera/tpscamera.h"

class QImage;
class QTimer;
class GLFunctions;

class GLWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GLWidget(QWidget *parent = nullptr);
    ~GLWidget();

private:
    void paintEvent(QPaintEvent *) override;

protected slots:
    void fpsTimeOut();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    virtual void initGL() = 0;                 // 初始化GL
    virtual void paintGL() = 0;                // 主渲染循环
    virtual void resizeGL(int w, int h);       // 改变窗口大小
    virtual void sceneKeyEvent(QKeyEvent* event) {}
    virtual void changeProjection();
    virtual void setProjection(glm::Matrix projection) {}

private:
    int fps;
    bool firstMouseMove = true;
    bool notInitGL = true;
    QPoint preMousePos;
    std::shared_ptr<QTimer> timer;
    std::shared_ptr<QImage> canvas;
    int num_trangles, num_vertices;

public:
    std::shared_ptr<GLFunctions> gl;
    std::shared_ptr<FPSCamera> camera;
};
