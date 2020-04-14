#pragma once
#include "../Render/glwidget.h"
#include "../Render/model.h"
#include "../Shader/phongshader.h"

class SimpleScene : public GLWidget
{
public:
    void initGL() override;
    void paintGL() override;
    void setProjection(glm::Matrix projection) override;

private:
    std::shared_ptr<Model> model;
    std::shared_ptr<PhongShader> shader;
};