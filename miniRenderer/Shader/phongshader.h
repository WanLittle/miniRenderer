#pragma once

#include <memory>
#include <algorithm>

#include "baseshader.h"

#include "../Math/mat.h"
#include "../Math/vec.h"
#include "../Render/model.h"

class PhongShader : public BaseShader
{
public:
    glm::Matrix projection, view, model;
    glm::Vec3f light_dir;

public:
    virtual VertexOut vertex_shader(const VertexIn &in) override;

    virtual bool fragment_shader(const VertexOut &out, glm::Vec4f& gl_FragColor, Model* m_model) override;
};