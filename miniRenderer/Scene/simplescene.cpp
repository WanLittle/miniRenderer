#include "simplescene.h"
#include "../Math/transform.h"

void SimpleScene::initGL()
{
    model = std::make_shared<Model>("../assets/african_head/african_head.obj");
    //model = std::make_shared<Model>("../assets/diablo3_pose/diablo3_pose.obj");
    //model = std::make_shared<Model>("../assets/floor/floor.obj");
    shader = std::make_shared<PhongShader>();

    camera->setCameraPos(glm::vec3f(0, -1.5, -3));
    camera->lookToPos(glm::vec3f(0, -1, 0));
    //camera->setTarget(glm::Vec3f(0, -1, 0));

    shader->model = glm::Matrix::identity();
}

void SimpleScene::paintGL()
{
    shader->view = camera->GetViewMatrix();
    gl->clearFrameBuffer(glm::vec4f(1.0, 1.0, 1.0, 1.0));
    model->draw(gl, shader);
}


void SimpleScene::setProjection(glm::Matrix projection)
{
    shader->projection = projection;
}

