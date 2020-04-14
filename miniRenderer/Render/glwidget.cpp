#include "glwidget.h"

#include "../Math/transform.h"

#include <QTimer>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

GLWidget::GLWidget(QWidget *parent) :
    QWidget(parent)
{
    resize(800, 600);
    setWindowTitle("Mini Software Renderer");

    fps = 0;

    gl = std::make_shared<GLFunctions>(width(), height());
    camera = std::make_shared<FPSCamera>();

    // fps 累计
    timer = std::make_shared<QTimer>();
    connect(timer.get(), &QTimer::timeout, this, &GLWidget::fpsTimeOut);
    timer->start(1000);

    setMouseTracking(true); // 使能鼠标移动事件
}

GLWidget::~GLWidget()
{
}

void GLWidget::paintEvent(QPaintEvent *event)
{
    clock_t start, finish;
    start = clock();
    gl->beginFrame();

    if (notInitGL)
    {
        notInitGL = false;
        gl->initialize();
        gl->DepthRange(0.0f, 1.0f);
        gl->Viewport(width() / 8, height() / 8, width() * 3 / 4, height() * 3 / 4);
        initGL();
        changeProjection();
    }
    paintGL(); // 主渲染循环
    update();
    fps++;

    gl->swapFrameBuffer();
    gl->endFrame();

    finish = clock();
    double deltaFrameTime = (double)(finish - start) / CLOCKS_PER_SEC;

    canvas = std::make_shared<QImage>(gl->getFrameResult(), width(), height(), QImage::Format_RGBA8888);

    // TODO:顶点数
    this->num_trangles = 1;
    this->num_vertices = 2;

    if(canvas)
    {
        QPainter painter(this);
        painter.drawImage(0, 0, *canvas);
    }
    QWidget::paintEvent(event);
}

void GLWidget::fpsTimeOut()
{
    this->setWindowTitle("Mini Software Renderer"
                         + QString(" fps: %1 triangles: %2 vertices: %3")
                         .arg(fps).arg(num_trangles).arg(num_vertices));
    fps = 0;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (firstMouseMove)
    {
        firstMouseMove = false;
        preMousePos = event->pos();
    }
    else
    {
        QPoint delta = event->pos() - preMousePos;
        preMousePos = event->pos();
        if (event->buttons() & Qt::LeftButton)
            camera->ProcessMouseMovement(delta.x(), delta.y());
        else if (event->buttons() & Qt::RightButton)
            camera->ProcessMouseMovement(delta.x(), delta.y());;
    }
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "key press";
    switch (event->key()) 
    {
    case Qt::Key_Up:
    case Qt::Key_W:
        camera->ProcessKeyboard(FORWARD, 0.05);
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        camera->ProcessKeyboard(BACKWARD, 0.05);
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        camera->ProcessKeyboard(LEFT, 0.05);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        camera->ProcessKeyboard(RIGHT, 0.05);
        break;
    default:
        sceneKeyEvent(event);
        break;
    }
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    double delta = event->delta();
    camera->ProcessMouseScroll(delta);
    changeProjection();
}

void GLWidget::resizeEvent(QResizeEvent *event)
{
    //TODO resizeEvent
    // resizeGL(width(), height());
}

void GLWidget::resizeGL(int w, int h)
{
    // TODO 改变帧缓冲大小
    changeProjection();
}

void GLWidget::changeProjection()
{
    glm::Matrix projection = glm::perspective(glm::radians(camera->getZoom()), (float)width() / (float)height(), 0.1f, 100.0f);
    setProjection(projection);
}
