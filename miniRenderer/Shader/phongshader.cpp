#include "phongshader.h"


VertexOut PhongShader::vertex_shader(const VertexIn &in)
{
    VertexOut res;
    res.FragPos = model * in.cam_pos;
    res.Normal = glm::Vec3f(model * glm::Vec4f(in.normal, 1.0));
    res.TexCoords = in.texcoord;
    res.gl_position = projection * view * res.FragPos;
    return res;
}

bool PhongShader::fragment_shader(const VertexOut &out, glm::Vec4f& gl_FragColor, Model* model)
{
    glm::Vec3f normal = out.Normal;
    glm::Vec2f uv = out.TexCoords;

    TGAColor diffuse = model->diffuse(uv);

    gl_FragColor = glm::Vec4f(diffuse.bgra[0] / 256.f, diffuse.bgra[1] / 256.f, diffuse.bgra[2] / 256.f, diffuse.bgra[3] / 256.f);
    return false;
}
