#include "phongshader.h"


VertexOut PhongShader::vertex_shader(const VertexIn &in)
{
    VertexOut res;
    res.FragPos = model * in.cam_pos;
    res.Normal = glm::vec3f(model * glm::vec4f(in.normal, 1.0));
    res.TexCoords = in.texcoord;
    res.gl_position = projection * view * res.FragPos;
    return res;
}

bool PhongShader::fragment_shader(const VertexOut &out, glm::vec4f& gl_FragColor, Model* model)
{
    glm::vec3f normal = out.Normal;
    glm::vec2f uv = out.TexCoords;

    glm::vec4f diffuse = model->diffuse(uv);

    gl_FragColor = diffuse;
    return false;
}
